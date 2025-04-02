#pragma once
#include <string>

struct WorkerConfig {
    const unsigned int port;
    const std::string searchPath;

    WorkerConfig(unsigned int port, std::string searchPath)
            : port(port), searchPath(std::move(searchPath)) {}
};