/**************************************************************************/
/*!
    @file     iso14443a_uid.pde
    @author   Adafruit Industries
  @license  BSD (see license.txt)

    This example will attempt to connect to an ISO14443A
    card or tag and retrieve some basic information about it
    that can be used to determine what type of card it is.

    Note that you need the baud rate to be 115200 because we need to print
  out the data and read from the card at the same time!

  This is an example sketch for the Adafruit PN532 NFC/RFID breakout boards
  This library works with the Adafruit NFC breakout
  ----> https://www.adafruit.com/products/364

  Check out the links above for our tutorials and wiring diagrams
  These chips use SPI or I2C to communicate.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

*/
/**************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
int fadeRate = 10;
int MaxBrightness = 255;
int TotalSteps = 15;
// If using the breakout with SPI, define the pins for SPI communication.
#define PN532_SCK  (2)
#define PN532_MOSI (3)
#define PN532_SS   (4)
#define PN532_MISO (5)

// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield
int add;
// Uncomment just _one_ line below depending on how your breakout or shield
// is connected to the Arduino:

// Use this line for a breakout with a SPI connection:
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

// Use this line for a breakout with a hardware SPI connection.  Note that
// the PN532 SCK, MOSI, and MISO pins need to be connected to the Arduino's
// hardware SPI SCK, MOSI, and MISO pins.  On an Arduino Uno these are
// SCK = 13, MOSI = 11, MISO = 12.  The SS line can be any digital IO pin.
//Adafruit_PN532 nfc(PN532_SS);

// Or use this line for a breakout or shield with an I2C connection:
//Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

int vars[] = { 591, 680, 585, 547, 586, 579};

void setup(void) {
  Serial.begin(115200);
  Serial.println("Hello!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }

  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);

  // configure board to read RFID tags
  nfc.SAMConfig();

  Serial.println("Waiting for an ISO14443A card");
}

void loop(void) {
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  if (success) {
    Serial.println("Found a card!");
    add = 0;
    for (uint8_t i = 0; i < uidLength; i++)
    {
      add = add+ int(uid[i]);
    }
    Serial.println(add);
    if (add == vars[0]) { // forests
          setSection(0, 40, 0, 200, 0);// eng
          setSection(41, 59, 255, 0, 255);//fr

    }
    if (add == vars[1]) { // starbucks vs tims
      setSection(0,50, 255, 0, 0);// tims
          setSection(51, 59,0, 255, 0); // star
    }
    if (add == vars[2]) { // money
       setSection(0, 15, 0, 255, 0);// $$$
          setSection(16, 59,0, 0, 0); // everyone else
    }
    if (add == vars[3]) { // urban vs rural
      setSection(0, 20, 0, 255, 0);// 14% beavers
          setSection(41, 59,0, 0, 0); // everyone else
    }
    if (add == vars[4]) { // indig
          setSection(0, 10, 255, 150, 30);// 
          setSection(11, 59,0, 0, 0); // everyone else
    }
    if (add == vars[5]) { // lights
//           setSection(0, 10, 255, 0, 255);// 68% of the world
//          setSection(11, 21,0, 0, 0); // everyone else
//          setSection(22, 35, 255, 200, 0);// 68% of the world
//          setSection(36, 59,0, 0, 0); // everyone else
    }
    // Wait 1 second before continuing
    delay(1000);
  }
}
void setSection(int start, int finish, int Nred, int Ngreen, int Nblue) {
  uint32_t c = strip.getPixelColor(start);
  uint8_t  redCur = (c >> 16) & 0xFF;
  uint8_t  greenCur = (c >>  8) & 0xFF;
  uint8_t  blueCur = c & 0xFF;
  uint8_t redNew = Nred;
  uint8_t greenNew = Ngreen;
  uint8_t blueNew = Nblue;
  for (int i = 1; i < TotalSteps; i++)
  {
    uint8_t red = (((redCur * (TotalSteps - i)) + (redNew * i)) / TotalSteps);
    uint8_t green = (((greenCur * (TotalSteps - i)) + (greenNew * i)) / TotalSteps);
    uint8_t blue = (((blueCur * (TotalSteps - i)) + (blueNew * i)) / TotalSteps);
    for (int j = start; j < finish + 1; j++) {
      strip.setPixelColor(j, red, green, blue);
      strip.show();
      delay(fadeRate);
    }
  }
}
