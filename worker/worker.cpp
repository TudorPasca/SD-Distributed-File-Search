#include <crow/app.h>
#include <crow/middlewares/cors.h>
#include <memory>
#include <string>
#include <vector>
#include "include/FileScraper/FileScraper.h"
#include "include/FileScraper/RegexFileIgnorer.h"
#include "include/Controller/SearchController.h"

int main(int argc, char *argv[]) {
    int port = 18080;

    if (argc > 1) {
        try {
            port = std::stoi(argv[1]);
        } catch (const std::exception& e) {
            std::cerr << "Invalid port number: " << argv[1] << std::endl;
            std::cerr << "Used at: " << argv[0] << " [port]" << std::endl;
            return 1;
        }
    }

    std::vector<std::string> ignorePatterns = {R"(^.*\.(?!(txt|json|cpp|c|h|py|java|in|out)$)[A-Za-z0-9]+$)"};
    auto ignorer = std::make_shared<RegexFileIgnorer>(ignorePatterns);
    auto fileScraper = std::make_shared<FileScraper>(ignorer);
    auto searchController = std::make_shared<SearchController>(fileScraper);

    crow::App<crow::CORSHandler> app;
    searchController->registerRoutes(app);

    app.port(port).multithreaded().run();

    return 0;
}