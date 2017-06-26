#include <AddicoreRFID.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#define PIN 6
#define	uchar	unsigned char
#define	uint	unsigned int
int currTag;
uchar fifobytes;
uchar fifoValue;
AddicoreRFID myRFID;
const int chipSelectPin = 10;
const int NRSTPD = 5;
#define MAX_LEN 16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(20, PIN, NEO_GRB + NEO_KHZ800);
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

  status = myRFID.AddicoreRFID_Anticoll(str);
  if (status == MI_OK)
  {
    checksum1 = str[0] ^ str[1] ^ str[2] ^ str[3];
    Serial.println(str[4]);
    currTag = str[4];
    if (str[0] == 197)                   
    {
      Serial.println("\nHello Craig!\n");
    } else if (str[0] == 244) { 
      Serial.println("\nHello Erin!\n");
    }
    Serial.println();
    //delay(1000);
  }

  myRFID.AddicoreRFID_Halt();	
  if (int(currTag) == 233) {
    colorWipe(strip.Color(255, 0, 0), 5); // Red
  }
  if (int(currTag) == 234) {
    colorWipe(strip.Color(0, 255, 0), 0); // Green
  }
  if (int(currTag) == 246) {
    colorWipe(strip.Color(0, 0, 255), 0); // Blue
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

