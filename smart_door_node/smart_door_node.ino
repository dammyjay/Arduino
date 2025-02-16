CODE FOR NODEMCU
/*************************************************************
  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/
// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLosEXdQG7"
#define BLYNK_DEVICE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN "LP3TDVWuQWTJjH-FEYC0XPNNgGMm5C4c"
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Keypad.h>
#include <EEPROM.h>
#include "Gsender.h"
#pragma region Globals
  char auth[] = "3KfaUs77vhUox4W1PoN_xSl66V-SE93c";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Infinix HOT 6 Pro";
char pass[] = "Dammy4real4";
// const char* ssid = "TP-Link_7D05";                           // WIFI network name
// const char* password = "123ABC___";                       // WIFI network password
uint8_t connection_state = 0;         // Connected to WIFI or not
uint16_t reconnect_interval = 10000;  // If not connected wait time to try again
#pragma endregion Globals
BlynkTimer timer;
#define Relay 14  //D8
#define Buzzer 10
#define ardu 16  // D0
WidgetLCD lcd(V7);
// #define arduin V8
//Relay State

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0) {
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  if (value == 1) {
    digitalWrite(Relay, LOW);
    Serial.println("Door Opened");
    delay(5000);
    Blynk.virtualWrite(V0, 0);
    digitalWrite(Relay, HIGH);
    Serial.println("Door Closed");
  } else {
    Serial.println("Door Closed");
  }
  // Update state
  Blynk.virtualWrite(V1, value);
}
// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED() {
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}
// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent() {
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}
uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr) {
  static uint16_t attempt = 0;
  Serial.print("Connecting to ");
  if (nSSID) {
    WiFi.begin(nSSID, nPassword);
    Serial.println(nSSID);
  } else {
    WiFi.begin(ssid, pass);
    Serial.println(ssid);
  }
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 50) {
    delay(200);
    Serial.print(".");
  }
  ++attempt;
  Serial.println("");
  if (i == 51) {
    Serial.print("Connection: TIMEOUT on attempt: ");
    Serial.println(attempt);
    if (attempt % 2 == 0)
      Serial.println("Check if access point available or SSID and Password\r\n");
    return false;
  }
  Serial.println("Connection: ESTABLISHED");
  Serial.print("Got IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}
void Awaits() {
  uint32_t ts = millis();
  while (!connection_state) {
    delay(50);
    if (millis() > (ts + reconnect_interval) && !connection_state) {
      connection_state = WiFiConnect();
      ts = millis();
    }
  }
}
int arduino_input = 0;
void setup() {
  // Debug console
  Serial.begin(115200);
  pinMode(Relay, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(ardu, INPUT);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  lcd.clear();                         //Use it to clear the LCD Widget
  lcd.print(0, 0, "FINGERPRINT KEY");  // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
  lcd.print(0, 1, "  ACCESS DOOR  ");
  lcd.clear();
  lcd.print(0, 0, "* = ENTER CODE");
  lcd.print(0, 1, "# = CHANGE CODE");
  digitalWrite(Buzzer, HIGH);
  digitalWrite(Relay, HIGH);
  delay(500);
  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
  connection_state = WiFiConnect();
  if (!connection_state)  // if not connected to WIFI
    Awaits();             // constantly trying to connect
  Serial.println("Standby");
  yield();
}
void loop() {
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function
  int arduino_input = digitalRead(ardu);
  Serial.println(arduino_input);
  if (arduino_input == HIGH) {
    Blynk.virtualWrite(V8, 1);
    delay(2000);
    Blynk.virtualWrite(V8, 0);

  } else {
    Blynk.virtualWrite(V8, 0);
  }
}
