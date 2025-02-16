#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>


#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(2, 3);

#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial1

#endif


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int buzzer = 13;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

// 'finger2', 128x32px

uint8_t getFingerprintEnroll(uint8_t id);

int process = 0;
int numberpressed = 0;
int IDNumber;
String fingerID = "";

const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {11, 10, 9, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 6, 5};  //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000
};

// 'finger2', 128x32px
const unsigned char myBitmap [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void setup()
{
  Serial.begin(9600);

  /** display.begin(SSD1306_SWITCHCAPVCC, 0x3D); //or 0x3C
    display.clearDisplay(); //for Clearing the display
    display.drawBitmap(0, 0, myBitmap, 128, 32, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    display.display();
  **/

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  //display.display();
  //delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 1);
  display.println(("JAYKIRCH"));
  display.println(("TECH HUB"));
  display.display();      // Show initial text
  delay(2000);

  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(("FINGERPRINT"));
  display.setCursor(30, 18);
  display.println(("BASED"));
  display.display();
  delay(2000);

  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(30, 0);
  display.println(("CLASS"));
  display.println(("ATTENDANCE"));
  display.display();      // Show initial text
  delay(3000);


  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");
  pinMode(buzzer, OUTPUT);
  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }
  process = 2;
}

void loop()                     // run over and over again
{
  char customKey = customKeypad.getKey();
  if (customKey) {
    process = 0;
    /**if (customKey == '0') {
      numberpressed = 0;
    }**/
    if (customKey == '1') {
      numberpressed = 1;
    }
    if (customKey == '2') {
      numberpressed = 2;
    }
    if (customKey == '3') {
      numberpressed = 3;
    }
    if (customKey == '4') {
      numberpressed = 4;
    }
    if (customKey == '5') {
      numberpressed = 5;
    }
    if (customKey == '6') {
      numberpressed = 6;
    }
    if (customKey == '7') {
      numberpressed = 7;
    }
    if (customKey == '8') {
      numberpressed = 8;
    }
    if (customKey == '9') {
      numberpressed = 9;
    }
    if (customKey == '0') {
      fingerID = "";
      IDNumber = fingerID.toInt();
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println(("Enter FingerID"));
      display.setCursor(0, 18);
      display.println(("               "));
      display.display();      // Show initial text

    }
    if (customKey != '*' && customKey != '#' && customKey != 'C') {
      fingerID += numberpressed;
      IDNumber = fingerID.toInt();
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println(("Enter FingerID"));
      display.setCursor(0, 18);
      display.println((IDNumber));
      display.display();      // Show initial text

    }
    if (customKey == '*') {
      if (IDNumber >= 1) {
        newregistration();
      }
      if (fingerID == "") {
        fingerID = "";
        IDNumber = fingerID.toInt();
        display.clearDisplay();
        display.setTextSize(1); // Draw 2X-scale text
        display.setTextColor(WHITE);
        display.setCursor(0, 1);
        display.println(("Enter FingerID"));
        display.setCursor(0, 18);
        display.println(("               "));
        display.display();      // Show initial text
      }
    }
    if (customKey == '#') {
      process = 2;
    }
  }
  if (process == 2) {

    display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println(("Place Finger For "));
    display.setCursor(0, 18);
    display.println(("Class Attendance"));
    display.display();      // Show initial text
    getFingerprintIDez();
    delay(50);
  }
}
//
//
void newregistration() {
  int id = IDNumber;
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(0, 1);
  display.println(("Enrolling ID:"));
  display.setCursor(0, 18);
  display.println((id));
  display.display();      // Show initial text
  delay(2000);
  while (!getFingerprintEnroll(id));
}
//
//
uint8_t getFingerprintEnroll(uint8_t id) {
  uint8_t p = -1;
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 1);
  display.println(("PLACE A"));
  display.setCursor(30, 18);
  display.println(("FINGER"));
  display.display();      // Show initial text
  delay(2000);

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        display.clearDisplay();
        display.setTextSize(1); // Draw 2X-scale text
        display.setTextColor(WHITE);
        display.setCursor(10, 1);
        display.println(("iMAGE TAKEN"));
        display.setCursor(30, 18);
        display.println(("FINGER REGISTERED"));
        display.display();      // Show initial text
        delay(2000);
        break;
      case FINGERPRINT_NOFINGER:
        display.clearDisplay();
        display.setTextSize(1); // Draw 2X-scale text
        display.setTextColor(WHITE);
        display.setCursor(10, 1);
        display.println(("Please.. Place"));
        display.setCursor(30, 18);
        display.println(("Your Finger Now"));
        display.display();      // Show initial text
        delay(2000);
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        display.clearDisplay();
        display.setTextSize(1); // Draw 2X-scale text
        display.setTextColor(WHITE);
        display.setCursor(10, 1);
        display.println(("Communication"));
        display.setCursor(30, 18);
        display.println(("Error...... Wait"));
        display.display();      // Show initial text
        delay(2000);
        break;
      case FINGERPRINT_IMAGEFAIL:
        display.clearDisplay();
        display.setTextSize(1); // Draw 2X-scale text
        display.setTextColor(WHITE);
        display.setCursor(10, 1);
        display.println(("IMAGING"));
        display.setCursor(30, 18);
        display.println(("ERROR...."));
        display.display();      // Show initial text
        delay(2000);
        break;
      default:
        display.clearDisplay();
        display.setTextSize(1); // Draw 2X-scale text
        display.setTextColor(WHITE);
        display.setCursor(10, 1);
        display.println(("Unknown Error.."));
        display.setCursor(30, 18);
        display.println(("Restart Device!!"));
        display.display();      // Show initial text
        delay(2000);
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(10, 1);
      display.println(("Image Converted"));
      display.setCursor(30, 18);
      display.println(("Successfully!!!"));
      display.display();      // Show initial text
      delay(2000);
      break;
    case FINGERPRINT_IMAGEMESS:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(10, 1);
      display.println(("Image Messy"));
      display.setCursor(30, 18);
      display.println(("Try Again"));
      display.display();      // Show initial text
      delay(2000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:

      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(10, 1);
      display.println(("Communication"));
      display.setCursor(30, 18);
      display.println(("Error...Wait!"));
      display.display();      // Show initial text
      delay(2000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(10, 1);
      display.println(("Attention Please"));
      display.setCursor(30, 18);
      display.println(("Finger Not Found"));
      display.display();      // Show initial text
      delay(2000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(10, 1);
      display.println(("Attention Please"));
      display.setCursor(30, 18);
      display.println(("Finger Not Found"));
      display.display();      // Show initial text
      delay(2000);
      return p;
    default:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(10, 1);
      display.println(("Unknown Error.."));
      display.setCursor(30, 18);
      display.println(("Restart Device!!"));
      display.display();      // Show initial text
      delay(2000);
      return p;
  }

  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 1);
  display.println(("Remove Finger"));
  display.setCursor(30, 18);
  display.println(("And Wait Please"));
  display.display();      // Show initial text
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  p = -1;

  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 1);
  display.println(("Please..! Place"));
  display.setCursor(30, 18);
  display.println(("Your Finger Now"));
  display.display();      // Show initial text
  delay(2000);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        display.clearDisplay();
        display.setTextSize(1); // Draw 2X-scale text
        display.setTextColor(WHITE);
        display.setCursor(10, 1);
        display.println(("Thanks!! Finger"));
        display.setCursor(30, 18);
        display.println(("Image Captured!!"));
        display.display();      // Show initial text
        delay(2000);
        break;
      case FINGERPRINT_NOFINGER:

        display.clearDisplay();
        display.setTextSize(1); // Draw 2X-scale text
        display.setTextColor(WHITE);
        display.setCursor(0, 1);
        display.println(("Please.. Place"));
        display.setCursor(0, 18);
        display.println(("your finger now"));
        display.display();      // Show initial text
        delay(2000);
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        display.clearDisplay();
        display.setTextSize(1); // Draw 2X-scale text
        display.setTextColor(WHITE);
        display.setCursor(0, 1);
        display.println(("Communication..."));
        display.setCursor(0, 18);
        display.println(("Error... Wait"));
        display.display();      // Show initial text
        delay(2000);
        break;
      case FINGERPRINT_IMAGEFAIL:
        display.clearDisplay();
        display.setTextSize(2); // Draw 2X-scale text
        display.setTextColor(WHITE);
        display.setCursor(0, 1);
        display.println(("IMAGE"));
        display.setCursor(0, 18);
        display.println(("ERROR"));
        display.display();      // Show initial text
        delay(2000);
        break;
      default:
        display.clearDisplay();
        display.setTextSize(1); // Draw 2X-scale text
        display.setTextColor(WHITE);
        display.setCursor(10, 1);
        display.println(("Unknown Error.."));
        display.setCursor(30, 18);
        display.println(("Restart Device!!"));
        display.display();      // Show initial text
        delay(2000);
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println(("Image Converted"));
      display.setCursor(0, 18);
      display.println(("Successfully!!!"));
      display.display();      // Show initial text
      delay(2000);
      break;
    case FINGERPRINT_IMAGEMESS:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println(("Image Too"));
      display.setCursor(0, 18);
      display.println(("Messy Retry."));
      display.display();      // Show initial text
      delay(2000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println(("Communication..."));
      display.setCursor(0, 18);
      display.println(("Error... Wait"));
      display.display();      // Show initial text
      delay(2000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println(("Attention Please"));
      display.setCursor(0, 18);
      display.println(("Finger Not Found"));
      display.display();      // Show initial text
      delay(2000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println(("Attention Please"));
      display.setCursor(0, 18);
      display.println(("Finger Not Found"));
      display.display();      // Show initial text
      delay(2000);
      return p;
    default:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println(("Unknown Error..."));
      display.setCursor(0, 18);
      display.println(("Restart Device"));
      display.display();      // Show initial text
      delay(1000);
      return p;
  }


  // OK converted!
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println(("Finger Matched"));
    display.setCursor(0, 18);
    display.println(("                  "));
    display.display();      // Show initial text
    delay(2000);
    fingerID = "";
    IDNumber = fingerID.toInt();
    delay(1000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println(("Communication...."));
    display.setCursor(0, 18);
    display.println(("Error...Wait!!"));
    display.display();      // Show initial text
    delay(2000);
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println(("Attention Please"));
    display.setCursor(0, 18);
    display.println(("Finger Not Match"));
    display.display();      // Show initial text
    delay(2000);
    return p;
  } else {
    display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println(("Unknown Error...."));
    display.setCursor(0, 18);
    display.println(("Restart Device!!"));
    display.display();      // Show initial text
    delay(2000);
    return p;
  }

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.print("W");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println(("Communication..."));
    display.setCursor(0, 18);
    display.println(("Error.....Wait!!"));
    display.display();      // Show initial text
    delay(2000);
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println(("Could Not Store"));
    display.setCursor(0, 18);
    display.println(("In The Location"));
    display.display();      // Show initial text
    delay(2000);
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println(("Writing To Flash"));
    display.setCursor(0, 18);
    display.println(("Error...Restart"));
    display.display();      // Show initial text
    delay(2000);
    return p;
  } else {
    display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println(("Unknown Error...."));
    display.setCursor(0, 18);
    display.println(("Restart Device!!"));
    display.display();      // Show initial text
    delay(2000);
    return p;
  }
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println(("Image Taken "));
      display.setCursor(0, 18);
      display.println(("Successfully!!!"));
      display.display();      // Show initial text
      delay(2000);
      break;
    case FINGERPRINT_NOFINGER:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println(("Attention Please"));
      display.setCursor(0, 18);
      display.println(("Finger Not Found"));
      display.display();      // Show initial text
      delay(2000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println(("Communication..."));
      display.setCursor(0, 18);
      display.println(("Error.....Wait!!"));
      display.display();      // Show initial text
      delay(2000);
      return p;
    case FINGERPRINT_IMAGEFAIL:
      display.clearDisplay();
      display.setTextSize(2); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println(("IMAGE"));
      display.setCursor(0, 18);
      display.println(("ERROR"));
      display.display();      // Show initial text
      delay(2000);
      return p;
    default:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println(("Unknown Error..."));
      display.setCursor(0, 18);
      display.println(("Restart Device"));
      display.display();      // Show initial text
      delay(2000);
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println(("Image Converted"));
      display.setCursor(0, 18);
      display.println(("Successfully!!!"));
      display.display();      // Show initial text
      delay(2000);
      break;
    case FINGERPRINT_IMAGEMESS:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println(("Finger Image Too"));
      display.setCursor(0, 18);
      display.println(("Messy Try Again"));
      display.display();      // Show initial text
      delay(2000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println(("Communication..."));
      display.setCursor(0, 18);
      display.println(("Error.....Wait!!"));
      display.display();      // Show initial text
      delay(2000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println(("Attention Please"));
      display.setCursor(0, 18);
      display.println(("Finger Not Found"));
      display.display();      // Show initial text
      delay(2000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println(("Attention Please"));
      display.setCursor(0, 18);
      display.println(("Finger Not Found"));
      display.display();      // Show initial text
      delay(2000);
      return p;
    default:
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println(("Unknown Error..."));
      display.setCursor(0, 18);
      display.println(("Restart Device!!"));
      display.display();      // Show initial text
      delay(2000);
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println(("FInger Matched"));
    display.setCursor(0, 18);
    display.println(("               "));
    display.display();      // Show initial text
    delay(2000);
    fingerID = "";
    IDNumber = fingerID.toInt();
    delay(1000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println(("Communication..."));
    display.setCursor(0, 18);
    display.println(("Error.....Wait!!"));
    display.display();      // Show initial text
    delay(2000);
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println(("Attention Please"));
    display.setCursor(0, 18);
    display.println(("Finger Not Match"));
    display.display();      // Show initial text
    delay(2000);
    return p;
  } else {

    display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println(("Unknown Error..."));
    display.setCursor(0, 18);
    display.println(("Restart Device"));
    display.display();      // Show initial text
    delay(2000);
    return p;
  }
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) {
    //Serial.println("0");
    return -1;
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) {
    //Serial.println("0");
    return -1;
  }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    //Serial.println("0");
    return -1;
  }
  // found a match!
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(0, 1);
  display.println(("Finger ID Found"));
  display.setCursor(0, 18);
  display.println(("ID Number"));
  display.display();      // Show initial text
  delay(2000);

  display.print(finger.fingerID);
  delay(500);
  Serial.println(finger.fingerID);
  Serial.flush();
  delay(500);
  for (int i = 0; i <= 300; i++) {
    if (Serial.available() > 0) {
      i = 300;
      //
      String readText = "";
      //
      readText = Serial.readStringUntil('\n');
      delay(10);
      readText.toUpperCase();
      //
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println(("Matric Number"));
      display.setCursor(0, 18);
      display.println(("                "));
      display.display();      // Show initial text
      delay(2000);
    }
    delay(10);
  }

  return finger.fingerID;
}
