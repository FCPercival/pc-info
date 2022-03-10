# PC-info Version 1
A little project to show some info about your pc (Ex. GPU temperature) on an external display driven by an ESP32.
The python script works only for NVIDIA cards.

# Requirements
 * ESP-32 Board or any I2C/SPI capable board (**NOTE**: This code works only for ESP32 and ESP8266)
 * GC9A01 IPS LCD 240x240 (Using SPI communication)
 * Python 3


# Installation
There are two scripts, one for your microcontroller and one to run on your local PC (sendBackground.pyw)

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
If you want to run the script at login put a shortcut on this folder `%APPDATA%\Roaming\Microsoft\Windows\Start Menu\Programs\Startup`

*Note*: You could also find an enclosure to fix the display. (stl file on "other files" folder)

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
- GC9A01 Round display 240x240 [[test video](https://youtu.be/kJrAFm20-zg)] (**Recommended**)
- HX8347C 240x320 [[test video](https://youtu.be/25ymuV51YQM)]
- HX8347D 240x320 [[test video](https://youtu.be/sv6LGkLRZjI)]
- HX8352C 240x400 [[test video](https://youtu.be/m2xWYbS3t7s)]
- HX8357B (9-bit SPI) 320x480 [[test video](https://youtu.be/pB6_LOCiUqg)]
- ILI9225 176x220 [[test video](https://youtu.be/jm2UrCG27F4)]
- ILI9341 240x320 [[test video](https://youtu.be/NtlEEL7MkQY)]
- ILI9341 M5Stack 320x240 [[test video](https://youtu.be/UoPpIjVSO5Q)]
- ILI9481 320x480 (18 bit color) [[test video](https://youtu.be/YxjuuCFhlqM)]
- ILI9486 320x480 (18 bit color) [[test video](https://youtu.be/pZ6izDqmVds)]
- ILI9488 320x480 (18 bit color) [[test video](https://youtu.be/NkE-LhtLHBQ)]
- JBT6K71 (8-bit Parallel) 240x320 [[test video](https://youtu.be/qid3F4Gb0mM)]
- R61529 (8-bit/16-bit Parallel) 320x480 [[test video](https://youtu.be/s93gxjbIAT8)]
- SEPS525 160x128 [[test video](https://youtu.be/tlmvFBHYv-k)]
- SSD1283A 130x130 [[test video](https://youtu.be/OrIchaRikiQ)]
- SSD1331 96x64 [[test video](https://youtu.be/v20b1A_KDcQ)]
- SSD1351 128x128 [[test video](https://youtu.be/5TIM-qMVBNQ)]
- SSD1351 128x96
- ST7735 128x160 (various tabs) [[test video](https://youtu.be/eRBSSD_N9II)]
- ST7735 128x128 (various tabs) [[test video](https://youtu.be/6rueSV2Ee6c)]
- ST7735 80x160 [[test video](https://youtu.be/qESHDuYo_Mk)]
- ST7789 TTGO T-Display 135x240 [[test video](https://youtu.be/Zk81_T8c20E)]
- ST7789 240x240 [[test video](https://youtu.be/Z27zYg5uAsk)]
- ST7789 TTGO T-Watch 240x240 [[test video](https://youtu.be/9AqsXMB8Qbk)]
- ST7789 round corner display 240x280 [[test video](https://youtu.be/KzDC02wg8z0)]
- ST7789 240x320 [[test video](https://youtu.be/ZEvc1LkuVuQ)]
- ST7796 320x480 [[test video](https://youtu.be/hUL-RuG4MAQ)]
- Canvas (framebuffer)
- Canvas_Indexed (for saving memory space)

# Images
<img src="https://github.com/FCPercival/pc-info/blob/main/Other%20Files/testGreen.jpg" width="500"><img src="https://github.com/FCPercival/pc-info/blob/main/Other%20Files/testOrange.jpg" width="500"><img src="https://github.com/FCPercival/pc-info/blob/main/Other%20Files/testRed.jpg" width="500">

# Libraries used
 * [Arduino GFX](https://github.com/moononournation/Arduino_GFX)
 * [JPGDEC](https://github.com/bitbank2/JPEGDEC)
 * [PNGLE](https://github.com/kikuchan/pngle)


# LICENSE
This project is under the MIT licence, see the LICENSE file for more informations.
