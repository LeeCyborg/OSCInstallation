// Example sketch to read the ID from an Addicore 13.56MHz RFID tag
// as found in the RFID AddiKit found at:
// http://www.addicore.com/RFID-AddiKit-with-RC522-MIFARE-Module-RFID-Cards-p/126.htm

#include <AddicoreRFID.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
int fadeRate = 10;
int MaxBrightness = 255;
int TotalSteps = 15;
#define  uchar unsigned char
#define uint  unsigned int

uchar fifobytes;
uchar fifoValue;
int add;

AddicoreRFID myRFID; // create AddicoreRFID object to control the RFID module

/////////////////////////////////////////////////////////////////////
//set the pins
/////////////////////////////////////////////////////////////////////
const int chipSelectPin = 10;
const int NRSTPD = 5;

//Maximum length of the array
#define MAX_LEN 16

int currTag;

int vars[] = { 216, 228, 227, 203, 229, 238, 215,
248, 239, 257, 228, 281, 238, 237, 248, 260, 204,
195, 206, 205 };


void setup() {
  Serial.begin(9600);                        // RFID reader SOUT pin connected to Serial RX pin at 9600bps

  // start the SPI library:
  SPI.begin();

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(chipSelectPin, OUTPUT);             // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin
  digitalWrite(chipSelectPin, LOW);         // Activate the RFID reader
  pinMode(NRSTPD, OUTPUT);                    // Set digital pin 10 , Not Reset and Power-down
  digitalWrite(NRSTPD, HIGH);

  myRFID.AddicoreRFID_Init();
  Serial.println("finished init");
}

void loop()
{
  uchar i, tmp, checksum1;
  uchar status;
  uchar str[MAX_LEN];
  uchar RC_size;
  uchar blockAddr;  //Selection operation block address 0 to 63

  str[1] = 0x4400;
  //Find tags, return tag type
  status = myRFID.AddicoreRFID_Request(PICC_REQIDL, str);
  if (status == MI_OK)
    Serial.println("RFID tag detected");
  status = myRFID.AddicoreRFID_Anticoll(str);
  if (status == MI_OK)
  {
    add = str[0] + str[1] + str[2] + str[3];
    Serial.println(add);
    myRFID.AddicoreRFID_Halt();
  }
  myRFID.AddicoreRFID_Halt();      //Command tag into hibernation
  if (add == vars[0]) {
    setSection(0, 2, 255, 0, 0);
    setSection(3, 5, 255, 0, 255);
  }
  if (add == vars[1]) {
     setSection(0, 2, 0, 255, 0);
    setSection(3, 5,0, 0, 255);
  }

}
//pretty self explanatory, cards should be saved in a uchar array like the variable 'blue' above
bool compareTag(uchar from[], uchar to[]) {
  if (from[0] == to[0] && from[1] == to[1] && from[2] == to[2] && from[3] == to[3])
    return true;
  return false;
}

void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
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
