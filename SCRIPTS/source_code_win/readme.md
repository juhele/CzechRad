# CzechRad/Safecast utilities source code

This folder contains source code for all CzechRad/Safecast related utilities and all things needed for compilation.
Requires mingw-w64 - in Ubuntu / Debian based linux distributions use this command:

`sudo apt-get install mingw-w64`

to install it. 

Compilation was performed in current Kubuntu LTS, compilation commands creating a static Windows binary are provided below.

## CzechRadStats:

- run in terminal:

`x86_64-w64-mingw32-g++ -std=c++17 -I fmt/include CzechRadStats.cpp -o CzechRadStats.exe -static`
