#include "include/Config/WorkerConfig.h"
#include "include/Controller/SearchController.h"
#include "include/Cache/LRUCache.h"
#include <boost/process.hpp>
#include <iostream>
#include <vector>
#include <filesystem>
#include <csignal>
#include <crow.h>
#include <crow/middlewares/cors.h>

namespace bp = boost::process;

std::vector<bp::child> workers;
crow::App<crow::CORSHandler> app;

void handleShutdown(int signal) {
    for (auto &worker : workers) {
        if (worker.running()) {
            worker.terminate();
            worker.wait();
        }
    }
    app.stop();
}

int main() {
    std::signal(SIGINT, handleShutdown);

    std::vector<WorkerConfig> workerConfigs = {
            WorkerConfig(18081, "C:\\test"),
            WorkerConfig(18082, "D:\\Tudor\\School")
    };
    std::string workerExecutable = (std::filesystem::current_path() / "worker.exe").string();
    try {
        for (const auto& config : workerConfigs) {
            workers.emplace_back(workerExecutable, std::to_string(config.port),
                                 bp::std_out > bp::null, bp::std_err > bp::null);

            std::cout << "[Orchestrator] Launched worker on port " << config.port
                      << " with search path " << config.searchPath << std::endl;
        }
        std::cout << "[Orchestrator] Press STOP button to kill workers...\n";
    }
    catch (const std::exception &ex) {
        std::cerr << "[Orchestrator] Error launching worker process: " << ex.what() << std::endl;
        return 1;
    }

    auto &cors = app.get_middleware<crow::CORSHandler>();
    cors.global()
            .origin("http://localhost:4200")
            .allow_credentials()
            .methods("POST"_method, "GET"_method);

    const size_t CACHE_SIZE = 3;
    std::shared_ptr<ICache<std::string, std::vector<FileDTO>>> cache =
            std::make_shared<LRUCache<std::string, std::vector<FileDTO>>>(CACHE_SIZE);
    SearchController searchController(workerConfigs, cache);
    searchController.registerRoutes(app);

    app.port(18080).multithreaded().run();

    return 0;
}