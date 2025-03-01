#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN); 
SoftwareSerial BTSerial(2, 3); // Bluetooth TX = 2, RX = 3

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init(); 
  Serial.println("Scan an RFID card...");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String tagID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    tagID += String(mfrc522.uid.uidByte[i], HEX);
  }
  
  Serial.print("Card UID: ");
  Serial.println(tagID);
  
  BTSerial.println(tagID); // Send to Bluetooth module
  
  delay(1000);
}
