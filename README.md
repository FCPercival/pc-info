# PC-info Version 1
A little project to show some info about your pc (ex. GPU temperature) on an external display driven by an ESP32.
The python script works only for NVIDIA cards.

# Requirements
 * ESP-32 Board or any I2C/SPI capable board (**NOTE**: This code actually works only for ESP32 and ESP8266)
 * GC9A01 IPS LCD 240x240 (Using SPI communication)
 * Python 3


# Installation
There are two scripts, one for your microcontroller and one to run on your local PC

## Prepare your IDE and install Python
 1. Install the latest Python release from [here](https://www.python.org/downloads/)
 2. Install the following packages for python: gputil, pyserial (Ex. on cmd type: pip install gputil)
 3. Install Platformio for VSCode following this [guide](https://platformio.org/install/ide?install=vscode)


## Circuit schematics and how to upload the code to the microcontroller
*NOTE*: If you are not using a display with the GC9A01 driver, go to the Step 2 in the code, comment the line 
`Arduino_GC9A01 *gfx = new Arduino_GC9A01(bus, TFT_RST, 0 /* rotation */, true /* IPS */);`
and uncomment the code for your specific LCD driver [(More info)](https://github.com/FCPercival/pc-info/edit/main/README.md#supported-lcd-drivers)

Connect the ESP32 to the LCD :
    
       ESP32          |  LCD
       VCC (**3.3V**) -> VCC
       GND            -> GND
       Pin 18         -> SCL
       Pin 23         -> SDA
       Pin 33         -> RES
       Pin 27         -> DC
       Pin 5          -> CS
       Pin 22         -> BLK
       
Check the wiring and **make sure VCC is connected to the 3.3V pin on the board**. 
Now, proceed opening the "Microcontroller Code" folder with PlatformIO, in the PlatformIO tab select *Build Filesystem Image* under Platform and when it finishes click "Upload Filesystem Image" to upload the images that will be shown in the display, if everything was uploaded succesfully click*Upload* under *General* to upload the code.

After the code is succesfully uploaded you should see a message on the LCD screen. Now we need to run the python code on your local pc, so first we need to know in which port the microcontroller is connected, to do so look in the vscode console, platformio during the upload should have printed it (For example COM1 for Windows and /dev/ttyESB0 for Linux), now modify the port variable on the python script with that port and launch the script.

If everything is correct you should see the current GPU temperature on the screen.

# FAQ
 * The python script works for AMD cards? 
   
   The script only works for NVIDIA cards.
   
 * The python script works on Linux?

   Yes, the python script works on Linux.
 
# Known bug/problems
 * High refresh time (Approx 1 seconds)
 * The python script works only for NVIDIA cards.

# Planned features
 * AMD cards support

# Supported LCD drivers 
* GC9A01
* HX8347C
* HX8347D
* HX8352C
* HX8357B
* ILI9225
* ILI9341
* ILI9481
* ILI9486
* ILI9488
* JBT6K71
* R61529
* SEPS525
* SSD1283A
* SSD1331
* SSD1351
* ST7735
* ST7789
* ST7796

# LICENSE
This project is under the MIT licence, see the LICENSE file for more informations.
