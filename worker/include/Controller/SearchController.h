#pragma once

#include <memory>
#include <crow/app.h>
#include <crow/middlewares/cors.h>
#include "../FileScraper/FileScraper.h"
#include "../DTO/FileDTO.h"

class SearchController {
public:
    explicit SearchController(std::shared_ptr<FileScraper> fileScraper):
        fileScraper(std::move(fileScraper)) {}

    void registerRoutes(crow::App<crow::CORSHandler> &app);
private:
    std::shared_ptr<FileScraper> fileScraper;

    static crow::json::wvalue fileDTOToJson(const FileDTO& fileDto);
};