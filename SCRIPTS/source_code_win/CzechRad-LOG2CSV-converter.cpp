#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <windows.h>
#include <iomanip>
#include <algorithm>

namespace fs = std::filesystem;

// Function to convert ddmm.mmmm format to decimal degrees
double convertToDecimalDegrees(const std::string& coord, char direction) {
    // Extract degrees and minutes
    size_t dotPos = coord.find('.');
    if (dotPos == std::string::npos) return 0.0;
    
    std::string degreesStr = coord.substr(0, coord.length() - 7); // dd or ddd
    std::string minutesStr = coord.substr(coord.length() - 7);   // mm.mmmm
    
    double degrees = std::stod(degreesStr);
    double minutes = std::stod(minutesStr);
    
    // Convert to decimal degrees: degrees + (minutes / 60)
    double decimalDegrees = degrees + (minutes / 60.0);
    
    // Apply direction (N/S for latitude, E/W for longitude)
    if (direction == 'S' || direction == 'W') {
        decimalDegrees *= -1.0;
    }
    
    return decimalDegrees;
}

// Function to process a single LOG file and convert to CSV
bool processLogFile(const fs::path& logPath, const fs::path& outputDir) {
    std::ifstream inFile(logPath);
    if (!inFile.is_open()) {
        std::cerr << "Error opening input file: " << logPath << std::endl;
        return false;
    }

    // Prepare output CSV file
    fs::path csvPath = outputDir / logPath.filename().replace_extension(".csv");
    std::ofstream outFile(csvPath);
    if (!outFile.is_open()) {
        std::cerr << "Error opening output file: " << csvPath << std::endl;
        inFile.close();
        return false;
    }

    // Write CSV header
    outFile << "device,device_type,device_ID,date,time_UTC,CPM,ADER_microSvh,latitude,longitude,altitude,pulses5s,pulses_total,GPS_validity,Sat,HDOP\n";

    std::string line;
    std::string device, device_type, device_id;
    bool firstDataLine = true;

    while (std::getline(inFile, line)) {
        // Skip comment lines
        if (line.empty() || line[0] == '#') continue;

        // Process data lines starting with $
        if (line[0] == '$') {
            std::stringstream ss(line);
            std::vector<std::string> columns;
            std::string token;

            // Split by commas
            while (std::getline(ss, token, ',')) {
                columns.push_back(token);
            }

            // Validate column count (at least 16 including checksum)
            if (columns.size() < 15) continue;

            // Extract device prefix and static fields
            if (firstDataLine) {
                std::string prefix = columns[0].substr(1, 2); // After $
                device = (prefix == "BN") ? "Safecast" : (prefix == "CZ") ? "CzechRad" : "Unknown";
                device_type = columns[0].substr(1); // e.g., CZRA1 or BNRDD
                device_id = columns[1];             // e.g., 0371 or 2400
                firstDataLine = false;
            }

            // Extract date and time
            std::string datetime = columns[2];
            std::string date, time_utc;
            size_t tPos = datetime.find('T');
            if (tPos != std::string::npos) {
                date = datetime.substr(0, tPos);                    // e.g., 2025-03-25
                time_utc = datetime.substr(tPos + 1, datetime.length() - tPos - 2); // e.g., 14:23:31
            }

            // Extract numeric fields
            int cpm = std::stoi(columns[3]);
            int pulses5s = std::stoi(columns[4]);
            int pulses_total = std::stoi(columns[5]);
            std::string gps_validity = columns[6];
            double altitude = std::stod(columns[11]);
            int sat = std::stoi(columns[13]);
            int hdop = std::stoi(columns[14].substr(0, columns[14].find('*'))); // Remove checksum

            // Calculate ADER_microSvh
            double ader = (device == "Safecast") ? 
                pulses5s * 0.0359281437125749:
                pulses5s * 0.0365296803652968;
            // for Safecast ADER_microSvh = (CP5s * 12) * 0.0029940119760479 = CP5s * 0.0359281437125749
            // for CzechRad ADER_microSvh = (CP5s * 12) * 0.0030441400304414 = CP5s * 0.0365296803652968

            // Convert latitude and longitude
            double latitude = convertToDecimalDegrees(columns[7], columns[8][0]);
            double longitude = convertToDecimalDegrees(columns[9], columns[10][0]);

            // Write to CSV
            outFile << std::fixed << std::setprecision(6);
            outFile << device << ","
                    << device_type << ","
                    << device_id << ","
                    << date << ","
                    << time_utc << ","
                    << cpm << ","
                    << ader << ","
                    << latitude << ","
                    << longitude << ","
                    << altitude << ","
                    << pulses5s << ","
                    << pulses_total << ","
                    << gps_validity << ","
                    << sat << ","
                    << hdop << "\n";
        }
    }

    inFile.close();
    outFile.close();
    return true;
}

int main() {
    // Define input and output directories
    fs::path inputDir = "input";
    fs::path outputDir = "output";

    // Create directories if they don't exist
    try {
        fs::create_directories(inputDir);
        fs::create_directories(outputDir);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error creating directories: " << e.what() << std::endl;
        return 1;
    }

    // Collect all *.LOG and *.log files recursively
    std::vector<fs::path> logFiles;
    try {
        for (const auto& entry : fs::recursive_directory_iterator(inputDir)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                // Convert extension to lowercase for case-insensitive comparison
                std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                if (ext == ".log") {
                    logFiles.push_back(entry.path());
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error scanning input directory: " << e.what() << std::endl;
        return 1;
    }

    // Process each LOG file
    for (const auto& logPath : logFiles) {
        std::cout << "Processing: " << logPath << std::endl;
        if (processLogFile(logPath, outputDir)) {
            // Move the LOG file to output directory
            fs::path destPath = outputDir / logPath.filename();
            try {
                fs::rename(logPath, destPath);
                std::cout << "Moved to: " << destPath << std::endl;
            } catch (const fs::filesystem_error& e) {
                std::cerr << "Error moving file " << logPath << ": " << e.what() << std::endl;
            }
        } else {
            std::cerr << "Failed to process: " << logPath << std::endl;
        }
    }

    // Clear input directory
    try {
        for (const auto& entry : fs::directory_iterator(inputDir)) {
            fs::remove_all(entry.path());
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error clearing input directory: " << e.what() << std::endl;
        return 1;
    }

    // Report completion
    std::cout << "\nJob finished. All LOG files processed and moved to output folder." << std::endl;
    std::cout << "Press Enter to close..." << std::endl;
    std::cin.get();

    return 0;
}

// Created with Grok AI, released under MIT No Attribution License
// Copyright 2025 Jan Helebrant, czechrad@suro.cz, www.suro.cz

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so.

// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

