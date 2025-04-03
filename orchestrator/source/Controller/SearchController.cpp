#include "../../include/Controller/SearchController.h"
#include <cpr/cpr.h>

void SearchController::registerRoutes(crow::App<crow::CORSHandler> &app) {
    CROW_ROUTE(app, "/search").methods(crow::HTTPMethod::POST)
            ([this](const crow::request &req) {
                auto body = crow::json::load(req.body);
                if (!body || !body.has("filter")) {
                    return crow::response(400, "Missing 'filter' in request.");
                }
                std::string filter = body["filter"].s();
                std::vector<FileDTO> result;
                if (cache->exists(filter)) {
                    result = cache->get(filter);
                    std::cout << "[Orchestrator - SearchController] Cache HIT" << std::endl;
                } else {
                    result = getWorkerResults(filter);
                    cache->insert(filter, result);
                    std::cout << "[Orchestrator - SearchController] Cache MISS" << std::endl;
                }
                return crow::response(200, vectorToJson(result).dump());
            });
}

std::vector<FileDTO> SearchController::getWorkerResults(const std::string &filter) {
    std::vector<FileDTO> aggregatedResults;
    for (const auto &worker: workerConfigs) {
        std::string workerUrl = "http://localhost:" + std::to_string(worker.port) + "/search";

        cpr::Response r = cpr::Post(cpr::Url{workerUrl},
                                    cpr::Header{{"Content-Type", "application/json"}},
                                    cpr::Body{crow::json::wvalue({
                                        {"searchPath", worker.searchPath},
                                        {"filter",     filter}}).dump()});
        if (r.status_code == 200) {
            crow::json::rvalue response = crow::json::load(r.text);
            if (response.has("results") && response["results"].t() == crow::json::type::List) {
                for (const auto &item: response["results"]) {
                    aggregatedResults.emplace_back(item);
                }
            }
        } else {
            std::cerr << "[Orchestrator - Controller] Failed to reach worker at " << workerUrl << std::endl;
        }
    }
    return aggregatedResults;
}

crow::json::wvalue SearchController::vectorToJson(const std::vector<FileDTO>& files) {
    crow::json::wvalue jsonResponse;
    std::vector<crow::json::wvalue> results;
    for (const auto &file : files) {
        results.push_back(file.toJson());
    }
    jsonResponse["results"] = std::move(results);
    return jsonResponse;
}