# SensESP BN-880 GPS Example

This is an example project for a GNSS (GPS) receiver using an [SH-ESP32](https://shop.hatlabs.fi/products/sh-esp32), a [Beitian BN-880 module](https://www.aliexpress.com/item/32853587391.html), and [SensESP](https://signalk.org/SensESP/). The BN-880 module also includes a 3-axis digital compass. This project also includes rudimentary support for the compass. Proper calibration of the compass is not yet implemented, so it is not usable for navigation.

Given that most GPS modules transmit NMEA 0183 like sentences over a serial interface, this code can be trivially adapted to use other GPS modules.

