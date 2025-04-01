#include "../../include/FileScraper/FileScraper.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

bool FileScraper::matchesFilter(const fs::path& filePath, const std::string& filter) const {
    if (filter.empty()) {
        return true;
    }
    std::string pathStr = filePath.string();
    return (pathStr.find(filter) != std::string::npos);
}

std::vector<FileDTO> FileScraper::getRegularFiles(const fs::directory_entry& entry) const {
    if (!entry.exists())
        return {};
    if (entry.is_regular_file()) {
        return {FileDTO(entry)};
    }
    std::vector<FileDTO> files;
    for (const auto& it: fs::directory_iterator(entry, fs::directory_options::skip_permission_denied)) {
        if (ignorer != nullptr && ignorer->shouldIgnore(it.path().string()))
            continue;
        if (it.is_regular_file()) {
            files.emplace_back(it);
        }
    }
    return files;
}

std::vector<FileDTO> FileScraper::getRegularFiles(const fs::path& path) const {
    fs::directory_entry entry{path};
    return getRegularFiles(entry);
}

std::vector<FileDTO> FileScraper::getFilesRecursively(const std::filesystem::directory_entry& entry) const {
    if (!entry.exists()) {
        return {};
    }
    if (entry.is_regular_file()) {
        return {FileDTO(entry)};
    }
    std::vector<FileDTO> files;
    std::error_code err;
    for (const auto& it : fs::recursive_directory_iterator(entry, fs::directory_options::skip_permission_denied, err)) {
        if (err) {
            std::cerr << "Iterator error: " << err.message() << std::endl;
            err.clear();
            continue;
        }
        if (ignorer != nullptr && ignorer->shouldIgnore(it.path().string()))
            continue;
        files.emplace_back(it);
    }
    return files;
}

std::vector<FileDTO> FileScraper::getFilesRecursively(const fs::path& path) const {
    fs::directory_entry entry{path};
    return getFilesRecursively(entry);
}

std::vector<FileDTO> FileScraper::getRegularFiles(const fs::directory_entry& entry, const std::string& filter) const {
    std::vector<FileDTO> files;
    if (!entry.exists())
        return files;
    if (entry.is_regular_file() && matchesFilter(entry.path(), filter)) {
        return {FileDTO(entry)};
    }
    for (const auto& it : fs::directory_iterator(entry, fs::directory_options::skip_permission_denied)) {
        if (ignorer && ignorer->shouldIgnore(it.path().string()))
            continue;
        if (it.is_regular_file() && matchesFilter(it.path(), filter)) {
            files.emplace_back(it);
        }
    }
    return files;
}

std::vector<FileDTO> FileScraper::getRegularFiles(const fs::path& path, const std::string& filter) const {
    fs::directory_entry entry{path};
    return getRegularFiles(entry, filter);
}

std::vector<FileDTO> FileScraper::getFilesRecursively(const fs::directory_entry& entry, const std::string& filter) const {
    std::vector<FileDTO> files;
    if (!entry.exists())
        return files;
    if (entry.is_regular_file() && matchesFilter(entry.path(), filter)) {
        return {FileDTO(entry)};
    }
    std::error_code err;
    for (const auto& it : fs::recursive_directory_iterator(entry, fs::directory_options::skip_permission_denied, err)) {
        if (err) {
            std::cerr << "Iterator error: " << err.message() << std::endl;
            err.clear();
            continue;
        }
        if (ignorer && ignorer->shouldIgnore(it.path().string()))
            continue;
        if (matchesFilter(it.path(), filter)) {
            files.emplace_back(it);
        }
    }
    return files;
}

std::vector<FileDTO> FileScraper::getFilesRecursively(const fs::path& path, const std::string& filter) const {
    fs::directory_entry entry{path};
    return getFilesRecursively(entry, filter);
}