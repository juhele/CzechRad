# CzechRad/Safecast utilities source code

This folder contains source code for all CzechRad/Safecast related utilities and all things needed for compilation.
Requires mingw-w64 - in Ubuntu / Debian based linux distributions use this command:

`sudo apt-get install mingw-w64`

to install it. 
Compilation was performed in current Kubuntu LTS, compilation commands creating a static Windows binary are provided below.

Compilation Instructions:

Use a C++17-compliant compiler (e.g., MSVC in Visual Studio).

Link against the Windows API (included by default in Windows environments).

Ensure the C++ standard library supports <filesystem> (C++17 or later).

## CzechRadStats:

- run in terminal:

`x86_64-w64-mingw32-g++ -std=c++17 -I fmt/include CzechRadStats.cpp -o CzechRadStats.exe -static`

## CzechRad-LOG2CSV-converter:

- run in terminal:

`x86_64-w64-mingw32-g++ -std=c++17 -I fmt/include CzechRad-LOG2CSV-converter.cpp -o CzechRad-LOG2CSV-converter.exe -static`


## CzechRad Log Patcher:

- run in terminal:

`x86_64-w64-mingw32-g++ CzechRadLogPatcherPortable.cpp -o CzechRadLogPatcher.exe -std=c++17 -static -static-libgcc -static-libstdc++`


## SAFECAST API checker for Windows:

This is a C++ application that checks the Safecast API for device data, processes input from a CSV file, and generates an output CSV with details like last data upload time, credits, and data info. It uses the WinHTTP library to fetch JSON data and the nlohmann/json library for parsing.

### Prerequisites
- **Operating System**: Windows (tested on Windows 10/11).
- **Compiler**: Visual Studio 2022 with the C++ development workload installed.
- **Windows SDK**: Included with Visual Studio 2022, providing `winhttp.lib`, `ws2_32.lib`, and `kernel32.lib`.
- **nlohmann/json Library**: The `json.hpp` file (included in this repository or downloadable from https://github.com/nlohmann/json).
- **Internet Connection**: Required to fetch data from `api.safecast.org`.

### Files Included
- `SafecastAPIchecker.cpp`: The main source code file.
- `json.hpp`: The nlohmann/json library header for JSON parsing.
- `input/safecast_API_devices.csv` (optional): Sample input CSV file for testing.

### Setup Instructions
1. **Install Visual Studio 2022**:
   - Download and install Visual Studio 2022 (Community edition is free) from https://visualstudio.microsoft.com/.
   - Ensure the "Desktop development with C++" workload is selected during installation.

2. **Prepare the Files**:
   - Place `SafecastAPIchecker.cpp` and `json.hpp` in the same directory (e.g., `C:\Projects\SAFECAST_API_checker`).
   - If testing, create an `input/` directory in the same location and place `safecast_API_devices.csv` inside it. The CSV should have the format:

- run in terminal:

`x86_64-w64-mingw32-g++ CzechRadLogPatcherPortable.cpp -o CzechRadLogPatcher.exe -std=c++17 -static -static-libgcc -static-libstdc++`
