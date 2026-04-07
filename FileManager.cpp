#include "FileManager.h"
#include "ErrorManager.h"
#include <filesystem>
#include <fstream>
#include <algorithm>

namespace fs = std::filesystem;

// Get all files with a specific extension in the current directory
std::vector<std::string> FileManager::getFileNames(const std::string& extension) {
    std::vector<std::string> files;

    try {
        // Go through all files in current directory
        for (const auto& entry : fs::directory_iterator(".")) {
            if (entry.is_regular_file()) {
                if (entry.path().extension() == extension) {
                    files.push_back(entry.path().filename().string());
                }
            }
        }
    }
    catch (...) {
        // If something goes wrong, just return empty vector
    }

    // Sort files alphabetically
    std::sort(files.begin(), files.end());
    return files;
}

// Read all lines from a file
std::vector<std::string> FileManager::readLines(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw GameException("Cannot open file: " + filename);
    }

    std::vector<std::string> lines;
    std::string line;
    bool isFirstLine = true;

    while (std::getline(file, line)) {
        // Remove \r if the file has Windows line endings
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        // Handle BOM (byte order mark) on first line
        if (isFirstLine) {
            if (line.size() >= 3) {
                unsigned char first = line[0];
                if (first == 0xEF) {
                    line = line.substr(3);
                }
            }
            isFirstLine = false;
        }

        lines.push_back(line);
    }

    file.close();
    return lines;
}

// Write lines to a file
void FileManager::writeLines(const std::string& filename, const std::vector<std::string>& lines) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw GameException("Cannot write to: " + filename);
    }

    for (const auto& line : lines) {
        file << line << std::endl;
    }

    file.close();
}

// Check if a file exists
bool FileManager::fileExists(const std::string& filename) {
    return fs::exists(filename) && fs::is_regular_file(filename);
}

// Copy a file from source to destination
void FileManager::copyFile(const std::string& source, const std::string& destination) {
    std::error_code error;
    fs::copy_file(source, destination, fs::copy_options::overwrite_existing, error);

    if (error) {
        throw GameException("Cannot copy " + source + " to " + destination);
    }
}