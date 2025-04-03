#include "../../include/Controller/SearchController.h"
#include <filesystem>
#include <algorithm>

void SearchController::registerRoutes(crow::App<crow::CORSHandler>& app)
{
    CROW_ROUTE(app, "/search").methods(crow::HTTPMethod::Post)
            ([this](const crow::request& req) {
                auto body = crow::json::load(req.body);
                if (!body)
                {
                    return crow::response(400, "Invalid JSON");
                }

                std::string searchPathStr = body["searchPath"].s();
                std::filesystem::path searchPath(searchPathStr);

                std::string filter = body["filter"].s();

                std::vector<FileDTO> results = fileScraper->getFilesRecursively(searchPath, filter);

                crow::json::wvalue resJson;
                resJson["results"] = crow::json::wvalue(crow::json::type::List);
                int index = 0;
                for (const auto& file : results)
                {
                    resJson["results"][index++] = file.toJson();
                }

                return crow::response{resJson};
            });
}