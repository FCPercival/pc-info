
/*********
 * 
 * Author Federico Vittorio Chiodo
 * This project is under the MIT License
 *
*********/


#include <Arduino.h>
//#include <String.h>
//#include <Preferences.h>  

/*
*
*
*   Functions and LCD Variables
*
*
*/

void updateLCD(int temperature);
void firstStart();


#define PNG_FILENAME_GREEN "/green.png" //Circle green
#define PNG_FILENAME_ORANGE_HEATING "/orange.png" //Cricle orange
#define PNG_FILENAME_RED_HEATING "/red.png"  //Circle Red with 

/*******************************************************************************
 * Start of Arduino_GFX setting
 ******************************************************************************/
#include <Arduino_GFX_Library.h>

/* first check if selected specific hardware */
// #define ESP32_LCDKIT_SPI
// #define TTGO_T_DISPLAY
// #define WT32_SC01
#if defined(ESP32_LCDKIT_SPI)
#define TFT_BL 23
Arduino_DataBus *bus = new Arduino_ESP32SPI(19 /* DC */, 5 /* CS */, 22 /* SCK */, 21 /* MOSI */, 27 /* MISO */);
Arduino_ILI9341 *gfx = new Arduino_ILI9341(bus, 18 /* RST */, 1 /* rotation */);

#elif defined(TTGO_T_DISPLAY)
#define TFT_BL 4
Arduino_DataBus *bus = new Arduino_ESP32SPI(16 /* DC */, 5 /* CS */, 18 /* SCK */, 19 /* MOSI */, -1 /* MISO */);
Arduino_ST7789 *gfx = new Arduino_ST7789(bus, 23 /* RST */, 2 /* rotation */, true /* IPS */, 135 /* width */, 240 /* height */, 53 /* col offset 1 */, 40 /* row offset 1 */, 52 /* col offset 2 */, 40 /* row offset 2 */);

#elif defined(WT32_SC01)
#define TFT_BL 23
Arduino_DataBus *bus = new Arduino_ESP32SPI(21 /* DC */, 15 /* CS */, 14 /* SCK */, 13 /* MOSI */, -1 /* MISO */);
Arduino_ST7796 *gfx = new Arduino_ST7796(bus, 22 /* RST */, 3 /* rotation */);

/* Wio Terminal */
#elif defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
#define TFT_BL LCD_BACKLIGHT
Arduino_HWSPI *bus = new Arduino_HWSPI(LCD_DC /* DC */, LCD_SS_PIN /* CS */);
Arduino_ILI9341 *gfx = new Arduino_ILI9341(bus, -1 /* RST */, 3 /* rotation */);

/* M5Stack */
#elif defined(ARDUINO_M5Stack_Core_ESP32) || defined(ARDUINO_M5STACK_FIRE)
#define TFT_BL 32
Arduino_ESP32SPI *bus = new Arduino_ESP32SPI(27 /* DC */, 14 /* CS */, SCK, MOSI, MISO);
Arduino_ILI9341_M5STACK *gfx = new Arduino_ILI9341_M5STACK(bus, 33 /* RST */, 1 /* rotation */);

/* Odroid-Go */
#elif defined(ARDUINO_ODROID_ESP32)
#define TFT_BL 14
Arduino_ESP32SPI *bus = new Arduino_ESP32SPI(21 /* DC */, 5 /* CS */, SCK, MOSI, MISO);
Arduino_ILI9341 *gfx = new Arduino_ILI9341(bus, -1 /* RST */, 3 /* rotation */);
// Arduino_ST7789 *gfx = new Arduino_ST7789(bus,  -1 /* RST */, 1 /* rotation */, true /* IPS */);

/* TTGO T-Watch */
#elif defined(ARDUINO_T) || defined(ARDUINO_TWATCH_BASE) || defined(ARDUINO_TWATCH_2020_V1) || defined(ARDUINO_TWATCH_2020_V2)
#define TFT_BL 12
Arduino_ESP32SPI *bus = new Arduino_ESP32SPI(27 /* DC */, 5 /* CS */, 18 /* SCK */, 19 /* MOSI */, -1 /* MISO */);
Arduino_ST7789 *gfx = new Arduino_ST7789(bus, -1 /* RST */, 2 /* rotation */, true /* IPS */, 240, 240, 0, 80);

#else /* not selected specific hardware */

#if defined(ESP32)
#define TFT_CS 5
// #define TFT_CS -1 // for display without CS pin
// #define TFT_DC 16
#define TFT_DC 27
// #define TFT_DC -1 // for display without DC pin (9-bit SPI)
// #define TFT_RST 17
#define TFT_RST 33
#define TFT_BL 22
#elif defined(ESP8266)
#define TFT_CS 15
#define TFT_DC 5
#define TFT_RST 16
// #define TFT_BL 4
#else
#define TFT_CS 9
#define TFT_DC 8
#define TFT_RST 7
#define TFT_BL 6
#endif

/*
 * Step 1: Initize one databus for your display
*/

// General software SPI
// Arduino_DataBus *bus = new Arduino_SWSPI(TFT_DC, TFT_CS, 18 /* SCK */, 23 /* MOSI */, -1 /* MISO */);

// General hardware SPI
//Arduino_DataBus *bus = new Arduino_HWSPI(TFT_DC, TFT_CS);

// ESP32 hardware SPI, more customizable parameters
 Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, 18 /* SCK */, 23 /* MOSI */, -1 /* MISO */, VSPI /* spi_num */);

// ESP32 parallel 8-bit
// Arduino_DataBus *bus = new Arduino_ESP32PAR8(TFT_DC, TFT_CS, 26 /* WR */, -1 /* RD */, 16 /* D0 */, 17 /* D1 */, 18 /* D2 */, 19 /* D3 */, 21 /* D4 */, 12 /* D5 */, 23 /* D6 */, 25 /* D7 */);

// ESP32 parallel 16-bit
// Almost all GPIO 0-31 used up for 16-bit and WR, disable PSRAM to gain 16 and 17 but still no GPIOs remain for CS and RD.
// CS connect to GND (enable); RD connect to Vcc (disable).
// Arduino_DataBus *bus = new Arduino_ESP32PAR16(
//     32 /* DC */, -1 /* CS */, 21 /* WR */, -1 /* RD */,
//     19 /* D0 */, 23 /* D1 */, 18 /* D2 */, 5 /* D3 */, 17 /* D4 */, 16 /* D5 */, 25 /* D6 */, 26 /* D7 */,
//     27 /* D8 */, 14 /* D9 */, 12 /* D10 */, 13 /* D11 */, 15 /* D12 */, 2 /* D13 */, 0 /* D14 */, 4 /* D15 */);

/*
 * Step 2: Initize one driver for your display
*/

// Canvas (framebuffer)
// Arduino_ST7789 *output_display = new Arduino_ST7789(bus, TFT_RST, 0 /* rotation */, true /* IPS */);
// 16-bit color Canvas (240x320 resolution only works for ESP32 with PSRAM)
// Arduino_Canvas *gfx = new Arduino_Canvas(240 /* width */, 320 /* height */, output_display);
// Indexed color Canvas, mask_level: 0-2, larger mask level mean less color variation but can have faster index mapping
// Arduino_Canvas_Indexed *gfx = new Arduino_Canvas_Indexed(240 /* width */, 320 /* height */, output_display, 0 /* output_x */, 0 /* output_y */, MAXMASKLEVEL /* mask_level */);

// GC9A01 IPS LCD 240x240
 Arduino_GC9A01 *gfx = new Arduino_GC9A01(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// HX8347C IPS LCD 240x320
// Arduino_HX8347C *gfx = new Arduino_HX8347C(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// HX8347D IPS LCD 240x320
// Arduino_HX8347D *gfx = new Arduino_HX8347D(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// HX8352C IPS LCD 240x400
// Arduino_HX8352C *gfx = new Arduino_HX8352C(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// HX8357B IPS LCD 320x480
// Arduino_HX8357B *gfx = new Arduino_HX8357B(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// ILI9225 LCD 176x220
// Arduino_ILI9225 *gfx = new Arduino_ILI9225(bus, TFT_RST);

// ILI9341 LCD 240x320
//Arduino_ILI9341 *gfx = new Arduino_ILI9341(bus, TFT_RST, 0 /* rotation */);

// ILI9481 LCD 320x480
// Arduino_ILI9481_18bit *gfx = new Arduino_ILI9481_18bit(bus, TFT_RST, 0 /* rotation */);

// ILI9486 LCD 320x480
// Arduino_ILI9486_18bit *gfx = new Arduino_ILI9486_18bit(bus, TFT_RST, 0 /* rotation */);

// ILI9488 LCD 320x480
// Arduino_ILI9488_18bit *gfx = new Arduino_ILI9488_18bit(bus, TFT_RST, 0 /* rotation */);

// JBT6K71 LCD 240x320
// Arduino_JBT6K71 *gfx = new Arduino_JBT6K71(bus, TFT_RST, 0 /* rotation */, true /* IPS */, 240, 320, 0, 0, 16, 0);

// R61529 IPS LCD 320x480
// Arduino_R61529 *gfx = new Arduino_R61529(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// SEPS525 OLED 160x128
// Arduino_SEPS525 *gfx = new Arduino_SEPS525(bus, TFT_RST, 0 /* rotation */);

// SSD1283A OLED 130x130
// Arduino_SSD1283A *gfx = new Arduino_SSD1283A(bus, TFT_RST, 0 /* rotation */);

// SSD1331 OLED 96x64
// Arduino_SSD1331 *gfx = new Arduino_SSD1331(bus, TFT_RST, 0 /* rotation */);

// SSD1351 OLED 128x128
// Arduino_SSD1351 *gfx = new Arduino_SSD1351(bus, TFT_RST, 0 /* rotation */);

// ST7735 LCD
// 1.8" REDTAB 128x160
// Arduino_ST7735 *gfx = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */);
// 1.8" BLACKTAB 128x160
// Arduino_ST7735 *gfx = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 128 /* width */, 160 /* height */, 2 /* col offset 1 */, 1 /* row offset 1 */, 2 /* col offset 2 */, 1 /* row offset 2 */, false /* BGR */);
// 1.8" GREENTAB A 128x160
// Arduino_ST7735 *gfx = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 128 /* width */, 160 /* height */, 2 /* col offset 1 */, 1 /* row offset 1 */, 2 /* col offset 2 */, 1 /* row offset 2 */);
// 1.8" GREENTAB B 128x160
// Arduino_ST7735 *gfx = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 128 /* width */, 160 /* height */, 2 /* col offset 1 */, 3 /* row offset 1 */, 2 /* col offset 2 */, 1 /* row offset 2 */);
// 1.8" Wide angle LCD 128x160
// Arduino_ST7735 *gfx = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 128 /* width */, 160 /* height */, 0 /* col offset 1 */, 0 /* row offset 1 */, 0 /* col offset 2 */, 0 /* row offset 2 */, false /* BGR */);
// 1.5" GREENTAB B 128x128
// Arduino_ST7735 *gfx = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 128 /* width */, 128 /* height */, 2 /* col offset 1 */, 3 /* row offset 1 */, 2 /* col offset 2 */, 1 /* row offset 2 */);
// 1.5" GREENTAB C 128x128
// Arduino_ST7735 *gfx = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 128 /* width */, 128 /* height */, 0 /* col offset 1 */, 32 /* row offset 1 */);
// 0.96" IPS LCD 80x160
// Arduino_ST7735 *gfx = new Arduino_ST7735(bus, TFT_RST, 3 /* rotation */, true /* IPS */, 80 /* width */, 160 /* height */, 26 /* col offset 1 */, 1 /* row offset 1 */, 26 /* col offset 2 */, 1 /* row offset 2 */);

// ST7789 LCD
// 2.4" LCD 240x320
// Arduino_ST7789 *gfx = new Arduino_ST7789(bus, TFT_RST, 0 /* rotation */);
// 2.4" IPS LCD 240x320
// Arduino_ST7789 *gfx = new Arduino_ST7789(bus, TFT_RST, 0 /* rotation */, true /* IPS */);
// 1.69" IPS round corner LCD 240x280
// Arduino_ST7789 *gfx = new Arduino_ST7789(bus, TFT_RST, 0 /* rotation */, true /* IPS */, 240 /* width */, 280 /* height */, 0 /* col offset 1 */, 20 /* row offset 1 */, 0 /* col offset 2 */, 20 /* row offset 2 */);
// 1.3"/1.5" square IPS LCD 240x240
// Arduino_ST7789 *gfx = new Arduino_ST7789(bus, TFT_RST, 2 /* rotation */, true /* IPS */, 240 /* width */, 240 /* height */, 0 /* col offset 1 */, 80 /* row offset 1 */);
// 1.14" IPS LCD 135x240 TTGO T-Display
// Arduino_ST7789 *gfx = new Arduino_ST7789(bus, TFT_RST, 0 /* rotation */, true /* IPS */, 135 /* width */, 240 /* height */, 53 /* col offset 1 */, 40 /* row offset 1 */, 52 /* col offset 2 */, 40 /* row offset 2 */);

// ST7796 LCD
// 4" LCD 320x480
// Arduino_ST7796 *gfx = new Arduino_ST7796(bus, TFT_RST, 0 /* rotation */);
// 4" IPS LCD 320x480
// Arduino_ST7796 *gfx = new Arduino_ST7796(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

#endif /* not selected specific hardware */
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

/* Wio Terminal */
#if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
#include <Seeed_FS.h>
#include <SD/Seeed_SD.h>
#elif defined(ESP32)
#include <SPIFFS.h>
#include <SD.h>
#elif defined(ESP8266)
#include <LittleFS.h>
#include <SD.h>
#else
#include <SD.h>
#endif

#include <pngle.h>
int16_t xOffset = 0;
int16_t yOffset = 0;

//Pngle init callback
void pngleInitCallback(pngle_t *pngle, uint32_t w, uint32_t h)
{
  int16_t gfxW = gfx->width();
  int16_t gfxH = gfx->height();
  xOffset = (w > gfxW) ? 0 : ((gfxW - w) / 2);
  yOffset = (h > gfxH) ? 0 : ((gfxH - h) / 2);
}

//Pngle draw callback
void pngleDrawCallback(pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t rgba[4])
{
  if (rgba[3]) //not transparent
  {
    gfx->fillRect(x + xOffset, y + yOffset, w, h, gfx->color565(rgba[0], rgba[1], rgba[2]));
  }
}


void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
  // Init Display
  gfx->begin();
  Serial.println("LCD initialized");
  gfx->fillScreen(BLACK);

  #ifdef TFT_BL
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
  #endif
  firstStart();

}
String lastTemp;

void loop(){
  if (Serial.available() > 0) {
    //Read the incoming String
    String temp = Serial.readString();
    if(temp.toInt()<10){
      return;
    }

    //Say what you got:
    Serial.print("I received: ");
    Serial.println(temp);
    if(!lastTemp.equals(temp)){
      lastTemp=temp;
      updateLCD(temp.toInt());
    }
    
  }
}

int cont=0;
void updateLCD(int temperature){
  if(temperature<=59){
    //*Set Green
    #if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
      if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI, 4000000UL))
    #elif defined(ESP32)
      if (!SPIFFS.begin())
      //if (!SD.begin(SS))
    #elif defined(ESP8266)
      if (!LittleFS.begin())
      //if (!SD.begin(SS))
    #else
      if (!SD.begin())
    #endif
      {
        Serial.println(F("ERROR: File System Mount Failed!"));
        gfx->println(F("ERROR: File System Mount Failed!"));
      }
      else
      {
        unsigned long start = millis();

      /* Wio Terminal */
      #if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
          File pngFile = SD.open(PNG_FILENAME_GREEN, "r");
      #elif defined(ESP32)
          File pngFile = SPIFFS.open(PNG_FILENAME_GREEN, "r");
          //File pngFile = SD.open(PNG_FILENAME_GREEN, "r");
      #elif defined(ESP8266)
          File pngFile = LittleFS.open(PNG_FILENAME_GREEN, "r");
          //File pngFile = SD.open(PNG_FILENAME_GREEN, "r");
      #else
          File pngFile = SD.open(PNG_FILENAME_GREEN, FILE_READ);
      #endif

      if (!pngFile || pngFile.isDirectory())
      {
        Serial.println(F("ERROR: Failed to open " PNG_FILENAME_GREEN " file for reading"));
        gfx->println(F("ERROR: Failed to open " PNG_FILENAME_GREEN " file for reading"));
      }
      else
      {
        pngle_t *pngle = pngle_new();
        pngle_set_init_callback(pngle, pngleInitCallback);
        pngle_set_draw_callback(pngle, pngleDrawCallback);
        char buf[16]; //Buffer minimum size is 16 but it can be much larger, e.g. 2048
        int remain = 0;
        int len;
        gfx->fillScreen(BLACK); //Transprant background color
        while ((len = pngFile.readBytes(buf + remain, sizeof(buf) - remain)) > 0)
        {
          int fed = pngle_feed(pngle, buf, remain + len);
          if (fed < 0)
          {
            Serial.printf("ERROR: %s\n", pngle_error(pngle));
            break;
          }

          remain = remain + len - fed;
          if (remain > 0)
          {
            memmove(buf, buf + fed, remain);
          }
      }

      pngle_destroy(pngle);
      pngFile.close();

      Serial.printf("Time used: %lu\n", millis() - start);

      gfx->setCursor(90, 175);  //100, 175
      gfx->setTextColor(GREEN);    //Green text 
      gfx->setTextSize(3 /* x scale */, 3 /* y scale */, 3 /* pixel_margin */);
      gfx->print(temperature); //*Symbol ° (char)223 o String tempUnit = " \xB0""C";
      gfx->print((char) 247); //*247 OK 9 meh
      gfx->println("C");

    }
  }
  }else if(temperature<=70){
    /* Wio Terminal */
    #if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
      if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI, 4000000UL))
    #elif defined(ESP32)
      if (!SPIFFS.begin())
      //if (!SD.begin(SS))
    #elif defined(ESP8266)
      if (!LittleFS.begin())
      //if (!SD.begin(SS))
    #else
      if (!SD.begin())
    #endif
      {
        Serial.println(F("ERROR: File System Mount Failed!"));
        gfx->println(F("ERROR: File System Mount Failed!"));
      }
      else
      {
        unsigned long start = millis();

      /* Wio Terminal */
      #if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
          File pngFile = SD.open(PNG_FILENAME_ORANGE_HEATING, "r");
      #elif defined(ESP32)
          File pngFile = SPIFFS.open(PNG_FILENAME_ORANGE_HEATING, "r");
          //File pngFile = SD.open(PNG_FILENAME_ORANGE_HEATING, "r");
      #elif defined(ESP8266)
          File pngFile = LittleFS.open(PNG_FILENAME_ORANGE_HEATING, "r");
          //File pngFile = SD.open(PNG_FILENAME_ORANGE_HEATING, "r");
      #else
          File pngFile = SD.open(PNG_FILENAME_ORANGE_HEATING, FILE_READ);
      #endif

      if (!pngFile || pngFile.isDirectory())
      {
        Serial.println(F("ERROR: Failed to open " PNG_FILENAME_ORANGE_HEATING " file for reading"));
        gfx->println(F("ERROR: Failed to open " PNG_FILENAME_ORANGE_HEATING " file for reading"));
      }
      else
      {
        pngle_t *pngle = pngle_new();
        pngle_set_init_callback(pngle, pngleInitCallback);
        pngle_set_draw_callback(pngle, pngleDrawCallback);
        char buf[16]; //Buffer minimum size is 16 but it can be much larger, e.g. 2048
        int remain = 0;
        int len;
        gfx->fillScreen(BLACK); //Transprant background color
        while ((len = pngFile.readBytes(buf + remain, sizeof(buf) - remain)) > 0)
        {
          int fed = pngle_feed(pngle, buf, remain + len);
          if (fed < 0)
          {
            Serial.printf("ERROR: %s\n", pngle_error(pngle));
            break;
          }

          remain = remain + len - fed;
          if (remain > 0)
          {
            memmove(buf, buf + fed, remain);
          }
      }

      pngle_destroy(pngle);
      pngFile.close();

      Serial.printf("Time used: %lu\n", millis() - start);

      gfx->setCursor(90, 175);  //100, 175
      gfx->setTextColor(ORANGE); //Orange text
      gfx->setTextSize(3 /* x scale */, 3 /* y scale */, 3 /* pixel_margin */);
      gfx->print(temperature); //*Symbol ° (char)223 o String tempUnit = " \xB0""C";
      gfx->print((char) 247); //*247 OK 9 meh
      gfx->println("C");
      
      }
    //delay(5000);
    }
  }else{
    /* Wio Terminal */
    #if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
      if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI, 4000000UL))
    #elif defined(ESP32)
      if (!SPIFFS.begin())
      //if (!SD.begin(SS))
    #elif defined(ESP8266)
      if (!LittleFS.begin())
      //if (!SD.begin(SS))
    #else
      if (!SD.begin())
    #endif
      {
        Serial.println(F("ERROR: File System Mount Failed!"));
        gfx->println(F("ERROR: File System Mount Failed!"));
      }
      else
      {
        unsigned long start = millis();

      /* Wio Terminal */
      #if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
          File pngFile = SD.open(PNG_FILENAME_RED_HEATING, "r");
      #elif defined(ESP32)
          File pngFile = SPIFFS.open(PNG_FILENAME_RED_HEATING, "r");
          // File pngFile = SD.open(PNG_FILENAME_RED_HEATING, "r");
      #elif defined(ESP8266)
          File pngFile = LittleFS.open(PNG_FILENAME_RED_HEATING, "r");
          // File pngFile = SD.open(PNG_FILENAME_RED_HEATING, "r");
      #else
          File pngFile = SD.open(PNG_FILENAME_RED_HEATING, FILE_READ);
      #endif

      if (!pngFile || pngFile.isDirectory())
      {
        Serial.println(F("ERROR: Failed to open " PNG_FILENAME_RED_HEATING " file for reading"));
        gfx->println(F("ERROR: Failed to open " PNG_FILENAME_RED_HEATING " file for reading"));
      }
      else
      {
        pngle_t *pngle = pngle_new();
        pngle_set_init_callback(pngle, pngleInitCallback);
        pngle_set_draw_callback(pngle, pngleDrawCallback);
        char buf[16]; //Buffer minimum size is 16 but it can be much larger, e.g. 2048
        int remain = 0;
        int len;
        gfx->fillScreen(BLACK); //Transprant background color
        while ((len = pngFile.readBytes(buf + remain, sizeof(buf) - remain)) > 0)
        {
          int fed = pngle_feed(pngle, buf, remain + len);
          if (fed < 0)
          {
            Serial.printf("ERROR: %s\n", pngle_error(pngle));
            break;
          }

          remain = remain + len - fed;
          if (remain > 0)
          {
            memmove(buf, buf + fed, remain);
          }
      }

      pngle_destroy(pngle);
      pngFile.close();

      Serial.printf("Time used: %lu\n", millis() - start);

      gfx->setCursor(90, 175);  //100, 175
      gfx->setTextColor(RED); //Red text
      gfx->setTextSize(3 /* x scale */, 3 /* y scale */, 3 /* pixel_margin */);
      gfx->print(temperature); //*Symbol ° (char)223 or String tempUnit = " \xB0""C";
      gfx->print((char) 247); //*247 OK 9 meh
      gfx->println("C");
      
      }
    //delay(5000);
    }
  }

  
}
void firstStart(){
  

  gfx->setCursor(0, 100);  //100, 175
  gfx->setTextColor(ORANGE);    //Orange text 
  gfx->setTextSize(2 /* x scale */, 2 /* y scale */, 3 /* pixel_margin */);
  gfx->println(" Run sendGPUTemp.pyw\n     on your pc!");

    
  
}

