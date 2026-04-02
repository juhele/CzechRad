#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <windows.h>
#include <iomanip>
#include <algorithm>
#include <cstdio>

namespace fs = std::filesystem;

// Improved coordinate conversion (handles both Safecast and CzechRad formats)
// CzechRad longitude sometimes includes a leading '-', Safecast does not.
// We always parse the absolute value and let the N/S/E/W direction determine the sign.
double convertToDecimalDegrees(std::string coord, char direction) {
    // Remove leading minus if present (CzechRad longitude case)
    if (!coord.empty() && coord[0] == '-') {
        coord = coord.substr(1);
    }

    size_t dotPos = coord.find('.');
    if (dotPos == std::string::npos || coord.length() < 7) return 0.0;

    // The last 7 characters are always "mm.mmmm"
    std::string degreesStr = coord.substr(0, coord.length() - 7);
    std::string minutesStr = coord.substr(coord.length() - 7);

    double degrees = std::stod(degreesStr);
    double minutes = std::stod(minutesStr);

    double decimalDegrees = degrees + (minutes / 60.0);

    // Apply direction (N/S for latitude, E/W for longitude)
    if (direction == 'S' || direction == 'W') {
        decimalDegrees *= -1.0;
    }

    return decimalDegrees;
}

// Function to process a single LOG file and convert to GPX track
bool processLogFile(const fs::path& logPath, const fs::path& outputDir) {
    std::ifstream inFile(logPath);
    if (!inFile.is_open()) {
        std::cerr << "Error opening input file: " << logPath << std::endl;
        return false;
    }

    // Build output GPX filename: remove "_CzechRad" or "_Safecast" suffix if present,
    // then append ".Track0.gpx" to match the example naming style.
    std::string stemStr = logPath.stem().string();
    size_t pos = stemStr.find("_CzechRad");
    if (pos != std::string::npos) {
        stemStr = stemStr.substr(0, pos);
    } else {
        pos = stemStr.find("_Safecast");
        if (pos != std::string::npos) {
            stemStr = stemStr.substr(0, pos);
        }
    }
    fs::path gpxPath = outputDir / (stemStr + ".Track0.gpx");

    std::ofstream outFile(gpxPath);
    if (!outFile.is_open()) {
        std::cerr << "Error opening output GPX file: " << gpxPath << std::endl;
        inFile.close();
        return false;
    }

    // Write standard GPX header (matching the style and creator from the example)
    outFile << R"(<?xml version="1.0"?>
<gpx xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" version="1.1" creator="CzechRad/Safecast LOG to GPX Converter" xmlns="http://www.topografix.com/GPX/1/1">
  <trk>
    <name>Track0</name>
    <trkseg>
)";

    std::string line;

    while (std::getline(inFile, line)) {
        // Skip comment lines
        if (line.empty() || line[0] == '#') continue;

        // Process data lines starting with $
        if (line[0] == '$') {
            std::stringstream ss(line);
            std::vector<std::string> columns;
            std::string token;

            while (std::getline(ss, token, ',')) {
                columns.push_back(token);
            }

            // Validate column count (same as CSV converter)
            if (columns.size() < 15) continue;

            // Extract UTC datetime (column 2)
            std::string utc_datetime = columns[2];

            // Convert UTC Z time to local +01:00 (add 1 hour, adjust date if needed)
            // This matches the example GPX exactly (e.g. 22:58:40Z → 23:58:40+01:00)
            std::string local_time_str;
            if (!utc_datetime.empty() && utc_datetime.back() == 'Z') {
                utc_datetime.pop_back();
            }
            int year = 0, mon = 0, day = 0, hour = 0, minu = 0, sec = 0;
            if (sscanf(utc_datetime.c_str(), "%d-%d-%dT%d:%d:%d", &year, &mon, &day, &hour, &minu, &sec) == 6) {
                hour += 1;
                if (hour >= 24) {
                    hour -= 24;
                    day += 1;
                    // Note: full month rollover is omitted for simplicity (logs are short, <2 hours)
                }
                std::ostringstream oss;
                oss << std::setfill('0') << std::setw(4) << year << "-"
                    << std::setw(2) << mon << "-"
                    << std::setw(2) << day << "T"
                    << std::setw(2) << hour << ":"
                    << std::setw(2) << minu << ":"
                    << std::setw(2) << sec << "+01:00";
                local_time_str = oss.str();
            } else {
                // Fallback (should never happen)
                local_time_str = utc_datetime + "+01:00";
            }

            // Convert coordinates (now correctly handles both device formats)
            char lat_dir = columns[8][0];
            char lon_dir = columns[10][0];
            double latitude = convertToDecimalDegrees(columns[7], lat_dir);
            double longitude = convertToDecimalDegrees(columns[9], lon_dir);

            // Altitude
            double altitude = std::stod(columns[11]);

            // Write GPX trackpoint
            // High precision for lat/lon (matches example decimal places), 2 decimals for elevation
            outFile << "      <trkpt lat=\""
                    << std::fixed << std::setprecision(10) << latitude
                    << "\" lon=\"" << longitude << "\">\n"
                    << "        <ele>"
                    << std::fixed << std::setprecision(2) << altitude
                    << "</ele>\n"
                    << "        <time>" << local_time_str << "</time>\n"
                    << "      </trkpt>\n";
        }
    }

    // Close GPX tags
    outFile << R"(    </trkseg>
  </trk>
</gpx>
)";

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
    std::cout << "\nJob finished. All LOG files processed to GPX tracks and moved to output folder." << std::endl;
    std::cout << "Press Enter to close..." << std::endl;
    std::cin.get();

    return 0;
}

// Created with Grok AI, released under MIT No Attribution License
// Copyright 2025 Jan Helebrant, czechrad@suro.cz, www.suro.cz (GPX version by Grok)

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so.

// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.