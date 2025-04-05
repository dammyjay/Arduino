#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// Define the HC-06 RX and TX pins
#define HC06_RX 12
#define HC06_TX 13

SoftwareSerial BT(HC06_RX, HC06_TX);
SoftwareSerial gsm(9, 11);           // SIM800L TX to Arduino RX (9), SIM800L RX to Arduino TX (10)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Check your LCD I2C address (0x27 or 0x3F)

const int gasThreshold = 150;  // Adjust threshold based on environment
// Function prototypes
bool checkAT();
bool checkSIM();
int getSignalStrength();
int checkNetwork();

#define MQ2_PIN A1  // Gas sensor analog pin
#define BUZZER_PIN 6
#define RED_LED 7
#define GREEN_LED 8

void setup() {
  Serial.begin(9600);
  gsm.begin(9600);
  BT.begin(9600); // Set the baud rate to match the HC-06
  lcd.init();
  lcd.backlight();

  pinMode(MQ2_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  digitalWrite(GREEN_LED, HIGH);  // Normal state


  lcd.setCursor(0, 0);
  lcd.print("Gas leak Detector");  // lcd printing name of project
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  Serial.println("Initializing GSM Module...");
  delay(3000);

  // 1️⃣ Check GSM module response
  if (checkAT()) {
    Serial.println("✅ GSM Module Detected!");
    lcd.clear();
    lcd.print("GSM Module: OK");
  } else {
    Serial.println("❌ GSM Module NOT Detected!");
    lcd.clear();
    lcd.print("GSM ERROR!");
    lcd.setCursor(0, 1);
    lcd.print("Check Power/SIM");
    // while (true); // Stop execution if GSM fails
  }
  delay(2000);

  // 2️⃣ Check SIM Card
  if (checkSIM()) {
    Serial.println("✅ SIM Card Detected!");
    lcd.clear();
    lcd.print("SIM: OK");
  } else {
    Serial.println("❌ SIM Card NOT Detected!");
    lcd.clear();
    lcd.print("SIM ERROR!");
    // while (true);
  }
  delay(2000);

  // 3️⃣ Check Signal Strength
  int signal = getSignalStrength();
  Serial.print("📡 Signal Strength: ");
  Serial.println(signal);
  lcd.clear();
  lcd.print("Signal: ");
  lcd.print(signal);
  delay(2000);

  // 4️⃣ Check Network Registration
  int network = checkNetwork();
  if (network == 1 || network == 5) {
    Serial.println("✅ Network Registered!");
    lcd.clear();
    lcd.print("Network: OK");
  } else {
    Serial.println("❌ Not Registered!");
    lcd.clear();
    lcd.print("No Network!");
    // while (true);
  }

  Serial.println("✅ System Ready!");
  lcd.clear();
  lcd.print("Device Ready!");
  delay(2000);
  lcd.clear();
}

void loop() {

  int network = checkNetwork();
  if (network == 1 || network == 5) {
    Serial.println("✅ Network Registered!");
    // lcd.clear();
    // clearsim();
    lcd.setCursor(10, 0);
    lcd.print("Net OK!  ");
  } else {
    Serial.println("❌ Not Registered!");
    // lcd.clear();
    // clearsim();
    lcd.setCursor(10, 0);
    lcd.print("No Net!  ");
    // while (true);
  }

  int signal = getSignalStrength();
  Serial.print("📡 Signal Strength: ");
  Serial.println(signal);
  // lcd.clear();
  // clearsig();
  lcd.setCursor(10, 1);
  lcd.print("Sig:");
  lcd.print(signal);


  int gasValue = analogRead(MQ2_PIN);
  Serial.print("Gas Level: ");
  Serial.println(gasValue);
  // BT.println(gasValue);

  // lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GasL: ");
  lcd.print(gasValue);

  // if ((gasValue > gasThreshold) && (gasValue < 700)) {
  //   Serial.println("Gas Detected! Sending SMS...");
  //   lcd.setCursor(0, 1);
  //   lcd.print("Gas Leak!");

  //   digitalWrite(BUZZER_PIN, HIGH);
  //   digitalWrite(RED_LED, HIGH);
  //   digitalWrite(GREEN_LED, LOW);

  //   sendSMS("Warning! Gas leak detected! Take immediate action.");

  //   delay(5000);  // Avoid continuous SMS spam
  // }
  // else if (gasValue > 500  ){
  //   Serial.println("Gas Detected! Sending SMS...");
  //   lcd.setCursor(0, 1);
  //   lcd.print("Activated");
  //   sendSMS("Device Online");
  //   digitalWrite(BUZZER_PIN, HIGH);
  //   digitalWrite(RED_LED, HIGH);
  //   digitalWrite(GREEN_LED, LOW);
  //   }

  // else {
  //   digitalWrite(BUZZER_PIN, LOW);
  //   digitalWrite(RED_LED, LOW);
  //   digitalWrite(GREEN_LED, HIGH);
  //   lcd.setCursor(0, 1);
  //   lcd.print("Normal   ");
  // }
//-------------------------------------------------------------------------------------------
  if (gasValue > 450) {
    Serial.println("Gas Detected! Sending SMS...");
    lcd.setCursor(0, 1);
    lcd.print("Activated");
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    sendSMS("Device Online");
    delay(5000);
    
  } 
  
  else if (gasValue >= gasThreshold && gasValue <= 450) {
    Serial.println("Gas Detected! Sending SMS...");
    lcd.setCursor(0, 1);
    lcd.print("Gas Leak!");
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    sendSMS("Warning! Gas leak detected! Take immediate action.");
    BT.println("Warning! Gas leak detected! Take immediate action.");

    delay(5000);  // Avoid continuous SMS spam
  } 
  
  else {  // Gas level > 500
    
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Normal   ");
    // BT.println("Normal");

  }

}
  // Function to check GSM module response
  bool checkAT() {
    gsm.println("AT");
    delay(1000);
    if (gsm.available()) {
      String response = gsm.readString();
      return response.indexOf("OK") >= 0;
    }
    return false;
  }

  // Function to check if SIM card is inserted
  bool checkSIM() {
    gsm.println("AT+CPIN?");
    delay(1000);
    if (gsm.available()) {
      String response = gsm.readString();
      return response.indexOf("READY") >= 0;
    }
    return false;
  }

  // Function to get signal strength (returns RSSI value)
  int getSignalStrength() {
    gsm.println("AT+CSQ");
    delay(1000);
    if (gsm.available()) {
      String response = gsm.readString();
      int index = response.indexOf(":");
      if (index >= 0) {
        int value = response.substring(index + 2, index + 4).toInt();
        return value;  // Returns signal strength value
      }
    }
    return 0;
  }

  // Function to check network registration
  int checkNetwork() {
    gsm.println("AT+CREG?");
    delay(1000);
    if (gsm.available()) {
      String response = gsm.readString();
      int index = response.indexOf(",");
      if (index >= 0) {
        int status = response.substring(index + 1, index + 2).toInt();
        return status;  // Returns network status (1 = Home, 5 = Roaming)
      }
    }
    return 0;
  }

  void sendSMS(String message) {
    gsm.println("AT+CMGF=1");  // Set SMS mode
    delay(1000);
    gsm.println("AT+CMGS=\"+2348100427914\"");  // Replace with actual phone number
    delay(1000);
    gsm.print(message);
    delay(1000);
    gsm.write(26);  // End SMS with Ctrl+Z
    delay(3000);
  }

  void clearsim() {
    lcd.setCursor(10, 0);
    lcd.print("            ");
  }

  void clearsig() {
    lcd.setCursor(10, 1);
    lcd.print("            ");
  }
