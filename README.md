# pc-info (Under Development) Version 0.5
A little project to see some info about your pc (ex. GPU temperature) on an external display driven by an ESP32.
The python script works only for NVIDIA cards.

# Installation
For noobs (arduino ide setup, python installation...)
for advanced...
COM /dev/... edit /dev/ttyUSB0
115200 BAUDRATE

# How to run
 * Compile and upload the code in the folder "Microcontroller Code" to the ESP32 or the board of your choice and make sure it remains attached to one of your ports (It uses USB communication
 * Run the script send.py on your pc and the pc should talk to the microcontroller, if not please edit the port section

# PINOUT
 * 18 SCK
 * 23 MOSI

# Requirements
 * ESP-32 Board or any I2C/SPI capable board (Personally tested on the TTGO T-Call V1.3 ESP32 board)
 * GC9A01 IPS LCD 240x240 (Using SPI communication)

# Known bug/problems
 * High refresh time (Approx 1,3 seconds)
 * The python script works only for NVIDIA cards.


# LICENSE
This project is under the MIT licence, see the LICENCE file for more informations.
