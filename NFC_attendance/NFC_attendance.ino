#include <LiquidCrystal_I2C.h>

#include <Wire.h>
#include <Adafruit_PN532.h>
// #include <SoftwareSerial.h>

#define SDA_PIN A4
#define SCL_PIN A5

LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);
// SoftwareSerial BTSerial(2, 3);  // Bluetooth TX = 2, RX = 3

void setup() {
  Serial.begin(9600);
  // BTSerial.begin(9600);
  lcd.init();
  lcd.backlight();

  Serial.println("NFC Scanner Ready...");
  lcd.setCursor(0, 0);
  lcd.print("NFC ATTENDANCE ");  // lcd printing name of project
  lcd.setCursor(0, 1);
  lcd.print("     SYSTEM    ");
  delay(2000);
  lcd.clear();

  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();

  if (!versiondata) {
    Serial.println("Didn't find PN532 chip!");
    lcd.setCursor(0, 0);
    lcd.print("NFC CARD READER");  // lcd printing name of project
    lcd.setCursor(0, 1);
    lcd.print("   NOT FOUND   ");

    while (1)
      ;
  }

  nfc.SAMConfig();
}

void loop() {
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  lcd.setCursor(0, 0);
  lcd.print("PLACE YOUR CARD");  // lcd asking user to place card

  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    // Serial.print("Card UID: ");
    String tagID = "";

    for (uint8_t i = 0; i < uidLength; i++) {
      tagID += String(uid[i], HEX);
    }

    Serial.println(tagID);
    // BTSerial.println(tagID);  // Send UID via Bluetooth

    delay(1000);

    lcd.setCursor(0, 1);
    lcd.print("CARD ID: ");
    lcd.setCursor(10, 1);
    lcd.print(tagID);
    delay(2000);
    clearscreen();
  }
}

void clearscreen() {
  lcd.setCursor(0, 1);
  lcd.print("                  ");  // lcd printing name of project
}
