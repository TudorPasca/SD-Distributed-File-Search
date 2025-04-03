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
                auto result = forwardRequest(filter);
                return crow::response(200, result.dump());
            });
}

crow::json::wvalue SearchController::forwardRequest(const std::string &filter) {
    crow::json::wvalue responseJson;
    std::vector<crow::json::wvalue> workerResponses;
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
                    workerResponses.push_back(item);
                }
            }
        } else {
            std::cerr << "[Orchestrator - Controller] Failed to reach worker at " << workerUrl << std::endl;
        }
    }
    responseJson["results"] = std::move(workerResponses);
    return responseJson;
}