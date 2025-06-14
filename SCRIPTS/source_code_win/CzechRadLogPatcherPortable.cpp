#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>

namespace fs = std::filesystem;

// Function to calculate XOR checksum between '$' and '*' (exclusive)
std::string calculateChecksum(const std::string& line) {
    unsigned char checksum = 0;
    bool between = false;
    for (char c : line) {
        if (c == '$') {
            between = true;
            continue;
        }
        if (c == '*') break;
        if (between) checksum ^= static_cast<unsigned char>(c);
    }
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)checksum;
    return ss.str();
}

// Function to make a value positive by removing the minus sign
std::string makePositive(const std::string& value) {
    if (!value.empty() && value[0] == '-') {
        return value.substr(1);
    }
    return value;
}

// Function to process a single LOG file
void processLogFile(const fs::path& inputPath, const fs::path& outputPath) {
    std::ifstream inFile(inputPath);
    std::ofstream outFile(outputPath);
    std::string line;

    while (std::getline(inFile, line)) {
        if (line.empty()) {
            outFile << line << "\n";
            continue;
        }

        if (line[0] == '#') {
            // Copy comment lines unchanged
            outFile << line << "\n";
            continue;
        }

        if (line[0] == '$') {
            // Replace CZRA1 with CZRDD
            size_t pos = line.find("CZRA1");
            if (pos != std::string::npos) {
                line.replace(pos, 5, "CZRDD");
            }

            // Find the position of the checksum asterisk
            size_t asteriskPos = line.find('*');
            if (asteriskPos == std::string::npos) {
                // Copy malformed lines unchanged
                outFile << line << "\n";
                continue;
            }

            // Extract the part before the asterisk
            std::string dataPart = line.substr(0, asteriskPos);
            std::string checksumPart = line.substr(asteriskPos);

            // Split the data part into columns
            std::vector<std::string> columns;
            std::stringstream ss(dataPart);
            std::string item;
            while (std::getline(ss, item, ',')) {
                columns.push_back(item);
            }

            // Ensure we have exactly 15 columns (14 commas + last column before asterisk)
            if (columns.size() == 15) {
                // Make 8th and 10th columns positive (index 7 and 9)
                columns[7] = makePositive(columns[7]);
                columns[9] = makePositive(columns[9]);

                // Reconstruct the line up to the 14th column
                std::string newLine = columns[0];
                for (size_t i = 1; i < 14; ++i) {
                    newLine += "," + columns[i];
                }
                newLine += "," + columns[14];

                // Calculate new checksum
                std::string newChecksum = calculateChecksum(newLine);
                newLine += "*" + newChecksum;

                outFile << newLine << "\n";
            } else {
                // Copy malformed lines unchanged
                outFile << line << "\n";
            }
        } else {
            // Copy other lines unchanged
            outFile << line << "\n";
        }
    }

    inFile.close();
    outFile.close();
}

int main() {
    // Create input and output directories if they don't exist
    fs::create_directory("input");
    fs::create_directory("output");

    // Get current time for backup folder name
    auto now = std::time(nullptr);
    std::stringstream backupDirName;
    backupDirName << std::put_time(std::localtime(&now), "%Y-%m-%d_%H-%M_backup");

    // Create backup directory in output
    fs::path backupDir = fs::path("output") / backupDirName.str();
    fs::create_directory(backupDir);

    // Process all .LOG and .log files in input directory
    for (const auto& entry : fs::recursive_directory_iterator("input")) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            if (ext == ".LOG" || ext == ".log") {
                // Copy original file to backup directory
                fs::path backupPath = backupDir / entry.path().filename();
                fs::copy_file(entry.path(), backupPath, fs::copy_options::overwrite_existing);

                // Create new filename with _l suffix
                std::string newFilename = entry.path().stem().string() + "_l" + entry.path().extension().string();
                fs::path newPath = fs::path("input") / newFilename;

                // Process the file
                processLogFile(entry.path(), newPath);

                // Move processed file to output
                fs::path finalPath = fs::path("output") / newFilename;
                fs::rename(newPath, finalPath);

                // Remove original file
                fs::remove(entry.path());
            }
        }
    }

    std::cout << "Processing complete. All files have been processed and moved to the output folder.\n";
    std::cout << "Backup created in: " << backupDir.string() << "\n";
    std::cout << "Press Enter to close...\n";
    std::cin.get();

    return 0;
}

// Created with Grok AI, released under MIT No Attribution License
// Copyright 2025 Jan Helebrant, czechrad@suro.cz, www.suro.cz

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so.

// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

