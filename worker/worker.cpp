#include <crow/app.h>
#include <crow/middlewares/cors.h>
#include <memory>
#include <string>
#include <vector>
#include "FileScraper.h"
#include "FileDTO.h"
#include "SearchController.h"

int main() {
    crow::SimpleApp app;

    std::vector<std::string> ignorePatterns = {R"(^.*\.(?!(txt|json|cpp|c|h|py|java|in|out)$)[A-Za-z0-9]+$)"};
    auto ignorer = std::make_shared<RegexFileIgnorer>(ignorePatterns);
    auto fileScraper = std::make_shared<FileScraper>(ignorer);
    auto searchController = std::make_shared<SearchController>(fileScraper);

    crow::App<crow::CORSHandler> app;
    auto &cors = app.get_middleware<crow::CORSHandler>();
    cors.global()
            .origin("http://localhost:4200")
            .allow_credentials()
            .methods("POST"_method, "GET"_method);

    searchController->registerRoutes(app);

    app.port(18080).multithreaded().run();

    return 0;
}