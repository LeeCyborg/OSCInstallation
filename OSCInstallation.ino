#include <AddicoreRFID.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#define PIN 6
#define  uchar unsigned char
#define uint  unsigned int
int TotalSteps = 10;
int fadeRate = 10;
int currTag;
uchar fifobytes;
uchar fifoValue;
AddicoreRFID myRFID;
const int chipSelectPin = 10;
const int NRSTPD = 5;
#define MAX_LEN 16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(300, PIN, NEO_GRB + NEO_KHZ800);
void setup() {
  Serial.begin(9600);
  SPI.begin();
  strip.begin();
  strip.show(); 
  pinMode(chipSelectPin, OUTPUT);             
  digitalWrite(chipSelectPin, LOW);        
  pinMode(NRSTPD, OUTPUT);                   
  digitalWrite(NRSTPD, HIGH);
  myRFID.AddicoreRFID_Init();
}

void loop()
{
  uchar i, tmp, checksum1;
  uchar status;
  uchar str[MAX_LEN];
  uchar RC_size;
  uchar blockAddr;
  String mynum = "";
  str[1] = 0x4400;
  status = myRFID.AddicoreRFID_Request(PICC_REQIDL, str);
  if (status == MI_OK)
  {
    Serial.println("RFID tag detected");
    uint tagType = str[0] << 8;
  }

  myRFID.AddicoreRFID_Halt(); 
  if (int(currTag) == 233) { // If the tag is this
    //colorWipe(strip.Color(255, 0, 0), 5); // Red

    // There are 300 LEDs, starting at 0
    setSection(0, 200, 255, 0, 0); // Set 0 to 200 RED
    setSection(201, 299,0, 255, 0); // Set the rest GREEN
    
  } // End each student code
  if (int(currTag) == 234) {
    //colorWipe(strip.Color(0, 255, 0), 0); // Green
  }
  if (int(currTag) == 246) {
    //colorWipe(strip.Color(0, 0, 255), 0); // Blue
  }
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
