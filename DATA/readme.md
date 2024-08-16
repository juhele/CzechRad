### CzechRad data structure

In order to be compatible with the [SAFECAST API](https://api.safecast.org) and the [Radiation Toolbox Plugin](https://opengeolabs.gitlab.io/qgis-radiation-toolbox-plugin/) for [QGIS](https://www.qgis.org/) the CzechRad device produces data files in the same *.LOG format as SAFECAST bGeigie Nano.
The file itself is just a plain text file with structured data.

Example:

    $CZRA1,0159,2024-04-21T05:16:31Z,42,7,32804,A,5001.1094,N,01420.7866,E,384.69,A,5,193*1C
    $CZRA1,0159,2024-04-21T05:16:36Z,42,2,32806,A,5001.0757,N,01420.7396,E,389.48,A,5,193*1F
    $CZRA1,0159,2024-04-21T05:16:41Z,41,3,32809,A,5001.0371,N,01420.6880,E,391.23,A,6,164*10
    $CZRA1,0159,2024-04-21T05:16:46Z,43,2,32811,A,5001.0000,N,01420.6353,E,391.02,A,6,164*1E
    $CZRA1,0159,2024-04-21T05:16:51Z,42,3,32814,A,5000.9668,N,01420.5859,E,390.56,A,6,164*1F

0. Header : CZRA1 (first units used CZRDD, current CzechRads use CZRA1, in future possibly others like CZRA2, CZRB1 etc.)
1. Device ID : Device serial number. `0159`
2. Date : Date formatted according to iso-8601 standard. Usually uses GMT/UTC. `2024-04-21T05:16:31Z`
3. Radiation 1 minute : number of pulses given by the Geiger tube in the last minute. `42`
4. Radiation 5 seconds : number of pulses given by the Geiger tube in the last 5 seconds. `7`
5. Radiation total count : total number of pulses recorded since startup. `32804`
6. Radiation count validity flag : 'A' indicates the counter has been running for more than one minute and the 1 minute count is not zero. Otherwise, the flag is 'V' (void). `A`
7. Latitude : As given by GPS. The format is `ddmm.mmmm` where `dd` is in degrees and `mm.mmmm` is decimal minute. `5001.1094`
8. Hemisphere : 'N' (north), or 'S' (south). `N`
9. Longitude : As given by GPS. The format is `dddmm.mmmm` where `ddd` is in degrees and `mm.mmmm` is decimal minute. `01420.7866`
10. East/West : 'W' (west) or 'E' (east) from Greenwich. `E`
11. Altitude : Above sea level as given by GPS in meters. `384.69`
12. GPS validity : 'A' ok, 'V' invalid. `A`
13. Number of satellites : 	the number of satellites used by the GPS `5`
14. HDOP : Horizontal Dilution of Precision (HDOP), relative accuracy of horizontal position. `193`
15. Checksum. `*1C`

The text above is based on [Safecast bGeigie Library](https://github.com/Safecast/bGeigieMini/blob/master/README.md) documentation.

### Dose rate calculation

The dose rate values in microSieverts per hour (μSv/h) are not a part of the LOG data files. The dose rate (more precisely "ambient dose equivalent rate" - ADER) values are calculated temporarily by the device (SAFECAST bGeigie Nano, CzechRad etc.) and then by other applications like [SAFECAST API](https://api.safecast.org) or [Radiation Toolbox Plugin](https://opengeolabs.gitlab.io/qgis-radiation-toolbox-plugin/) for [QGIS](https://www.qgis.org/). We use calculation from "pulse5s" values (number of pulses given by the Geiger tube in the last 5 seconds).

For Safecast data the 0.0029940119760479 (=1/334) is used following the official [Safecast github](https://github.com/Safecast/safecastapi/blob/7cbf30add95b9616f0a3d13087e9f9404f78279e/script/ios_query.sql#L14). CzechRad has slightly different callibration coefficient 0.0030441400304414 
(=1/328,5).

#### SAFECAST bGeigie Nano dose rate calculation

ADER = (pulse5s * 12) * 0.0029940119760479

#### CzechRad dose rate calculation

ADER = (pulse5s * 12) * 0.0030441400304414
