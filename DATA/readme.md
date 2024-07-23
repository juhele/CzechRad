### Radiation data sentence

This is the basic message containing the geo-located radiation measurement.

Example:

    $CZRA1,0159,2024-04-21T05:16:31Z,42,7,32804,A,5001.1094,N,01420.7866,E,384.69,A,5,193*1C
    $CZRA1,0159,2024-04-21T05:16:36Z,42,2,32806,A,5001.0757,N,01420.7396,E,389.48,A,5,193*1F
    $CZRA1,0159,2024-04-21T05:16:41Z,41,3,32809,A,5001.0371,N,01420.6880,E,391.23,A,6,164*10
    $CZRA1,0159,2024-04-21T05:16:46Z,43,2,32811,A,5001.0000,N,01420.6353,E,391.02,A,6,164*1E
    $CZRA1,0159,2024-04-21T05:16:51Z,42,3,32814,A,5000.9668,N,01420.5859,E,390.56,A,6,164*1F

0. Header : BNXRDD
1. Device ID : Device serial number. `300`
2. Date : Date formatted according to iso-8601 standard. Usually uses GMT. `2012-12-16T17:58:31Z`
3. Radiation 1 minute : number of pulses given by the Geiger tube in the last minute. `30`
4. Radiation 5 seconds : number of pulses given by the Geiger tube in the last 5 seconds. `1`
5. Radiation total count : total number of pulses recorded since startup. `116`
6. Radiation count validity flag : 'A' indicates the counter has been running for more than one minute and the 1 minute count is not zero. Otherwise, the flag is 'V' (void). `A`
7. Latitude : As given by GPS. The format is `ddmm.mmmm` where `dd` is in degrees and `mm.mmmm` is decimal minute. `4618.9612`
8. Hemisphere : 'N' (north), or 'S' (south). `N`
9. Longitude : As given by GPS. The format is `dddmm.mmmm` where `ddd` is in degrees and `mm.mmmm` is decimal minute. `00658.4831`
10. East/West : 'W' (west) or 'E' (east) from Greenwich. `E`
11. Altitude : Above sea level as given by GPS in meters. `443.7`
12. GPS validity : 'A' ok, 'V' invalid. `A`
13. HDOP : Horizontal Dilution of Precision (HDOP), relative accuracy of horizontal position. `1.28`
14. Fix Quality : 0 = invalid, 1 = GPS Fix, 2 = DGPS Fix. `1`
15. Checksum. `*1D`

The text above is based on [Safecast bGeigie Library](https://github.com/Safecast/bGeigieMini/blob/master/README.md) documentation.
