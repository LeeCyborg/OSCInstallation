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
//int oil = 228;
//int lang = 216;
//int immg = 227;
//int beaver = 203;
//int syrup = 229;
//int coffee = 238;
//int sport = 215;
//int ethnic = 248;
//int donut = 239;
//int lights = 257;
//int rural
//int immg
//int festival
//int 

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
    colorWipe(strip.Color(255, 0, 0), 50); // Red
  }
  if (add == vars[1]) {
    colorWipe(strip.Color(255, 255, 0), 50); // Red
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

