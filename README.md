# pc-info (Under Development)
A little project to see some info about your pc (ex. GPU temperature) on an external display driven by an ESP32.

# Installation
For noobs (arduino ide setup, python installation...)
for advanced...
COM /dev/... edit /dev/ttyUSB0
115200 BAUDRATE

# PINOUT
 * 18 SCK
 * 23 MOSI

# Requirements
 * ESP-32 Board or any I2C/SPI capable board (Personally tested on the TTGO T-Call V1.3 ESP32 board)
 * GC9A01 IPS LCD 240x240 (Using SPI communication)

# Known bug/problems
 * High refresh time (Approx 1,3 seconds)


# LICENSE
This project is under the MIT licence, see the LICENCE file for more informations.
