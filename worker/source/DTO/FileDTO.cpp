#include "../../include/DTO/FileDTO.h"

#include <fstream>
#include <sstream>

FileDTO::FileDTO(std::string name, std::string  absolutePath, bool folder, std::string contents):
        name(std::move(name)), absolutePath(std::move(absolutePath)), folder(folder), contents(std::move(contents)) {}

FileDTO::FileDTO(const std::filesystem::directory_entry &entry) {
    name = entry.path().filename().string();
    absolutePath = entry.path().string();
    folder = entry.is_directory();
    if (!folder) {
        std::ifstream file(absolutePath);
        if (file) {
            std::ostringstream oss;
            oss << file.rdbuf();
            contents = oss.str();
        }
    }
}

const std::string& FileDTO::getName() const { return name; }

const std::string& FileDTO::getAbsolutePath() const { return absolutePath; }

const bool FileDTO::isFolder() const { return folder; }

const std::string& FileDTO::getContents() const { return contents; }