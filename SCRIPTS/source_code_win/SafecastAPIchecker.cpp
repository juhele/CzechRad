#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <windows.h>
#include <winhttp.h>
#include <cstdlib> // For system("pause")
#include "json.hpp"
using json = nlohmann::json;

// Convert LPWSTR to std::string
std::string WideToNarrow(LPCWSTR wideStr) {
    if (!wideStr) return "";
    int size = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, nullptr, 0, nullptr, nullptr);
    std::string result(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, &result[0], size, nullptr, nullptr);
    return result.substr(0, result.size() - 1); // Remove null terminator
}

// Convert std::string to LPWSTR
std::wstring NarrowToWide(const std::string& str) {
    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    std::wstring result(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &result[0], size);
    return result.substr(0, result.size() - 1); // Remove null terminator
}

// Function to split string by delimiter
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to get current timestamp in YYYY-MM-DD_HH-MM-SS format
std::string getTimestamp() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", ltm);
    return std::string(buffer);
}

// Function to log message to file and console
void logMessage(std::ofstream& logFile, const std::string& message) {
    std::cout << message << std::endl;
    logFile << message << std::endl;
}

// Function to replace commas with semicolons in a string
std::string replaceCommasWithSemicolons(const std::string& input) {
    std::string result = input;
    std::replace(result.begin(), result.end(), ',', ';');
    return result;
}

// Function to fetch JSON from URL using WinHTTP
std::string fetchJson(const std::string& urlStr, std::ofstream& logFile) {
    std::string response;
    DWORD error = 0;

    logMessage(logFile, getTimestamp() + ": Fetching URL: " + urlStr);

    // Extract host and path
    std::wstring wideUrl = NarrowToWide(urlStr);
    std::string host = "api.safecast.org";
    std::wstring wideHost = NarrowToWide(host);
    std::string pathNarrow = urlStr.substr(urlStr.find("api.safecast.org") + 17);
    std::wstring path = NarrowToWide(pathNarrow);
    if (path.empty()) path = L"/";

    logMessage(logFile, getTimestamp() + ": Host: " + host + ", Path: " + pathNarrow + ", Port: 443, Scheme: HTTPS");

    HINTERNET hSession = WinHttpOpen(L"SAFECAST_API_checker/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, NULL, NULL, 0);
    if (!hSession) {
        error = GetLastError();
        logMessage(logFile, getTimestamp() + ": WinHttpOpen failed with error " + std::to_string(error));
        return "";
    }
    WinHttpSetTimeouts(hSession, 10000, 10000, 10000, 10000);

    HINTERNET hConnect = WinHttpConnect(hSession, wideHost.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) {
        error = GetLastError();
        logMessage(logFile, getTimestamp() + ": WinHttpConnect failed with error " + std::to_string(error));
        WinHttpCloseHandle(hSession);
        return "";
    }

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", path.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    if (!hRequest) {
        error = GetLastError();
        logMessage(logFile, getTimestamp() + ": WinHttpOpenRequest failed with error " + std::to_string(error));
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return "";
    }

    DWORD flags = SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID | SECURITY_FLAG_IGNORE_CERT_CN_INVALID | SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE;
    WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &flags, sizeof(flags));

    if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        error = GetLastError();
        logMessage(logFile, getTimestamp() + ": WinHttpSendRequest failed with error " + std::to_string(error));
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return "";
    }

    if (!WinHttpReceiveResponse(hRequest, NULL)) {
        error = GetLastError();
        logMessage(logFile, getTimestamp() + ": WinHttpReceiveResponse failed with error " + std::to_string(error));
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return "";
    }

    DWORD dwStatusCode = 0;
    DWORD dwSize = sizeof(dwStatusCode);
    WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, NULL, &dwStatusCode, &dwSize, NULL);

    if (dwStatusCode != HTTP_STATUS_OK) {
        std::string errMsg = getTimestamp() + ": HTTP ERROR " + std::to_string(dwStatusCode);
        logMessage(logFile, errMsg);
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return "";
    }

    DWORD dwBytesAvailable;
    do {
        dwBytesAvailable = 0;
        if (!WinHttpQueryDataAvailable(hRequest, &dwBytesAvailable)) {
            error = GetLastError();
            logMessage(logFile, getTimestamp() + ": WinHttpQueryDataAvailable failed with error " + std::to_string(error));
            break;
        }

        if (dwBytesAvailable == 0) break;

        char* pszOutBuffer = new char[dwBytesAvailable + 1];
        if (!pszOutBuffer) {
            logMessage(logFile, getTimestamp() + ": Memory allocation failed");
            break;
        }

        DWORD dwBytesRead = 0;
        if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwBytesAvailable, &dwBytesRead)) {
            error = GetLastError();
            logMessage(logFile, getTimestamp() + ": WinHttpReadData failed with error " + std::to_string(error));
            delete[] pszOutBuffer;
            break;
        }

        pszOutBuffer[dwBytesRead] = '\0';
        response += std::string(pszOutBuffer);
        delete[] pszOutBuffer;

    } while (dwBytesAvailable > 0);

    logMessage(logFile, getTimestamp() + ": Response received: " + response.substr(0, 100) + (response.length() > 100 ? "..." : ""));

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return response;
}

int main() {
    std::string timestamp = getTimestamp();

    std::string outputCsvName = "output/" + timestamp + "_Safecast_API_check.csv";
    std::string outputLogName = "output/" + timestamp + "_Safecast_API_check_debug_log.txt";

    std::ofstream outputCsv(outputCsvName);
    std::ofstream logFile(outputLogName);

    if (!outputCsv.is_open() || !logFile.is_open()) {
        std::cerr << "Failed to open output files." << std::endl;
        return 1;
    }

    logMessage(logFile, timestamp + ": application started");

    std::ifstream inputFile("input/safecast_API_devices.csv");
    if (!inputFile.is_open()) {
        logMessage(logFile, timestamp + ": Failed to open input file");
        outputCsv.close();
        logFile.close();
        return 1;
    }

    std::string line;
    std::vector<std::string> lines;

    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();

    int numLines = lines.size() - 1;  // Exclude header
    logMessage(logFile, timestamp + ": number of input lines is " + std::to_string(numLines));

    // Write header to output CSV
    outputCsv << "device,device_number,project,comment,last_data_uploaded,measured_by,data_info,api_link" << std::endl;

    // Process each line except header
    for (size_t i = 1; i < lines.size(); ++i) {
        auto fields = split(lines[i], ','); // Use comma as delimiter
        if (fields.size() < 5) {
            logMessage(logFile, timestamp + ": Invalid CSV line " + std::to_string(i + 1) + ": " + lines[i]);
            continue;  // Invalid line
        }

        std::string device = fields[0];
        std::string device_number = fields[1];
        std::string project = fields[2];
        std::string comment = fields[3];
        std::string api_link = fields[4]; // Use the full URL from the CSV

        std::string jsonStr = fetchJson(api_link, logFile);
        if (jsonStr.empty()) {
            logMessage(logFile, timestamp + ": No response for " + api_link);
            outputCsv << device << "," << device_number << "," << project << "," << comment << ",NO DATA,NO DATA,NO DATA," << api_link << std::endl;
            continue;
        }

        std::string last_data_uploaded = "NO DATA";
        std::string measured_by = "NO DATA";
        std::string data_info = "NO DATA";

        try {
            json j = json::parse(jsonStr);
            if (j.is_array() && !j.empty()) {
                json first = j[0];
                if (first.contains("created_at")) last_data_uploaded = first["created_at"].get<std::string>();
                if (first.contains("credits")) measured_by = first["credits"].get<std::string>();
                if (first.contains("name")) data_info = first["name"].get<std::string>();
            } else {
                logMessage(logFile, timestamp + ": JSON response is empty or not an array for " + api_link);
            }
        } catch (const json::exception& e) {
            logMessage(logFile, timestamp + ": JSON parse error for " + api_link + ": " + e.what());
            outputCsv << device << "," << device_number << "," << project << "," << comment << ",NO DATA,NO DATA,NO DATA," << api_link << std::endl;
            continue;
        }

        // Replace commas with semicolons in JSON-derived fields
        last_data_uploaded = replaceCommasWithSemicolons(last_data_uploaded);
        measured_by = replaceCommasWithSemicolons(measured_by);
        data_info = replaceCommasWithSemicolons(data_info);

        outputCsv << device << "," << device_number << "," << project << "," << comment << ","
                  << last_data_uploaded << "," << measured_by << "," << data_info << "," << api_link << std::endl;
    }

    logMessage(logFile, timestamp + ": processing finished");

    outputCsv.close();
    logFile.close();

    std::cout << "Processing finished, press Enter to close the application" << std::endl;
    std::cin.get();
    system("pause");

    return 0;
}

// Created with Grok AI, released under MIT No Attribution License
// Copyright 2025 Jan Helebrant, czechrad@suro.cz, www.suro.cz

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so.

// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.