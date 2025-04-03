#pragma once

#include <crow.h>
#include <string>
#include <utility>
#include <filesystem>

class FileDTO {
public:
    FileDTO(std::string  name, std::string  absolutePath, bool folder, std::string contents);
    explicit FileDTO(const std::filesystem::directory_entry &entry);
    explicit FileDTO(const crow::json::rvalue &json);

    const std::string& getName() const;
    const std::string& getAbsolutePath() const;
    const bool isFolder() const;
    const std::string& getContents() const;
    crow::json::wvalue to_json() const;
private:
    std::string name;
    std::string absolutePath;
    bool folder;
    std::string contents;
};