#pragma once

#include <crow/app.h>
#include <crow/middlewares/cors.h>
#include <vector>
#include <string>
#include "../Config/WorkerConfig.h"
#include "../Cache/ICache.h"
#include "../../../worker/include/DTO/FileDTO.h"

class SearchController {
public:
    SearchController(std::vector<WorkerConfig> workerConfigs, std::shared_ptr<ICache<std::string, std::vector<FileDTO>>> cache)
        : workerConfigs(std::move(workerConfigs)), cache(std::move(cache)) {}
    void registerRoutes(crow::App<crow::CORSHandler> &app);

private:
    std::vector<WorkerConfig> workerConfigs;
    std::shared_ptr<ICache<std::string, std::vector<FileDTO>>> cache;
    std::vector<FileDTO> getWorkerResults(const std::string &filter);
    crow::json::wvalue vectorToJson(const std::vector<FileDTO>& files);
};