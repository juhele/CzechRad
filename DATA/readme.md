### Radiation data sentence

This is the basic message containing the geo-located radiation measurement.

Example:

    $BNXRDD,300,2012-12-16T17:58:24Z,31,9,115,A,4618.9996,N,00658.4623,E,587.6,A,77.2,1*1A
    $BNXRDD,300,2012-12-16T17:58:31Z,30,1,116,A,4618.9612,N,00658.4831,E,443.7,A,1.28,1*1D
    $BNXRDD,300,2012-12-16T17:58:36Z,32,4,120,A,4618.9424,N,00658.4802,E,428.1,A,1.27,1*18
    $BNXRDD,300,2012-12-16T17:58:41Z,32,2,122,A,4618.9315,N,00658.4670,E,425.5,A,1.27,1*1B
    $BNXRDD,300,2012-12-16T17:58:46Z,34,3,125,A,4618.9289,N,00658.4482,E,426.0,A,1.34,1*13

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
