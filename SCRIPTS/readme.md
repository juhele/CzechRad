# CzechRad scripts

Here I provide scripts for working with CzechRad data. I am not a programmer, but I can do fine tuning / debugging of the scripts if AI writes them for me first. If it helps you, feel free to use the scripts published here. I cannot provide technical support for these scripts or modify them at your request. If you make any useful modifications to the script, or write a new script, please consider sharing it with other CzechRad users here. Thank you

## Supported Platforms

**GNU/Linux**

After several negative experiences on Windows, we are creating these scripts primarily for GNU/Linux. It works in Linux, the scripts can be easily deployed on a server, and anyone can set up a Linux environment on their computer for free, for example using [ORACLE Virtualbox](https://www.virtualbox.org/wiki/Downloads) - including institutions/companies. For a lightweight installation, we recommend [current LTS version of Lubuntu Linux](https://lubuntu.me/downloads/).

Tested on current LTS (K)Ubuntu but should work on Debian Linux without any problems. Scripts use standard tools like "grep", "sed" etc.

**Microsoft Windows 64bit**

However, recently we also managed to create a working Windows versions of some of the scripts, tested on Windows 10 and 11. The exe files are portable, do not require admin rights or external librarires.

**Use:**

Download package in ZIP file (click the link and then the Download raw file icon):

<img src="_images/download_raw.png" alt="Download raw file icon">

Unpack to your computer. In GNU/Linux environment copy the CzechRad or Safecast *.LOG data you wan to process to "input" folder. In GNU/Linux environment run terminal window from the script folder (can be usually done from default file manager) and then just drag and drop the *.sh file in your terminal window and press Enter.

## Script 01 - find and copy all *.LOG files measured in particular year from "input" to "output" folder

This script scans "input" directory for *.LOG files and copies those containing "2024" inside to "output" folder. Feel free to replace "2024" with other year according to your needs. Under Windows, I recommend using more advanced text editors like [PSPad](https://www.pspad.com/en/), [Notepad++](https://notepad-plus-plus.org/) for script editing and not using default Notepad in Windows - just to be safe. 

**Download:** [CzechRad_script_001_find_2024_data.zip (2.63 KB)](https://github.com/juhele/CzechRad/blob/main/SCRIPTS/CzechRad_script_001_find_2024_data.zip)

## Script 02 - sort the *.LOG files from "input" into subfolders (in "output") according to the year and month of measurement

This script scans "input" directory for *.LOG files and sorts them into subfolders (in "output") according to the year and month of measurement. Under Windows, I recommend using more advanced text editors like [PSPad](https://www.pspad.com/en/), [Notepad++](https://notepad-plus-plus.org/) for script editing and not using default Notepad in Windows - just to be safe. 

**Download:** [CzechRad_script_002_sort_into_folders.zip (2.97 KB)](https://github.com/juhele/CzechRad/blob/main/SCRIPTS/CzechRad_script_002_sort_into_folders.zip)

## Script 03 - calculate statistics for *.LOG files in a folder

This script scans "input" directory for *.LOG files and calculates a summary statistics file in a comma separated CSV file you can easily load in spreadsheet software like Microsoft Excel or LibreOffice Calc. The result looks like:

```
file,device_type,device_ID,date,start_time_UTC,points,duration,ADER_max
04860412_l.LOG,CZRDD,0486,2025-04-12,09:30:10,2461,03:25:05,0.5479
04860411_l.LOG,CZRDD,0486,2025-04-11,14:03:26,333,00:27:45,0.4018
04860413_l.LOG,CZRDD,0486,2025-04-13,07:33:32,784,01:05:20,0.3653
```
ADER_max is calculated from the number of pulses from five-second measurements. The ADER values ​​calculated in this way fluctuate greatly and therefore the maximum values ​​may be higher.

Under Windows, I recommend using more advanced text editors like [PSPad](https://www.pspad.com/en/), [Notepad++](https://notepad-plus-plus.org/) for script editing and not using default Notepad in Windows - just to be safe. 

**Download:** 

**Linux bash:**
[CzechRad_script_003_log_statistics_1_linux_bash.zip (3.77 KB)](https://github.com/juhele/CzechRad/blob/main/SCRIPTS/CzechRad_script_003_log_statistics_1_linux_bash.zip)

**MS Windows - EXE application:**
[CzechRad_script_003_log_statistics_2_Windows_x86_EXE.zip (832.0 KB)](https://github.com/juhele/CzechRad/blob/main/SCRIPTS/CzechRad_script_003_log_statistics_2_Windows_x86_EXE.zip)

## Script 04 - LOG2CSV-converter for Windows

This application scans "input" directory for *.LOG files and converts every LOG file in a comma separated CSV file you can easily load in spreadsheet software like Microsoft Excel or LibreOffice Calc. The app is able to distinguish data from CzechRad and Safecast devices based on device strings in the LOG file and calculate the dose rate (ADER) values using appropriate conversion factor.

Input:

Safecast sample:

```
$BNRDD,2408,2022-03-20T15:30:42Z,34,4,42,A,5003.1563,N,01430.9260,E,242.80,A,4,210*4B
$BNRDD,2408,2022-03-20T15:30:47Z,32,2,44,A,5003.1541,N,01430.9260,E,243.70,A,4,210*46
$BNRDD,2408,2022-03-20T15:30:52Z,31,0,44,A,5003.1506,N,01430.9265,E,244.00,A,4,210*45
```

CzechRad sample:

```
$CZRA1,0007,2025-01-25T15:13:18Z,26,3,867,A,5003.2222,N,01430.9257,E,284.32,A,5,826*1a
$CZRA1,0007,2025-01-25T15:13:23Z,25,3,870,A,5003.2241,N,01430.9308,E,290.94,A,5,829*1f
$CZRA1,0007,2025-01-25T15:13:28Z,28,4,874,A,5003.2266,N,01430.9340,E,295.45,A,5,832*10
```

Output:

Safecast output sample:

```
device,device_type,device_ID,date,time_UTC,CPM,ADER_microSvh,latitude,longitude,altitude,pulses5s,pulses_total,GPS_validity,Sat,HDOP
Safecast,BNRDD,2408,2022-03-20,15:30:42,34,0.0120,50.052605,14.515433,242.80,4,42,A,4,210
Safecast,BNRDD,2408,2022-03-20,15:30:47,32,0.0060,50.052568,14.515433,243.70,2,44,A,4,210
Safecast,BNRDD,2408,2022-03-20,15:30:52,31,0.0000,50.052510,14.515442,244.00,0,44,A,4,210
```

CzechRad output sample:

```
device,device_type,device_ID,date,time_UTC,CPM,ADER_microSvh,latitude,longitude,altitude,pulses5s,pulses_total,GPS_validity,Sat,HDOP
CzechRad,CZRA1,0007,2025-01-25,15:13:18,26,0.0091,50.053703,14.515428,284.32,3,867,A,5,826
CzechRad,CZRA1,0007,2025-01-25,15:13:23,25,0.0091,50.053735,14.515513,290.94,3,870,A,5,829
CzechRad,CZRA1,0007,2025-01-25,15:13:28,28,0.0122,50.053777,14.515567,295.45,4,874,A,5,832
```

**Download:** 

**MS Windows - EXE application:**
[CzechRad_script_004_LOG2CSV-converter_Windows_x86_EXE.zip (680.0 KB)](https://github.com/juhele/CzechRad/blob/main/SCRIPTS/CzechRad_script_004_LOG2CSV-converter_Windows_x86_EXE.zip)


## Script 05 - Czechrad LOG Patcher for Windows

This application scans "input" directory for *.LOG files and converts every LOG file to a "legacy" LOG file supported by the SAFECAST API. It also corrects some bugs if you have older firmware - incorrect negative GPS coordinates etc.

The application will:

- Create a backup folder with timestamped backup of all *.LOG files
- Process *.LOG and *.log files recursively in the "input" folder
- Replace "CZRA1" with "CZRDD" and remove incorrectly used minus signs in GPS coordinates
- Recalculate checksums for data lines
- Move processed files to "output" folder
- Clear the "input" folder
- Prompt user to press Enter to exit

Usage:

just put your LOG files in "input" folder and run the EXE.

**Download:** 

**MS Windows - EXE application:**
[CzechRad_script_005_CzechRad_LOG_Patcher_Windows_x86_EXE.zip (673.0 KB)](https://github.com/juhele/CzechRad/blob/main/SCRIPTS/CzechRad_script_005_CzechRad_LOG_Patcher_Windows_x86_EXE.zip)

## Script 06 - SAFECAST API checker for Windows

This application is tailor-made for organizations that operate a large number of Safecast / CzechRad devices and provides the ability to check the status of recorded data without the person / team processing and recording data to the Safecast API having to provide this information.

The application reads the "safecast_API_devices.csv" file from "input" directory (provided CSV sample file can be easily edited / updated using spreadsheet software like Microsoft Excel or LibreOffice Calc). The structure of the file is following (it is a "searching" link to api.safecast.org to list results from the database according to the selected filter):


```
device,device_number,project,comment,api_link
Safecast,2353,project name here,some other text here,https://api.safecast.org/en-US/bgeigie_imports?by_user_id=671&format=json&order=created_at+desc&q=2353
CzechRad,0292,CITISTRA,test,https://api.safecast.org/en-US/bgeigie_imports?by_user_id=6776&format=json&q=0292&status=all
```

The app then, line by line, queries the SAFECAST database and processes the provided JSON. The goal is to find out when the last data was uploaded to the API from a given device - i.e. whether to extend the loan to the user of the loaned device or, conversely, ask him to return it because he/she is not providing measured data. CSV output looks like this:

```
device,device_number,project,comment,last_data_uploaded,measured_by,data_info,api_link
Safecast,2353,project name here,some other text here,2024-10-08T09:49:41.760Z,Roman Slavík (for SURO.cz) ,Brno, Ivanovice na Hané - car,https://api.safecast.org/en-US/bgeigie_imports?by_user_id=671&format=json&order=created_at+desc&q=2353
CzechRad,0292,CITISTRA,test,2025-08-19T10:27:44.241Z,Lenka Benešová (for SURO.cz), #CITISTRA #CZ,Třebotovice, Třeboň, Nová Bystřice - car,https://api.safecast.org/en-US/bgeigie_imports?by_user_id=6776&format=json&q=0292&status=all
```

and there is also a debug log like:

```
2025-09-02_22-28-57: application started
2025-09-02_22-28-57: number of input lines is 2
2025-09-02_22-28-57: Fetching URL: https://api.safecast.org/en-US/bgeigie_imports?by_user_id=671&format=json&order=created_at+desc&q=2353
2025-09-02_22-28-57: Host: api.safecast.org, Path: en-US/bgeigie_imports?by_user_id=671&format=json&order=created_at+desc&q=2353, Port: 443, Scheme: HTTPS
2025-09-02_22-28-59: Response received: [{"id":65106,"user_id":671,"source":{"url":"https://safecastapi-imports-production-us-west-2.s3.amaz...
2025-09-02_22-28-59: Fetching URL: https://api.safecast.org/en-US/bgeigie_imports?by_user_id=6776&format=json&q=0292&status=all
2025-09-02_22-28-59: Host: api.safecast.org, Path: en-US/bgeigie_imports?by_user_id=6776&format=json&q=0292&status=all, Port: 443, Scheme: HTTPS
2025-09-02_22-28-59: Response received: [{"id":68870,"user_id":6776,"source":{"url":"https://safecastapi-imports-production-us-west-2.s3.ama...
2025-09-02_22-28-57: processing finished
```

**Download:** 

**MS Windows - EXE application:**
[CzechRad_script_006_SAFECAST_API_checker_Windows_x86_EXE (187.4 KB)](https://github.com/juhele/CzechRad/blob/main/SCRIPTS/CzechRad_script_006_SAFECAST_API_checker_Windows_x86_EXE.zip)

## Script 07 - CzechRad LOG2GPX converter for Windows

The application processes CzechRad/Safecast LOG data from the "input" folder and converts it to a standard GPX ([GPS Exchange Format](https://en.wikipedia.org/wiki/GPS_Exchange_Format)) file. 

This will allow you to use measured data from CzechRad/Safecast for geotagging photos from your camera or smartphone (for example, if you forgot to turn on inserting GPS location into Exif). The application is provided as an EXE for Windows, but also works via WINE on Linux.

**Download:** 

**MS Windows - EXE application:**
[CzechRad_script_006_SAFECAST_API_checker_Windows_x86_EXE (187.4 KB)](https://github.com/juhele/CzechRad/blob/main/SCRIPTS/CzechRad_script_006_SAFECAST_API_checker_Windows_x86_EXE.zip)

## License:

Code created with Grok AI, released under MIT No Attribution License
Copyright 2025 Jan Helebrant, czechrad@suro.cz, www.suro.cz

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
