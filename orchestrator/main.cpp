#include <boost/process.hpp>
#include <iostream>
#include "./include/WorkerConfig.h"
#include <vector>
#include <filesystem>
#include <csignal>

namespace bp = boost::process;

std::vector<bp::child> workers;

void handleShutdown(int signal) {
    std::cout << "\n[Orchestrator] Terminating workers...\n";
    for (auto &worker : workers) {
        if (worker.running()) {
            worker.terminate();
            worker.wait();
        }
    }
    std::cout << "[Orchestrator] All workers terminated. Exiting.\n";
    std::exit(0);
}

int main() {
    std::vector<WorkerConfig> workerConfigs = {
            WorkerConfig(18081, "C:/test")
    };

    std::string workerExecutable = (std::filesystem::current_path() / "worker.exe").string();

    std::signal(SIGINT, handleShutdown);

    try {
        for (const auto& config : workerConfigs) {
            workers.emplace_back(workerExecutable, std::to_string(config.port),
                                 bp::std_out > bp::null, bp::std_err > bp::null);

            std::cout << "[Orchestrator] Launched worker on port " << config.port
                      << " with search path " << config.searchPath << std::endl;
        }
        std::cout << "[Orchestrator] Press STOP button to kill workers...\n";
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    catch (const std::exception &ex) {
        std::cerr << "[Orchestrator] Error launching worker process: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}