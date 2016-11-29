
## trackertwo

This is a device tracking implementation of Particle.io 's  [Asset Tracker](https://github.com/spark/AssetTracker).  It uses the physical [Asset Tracker](https://www.particle.io/products/hardware/asset-tracker) when using the particle electron or uses a standalone [Adafruit Ultimate GPS](https://www.adafruit.com/product/746)


## Usage

These includes are setup for local compiling looking for a lib folder      
if you are compiling on the web IDE you should comment out this whole         
include block and add:  AssetTracker, Steaming, and HTTPClient using the web
IDE  

## Photon wiring

Photon pin | GPS Pin |Photon pin | GPS Pin
----| ------- | ----| -------
VIN| - | 3v3 | VIN
GND| - | RST | -
Tx| Rx | VBAT | -
Rx| Tx | GND | -
WKP| - | D7 | -
DAC| - | D6 | -
A5| - | D5 | -
A4| - | D4 | -
A3| - | D3 | -
A2| - | D2 | -
A1| - | D1 | -
A0| - | D0 | D-




### Links
[parse nmea messages ](http://freenmea.net/decoder)
[Adafruit GPS Library](https://github.com/adafruit/Adafruit_GPS)
[Adafruit Resources for Ultimate GPS](https://learn.adafruit.com/adafruit-ultimate-gps)
