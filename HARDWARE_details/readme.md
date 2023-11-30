# CzechRad detector - technical specifications

## Comparison between Safecast bGeigie Nano and CzechRad

While Safecast bGeigie Nano mainly relies on the use of off-the-shelf commercial third-party modules...

<img src="/HARDWARE_details/images/schema_01_Safecast.jpg" alt="Safecast wiring diagram" width="800"/>

... CzechRad was designed to only rely on basic electronics components and uses own, custom-designed electronic boards

<img src="/HARDWARE_details/images/schema_02_CzechRad.jpg" alt="CzechRad wiring diagram" width="800"/>

*One board to rule them all? Not yet but close :-)*

- only two boards instead of several 3rd party made modules

## Main board
- OpenHardware design
- SAMD21G processor (ARM M0+)
- GPS module by uBlox
- built-in SD card slot
- miniUSB for charging and data comm.

<img src="/HARDWARE_details/images/CzechRad_mainboard.jpg" alt="CzechRad main board" width="800"/>

## Detector board
- High Voltage power supply module by EMCO / XP Power instead of MEDCOM iRover
- buzzer, LEDs

<img src="/HARDWARE_details/images/CzechRad_detector_board_web.jpg" alt="CzechRad detector board" width="800"/>

## Detector unit - Geiger tube

Four different Geiger units of almost the same dimensions were considered for the device. From left to right: LND 7317 (USA) - used in Safecast devices; Saint-Gobain S800-1193 (France / India) - almost identical to the LND, but cheaper, was tested, but production discontinued during CzechRad prototype development, SPF "CONSENSUS" Gamma 6 and Gamma 6-1 (Russian Federation) - brand new design in production (not old Soviet military stock). ~~The Gamma 6-1 was chosen for the final design.~~

***UPDATE - detector changed to [LND 7317](https://www.lndinc.com/products/geiger-mueller-tubes/7317/) (USA)***

***As a result of 2022 Russian invasion of Ukraine and subsequent sanctions against the Russian Federation it was not possible to use the Russian detector in the device. The detector board had to be significantly redesigned and the [LND 7317](https://www.lndinc.com/products/geiger-mueller-tubes/7317/) pancake tube was chosen for the final design. Thanks to this, CzechRad has the same detector as bGeigie Nano - e.g. same sensitivity and other parameters. It also makes it possible to use the CzechRad electronics and combine them with used detector and Pelican case from an unfunctional Safecast bGeigie Nano.***

<img src="/HARDWARE_details/images/CzechRad_detectors_web.jpg" alt="CzechRad prototype with both boards and display" width="800"/>

### SPF "CONSENSUS" Gamma 6-1 tube properties

The [Gamma 6-1 tube](https://consensus-group.ru/radiation-counters/gamma-radiation/187-gamma-6-1) is also pancake type - but compared to the LND tube there is no  Mica window - e.g. we have given up the ability to measure other radiation than gamma (but such measurements were quite risky with the mechanically sensitive LND tube - potential Mica window damage etc.).

But the advantages of the Gamma 6-1 tube are:

- the tube is less sensitive to damage (no Mica window)

- it is lighter - 35 g compared to 125 g of the LND tube

- significantly higher sensitivity proven by field tests - below the same route measured at the same time by both devices:

30 minute measurement - raw pulses per 5s interval

<img src="/HARDWARE_details/images/Safecast_CzechRad_graph_pulses5s.png" alt="CzechRad vs. SAFECAST sensitivity - pulses 5s" width="700"/>

more than 11 hours measurement - calculated pulses per minute

<img src="/HARDWARE_details/images/Safecast_CzechRad_graph_CPM.png" alt="CzechRad vs. SAFECAST sensitivity - pulses per minute - CPM" width="700"/>


## Different high-voltage power supply

The custom designed US iRover high voltage supply by IMI (International Medcom Inc.) used in bGeigie Nano is hard to get and it is not freely available on the market.

<img src="/HARDWARE_details/images/HV_Safecast_IMI_iRover.jpg" alt="IMI iRover used in bGeigie Nano" width="500"/>

For CzechRad the high voltage power supply module from EMCO - XP Power was chosen. Maybe not cheaper, but easier to obtain and reliable HV supply module.

<img src="/HARDWARE_details/images/HV_CzechRad_EMCO.jpg" alt="EMCO HV power supply used in CzechRad" width="500"/>

## Different GPS module

Instead of the Ultimate GPS by Adafruit used in bGeigie Nano:

<img src="/HARDWARE_details/images/GPS_Safecast_Adafruit.jpg" alt="Ultimate GPS by Adafruit used in bGeigie Nano" width="400"/>

The module is available from Adafruit (USA), uses MTK3339 chipset and features 22 tracking / 66 acquisition-channel GPS receiver.

In CzechRad the NEO-M8 series GPS chip by u-blox was used instead:

<img src="/HARDWARE_details/images/GPS_CzechRad_uBlox.jpg" alt="NEO-M8 series by u-blox" width="400"/>

The GPS chip is mounted directly on the main board, features concurrent reception of up to 3 GNSS (GPS, Galileo, GLONASS, BeiDou), industry leading –167 dBm, navigation sensitivity.

Our field tests confirmed quicker position fix, possibly better sensitivity and precision.

## Quicker charging compared to bGeigie

- MAX1811 charging circuit by Maxim Integrated 
- at least 2 times quicker charging than bGeigie, but still "battery friendly"

