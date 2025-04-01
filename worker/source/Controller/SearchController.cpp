#include "../../include/Controller/SearchController.h"

void SearchController::registerRoutes(crow::App<crow::CORSHandler>& app)
{
    CROW_ROUTE(app, "/search").methods(crow::HTTPMethod::Post)
            ([this](const crow::request& req) {
                auto body = crow::json::load(req.body);
                if (!body)
                {
                    return crow::response(400, "Invalid JSON");
                }

                std::string searchPath = body["searchPath"].s();
                std::string filter = body["filter"].s();

                std::vector<FileDTO> results = fileScraper->getFilesRecursively(searchArea, filter);

                crow::json::wvalue resJson;
                resJson["results"] = crow::json::wvalue(crow::json::type::List);
                int index = 0;
                for (const auto& file : results)
                {
                    resJson["results"][index++] = fileDTOToJson(file);
                }

                return crow::response{resJson};
            });
}

crow::json::wvalue SearchController::fileDTOToJson(const FileDTO &file) {
    crow::json::wvalue obj;
    obj["filename"] = file.getName();
    obj["contents"] = file.getContents();
    obj["path"] = file.getAbsolutePath();
    obj["isFolder"] = file.isFolder();
    return obj;
}