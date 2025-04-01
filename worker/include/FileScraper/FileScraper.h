#pragma once

#include <vector>
#include <string>
#include "../DTO/FileDTO.h"
#include "IFileScraper.h"
#include "IFileIgnorer.h"

class FileScraper: public IFileScraper {
public:
    FileScraper() = default;
    explicit FileScraper(std::shared_ptr<IFileIgnorer> ignorer = nullptr)
            : ignorer(std::move(ignorer)) {}
    std::vector<FileDTO> getRegularFiles(const std::filesystem::directory_entry& entry) const override;
    std::vector<FileDTO> getRegularFiles(const std::filesystem::path& path) const override;
    std::vector<FileDTO> getFilesRecursively(const std::filesystem::directory_entry& entry) const override;
    std::vector<FileDTO> getFilesRecursively(const std::filesystem::path& path) const override;

    std::vector<FileDTO> getRegularFiles(const std::filesystem::directory_entry& entry, const std::string& filter) const;
    std::vector<FileDTO> getRegularFiles(const std::filesystem::path& path, const std::string& filter) const;
    std::vector<FileDTO> getFilesRecursively(const std::filesystem::directory_entry& entry, const std::string& filter) const;
    std::vector<FileDTO> getFilesRecursively(const std::filesystem::path& path, const std::string& filter) const;
private:
    std::shared_ptr<IFileIgnorer> ignorer;
    bool matchesFilter(const std::filesystem::path& filePath, const std::string& filter) const;
};