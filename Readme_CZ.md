# CzechRad (stránka v přípravě)

<img src="Images/flag_EN.png" alt="čeština" width="128"/>

[For English version click here](README.md)  

Mobilní detektor pro radiační mapování - podobný přístroji SAFECAST bGeigie Nano. [Technical details here](/HARDWARE_details)  

<img src="Images/CzechRad_vs_bGeigie_web.jpg" alt="CzechRad prototype copared with SAFECAST bGeigie Nano" width="800"/>

Přístroj bude dokončen a vyroben v rámci projektu:


### Název projektu: Centrum pro podporu obyvatelstva pro případ skutečného nebo domnělého vzniku mimořádných jaderných a radiačních událostí

### (English: Center for the support of the population in case of actual or suspected occurence of extraordinary nuclear and radiation events)


<img src="Images/IMPAKT_loga_CZ.png" alt="Loga poskytovatele a řešitelů" width="400"/>

Trvání: 2021 — 2025

ID: VJ01010116

Poskytovatel 	MV0 - Ministerstvo vnitra (MV)

Program 	VJ - Strategická podpora rozvoje bezpečnostního výzkumu ČR 2019 - 2025 (IMPAKT 1)  (2019 - 2025)

[další informace o projektu - Informační systém výzkumu, vývoje a inovací (IS VaVaI)](https://www.isvavai.cz/cep?s=jednoduche-vyhledavani&ss=detail&n=0&h=VJ01010116)




### Cíle vývoje zařízení

1) vytvořit ekvivalentní zařízení k přístroji bGeigie Nano equivalent from the measurement point of view

2) zachovat výhody malé velikosti a stejné, dlouhodobě prověřené odolné pouzdro Pelican

3) zachovat stejný formát dat i jejich strukturu - tj. LOG soubory v textovém formátu, aby bylo možné:
    
    - nahrávat data do Safecast API bez nutnosti zásadních změn
    
    - využívat stávající nástroje pro offline zpracování dat, vyvinuté původně pro data z přístrojů Safecast

ALE chtěli jsme změnit několik věcí, které se nám na přístrojích SAFECAST nelíbí ...

### Different production / assembly approach

[SAFECAST bGeigie Nano](https://safecast.org/devices/bgeigie-nano/) consists of several modules connected / soldered together - some of them are obsolete and possible to get from US provider only. Assembly requires most of the modules to be soldered together, which makes hardware diagnostics and repairs complicated.

CzechRad uses only two boards instead of several 3rd party made modules. These boards are built using standard electronic components available worldwide from big sellers like Farnell, RS Components, Mouser Electronics etc. 

This eliminates the dependency on particular 3rd party module producers - we only rely on source electronic components manufacturer = we can buy components directly, not through US. module reseller. The configuration should also make diagnostics easier.

### CzechRad - Czech made

Both two boards manufactured and assembled in the Czech Republic by Tesla.

*...No, not by [that Tesla](https://en.wikipedia.org/wiki/Tesla,_Inc.) from the U.S. (est. 2003).*

*...but [this Tesla](https://en.wikipedia.org/wiki/Tesla_(Czechoslovak_company)) (est. 1946) - traditional manufacturer of electrical products and components established in Czechoslovakia.*

<img src="/Images/Tesla_Radio_vitrage_web.jpg" alt="Stained glass Tesla Radio logo in Passage Světozor in Prague, photo by Ludek, CC BY-SA 3.0" width="400"/>

