#pragma once

#include <crow/app.h>
#include <crow/middlewares/cors.h>
#include <vector>
#include <string>
#include "../Config/WorkerConfig.h"

class SearchController {
public:
    explicit SearchController(std::vector<WorkerConfig> workerConfigs)
        : workerConfigs(workerConfigs) {}
    void registerRoutes(crow::App<crow::CORSHandler> &app);

private:
    std::vector<WorkerConfig> workerConfigs;
    crow::json::wvalue forwardRequest(const std::string &filter);
};