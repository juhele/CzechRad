#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cmath>

namespace fs = std::filesystem;

// Function to convert seconds to HH:MM:SS format
std::string seconds_to_hhmmss(int total_seconds) {
    int hours = total_seconds / 3600;
    int minutes = (total_seconds % 3600) / 60;
    int seconds = total_seconds % 60;
    return fmt::format("{:02d}:{:02d}:{:02d}", hours, minutes, seconds);
}

// Function to get current timestamp in YYYY-MM-DD_HH-MM format
std::string get_timestamp() {
    std::time_t now = std::time(nullptr);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M", std::localtime(&now));
    return buffer;
}

// Function to split a string by delimiter
std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    // Check if input and output directories exist
    if (!fs::exists("input")) {
        std::cerr << "Input directory does not exist." << std::endl;
        return 1;
    }
    if (!fs::exists("output")) {
        fs::create_directory("output");
    }

    // Create CSV file with timestamp
    std::string timestamp = get_timestamp();
    std::string csvfile = fmt::format("output/{}_CzechRad_stats.csv", timestamp);

    // Write CSV header
    std::ofstream csv(csvfile);
    if (!csv.is_open()) {
        std::cerr << "Failed to create CSV file: " << csvfile << std::endl;
        return 1;
    }
    csv << "file,device_type,device_ID,date,start_time_UTC,points,duration,ADER_max\n";

    // Process each *.LOG file in input directory
    bool all_processed = true;
    for (const auto& entry : fs::directory_iterator("input")) {
        if (entry.path().extension() != ".LOG") continue;

        std::string filename = entry.path().filename().string();
        std::cout << "Processing file: " << filename << std::endl;

        // Initialize variables
        std::string device_type, device_id, date, start_time_utc;
        int points = 0;
        int max_ader = 0;
        bool first_data_line = true;

        // Read file line by line
        std::ifstream file(entry.path());
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            continue;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] != '$') continue;

            points++;
            std::string data = line.substr(1); // Remove '$'
            auto fields = split(data, ',');

            // Process first data line
            if (first_data_line && fields.size() >= 3) {
                device_type = fields[0];
                device_id = fields[1];
                // Extract date and time from fields[2] (format: YYYY-MM-DDThh:mm:ssZ)
                size_t t_pos = fields[2].find('T');
                if (t_pos != std::string::npos) {
                    date = fields[2].substr(0, t_pos);
                    start_time_utc = fields[2].substr(t_pos + 1);
                    start_time_utc = start_time_utc.substr(0, start_time_utc.find('Z'));
                }
                first_data_line = false;
            }

            // Get 5th column (ADER value, integer)
            if (fields.size() >= 5 && !fields[4].empty()) {
                try {
                    int ader = std::stoi(fields[4]);
                    max_ader = std::max(max_ader, ader);
                } catch (...) {
                    // Skip invalid ADER values
                }
            }
        }
        file.close();

        // Calculate ADER_max (multiply max_ader by conversion factor)
        double ader_max = max_ader * 0.0365296803652968;
        std::string ader_max_formatted = fmt::format("{:.4f}", ader_max);

        // Calculate duration (points * 5 seconds)
        int duration_seconds = points * 5;
        std::string duration = seconds_to_hhmmss(duration_seconds);

        // Write to CSV
        csv << fmt::format("{},{},{},{},{},{},{},{}\n",
            filename, device_type, device_id, date, start_time_utc,
            points, duration, ader_max_formatted);

        // Move processed file to output
        fs::path dest = fs::path("output") / filename;
        try {
            fs::rename(entry.path(), dest);
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Failed to move file " << filename << ": " << e.what() << std::endl;
            all_processed = false;
        }
    }

    // Verify input directory is empty
    bool input_empty = true;
    for (const auto& entry : fs::directory_iterator("input")) {
        if (entry.is_regular_file()) {
            input_empty = false;
            break;
        }
    }

    if (input_empty && all_processed) {
        std::cout << "Job finished successfully. All *.LOG files processed and moved to output directory." << std::endl;
    } else {
        std::cerr << "Warning: Input directory is not empty after processing." << std::endl;
    }

    // Prompt user to press Enter to close the window
    std::cout << "Press Enter to close this window..." << std::endl;
    std::cin.get();

    csv.close();
    return 0;
}

// Created with Grok AI, released under MIT No Attribution License
// Copyright 2025 Jan Helebrant, czechrad@suro.cz, www.suro.cz

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so.

// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
