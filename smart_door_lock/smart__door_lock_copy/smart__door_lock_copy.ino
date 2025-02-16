#include "DFRobot_RGBLCD1602.h"
#include <Wire.h>


/**********************************************************************************
 *  TITLE: Blynk 2.0 + Manual Switch (Latched) control 4 Relays using NodeMCU (Real time feedback) (No Wi-Fi control)
 *  I have Modified the Blynk.Edgent.Edgent_ESP8266 example code (https://blynk.io/) for this project
 *  Click on the following links to learn more. 
 *  YouTube Video: https://youtu.be/O9VYZqWPNEQ
 *  Related Blog : https://iotcircuithub.com/home-automation-using-nodemcu-and-blynk/
 *  by Subhajit (Tech StudyCell)
 *  Preferences--> Aditional boards Manager URLs : 
 *  https://dl.espressif.com/dl/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
 *  
 *  Download Board ESP8266 NodeMCU : https://github.com/esp8266/Arduino
 *  Download the libraries
 *  Blynk 1.0.1 Library:  https://github.com/blynkkk/blynk-library
 **********************************************************************************/

// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPL2iXNFwiT"
#define BLYNK_DEVICE_NAME "Smart door"

#define BLYNK_FIRMWARE_VERSION "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

//#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

DFRobot_RGBLCD1602 lcd(/*lcdCols*/ 16, /*lcdRows*/ 2);  //16 characters and 2 lines of show

// define the GPIO connected with Relays and switches
#define RelayPin1 14  //D5
// #define RelayPin2 5   //D1
// #define RelayPin3 4  //D2
//#define RelayPin4 12  //D6
#define Sensor 12  // D6

#define SwitchPin1 10  //SD3
// #define SwitchPin2 D3  //D3
// #define SwitchPin3 13  //D7
// #define SwitchPin4 3   //RX

#define wifiLed 16  //D0

//Change the virtual pins according the rooms
#define VPIN_BUTTON_1 V1
// #define VPIN_BUTTON_2 V2
// #define VPIN_BUTTON_3 V3
// #define VPIN_BUTTON_4 V4
// #define VPIN_BUTTON_5 V5

// Relay State
bool toggleState_1 = LOW;  //Define integer to remember the toggle state for relay 1

// Switch State
bool SwitchState_1 = LOW;



#include "BlynkEdgent.h"

BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);
}

// When App button is pushed - switch the state

BLYNK_WRITE(VPIN_BUTTON_1) {
  long state = digitalRead(Sensor);
  toggleState_1 = param.asInt();
  if (toggleState_1 == 1) {
    digitalWrite(RelayPin1, HIGH);
    digitalWrite(state, LOW);
    lcd.setCursor(0, 1);
    lcd.print("  DOOR OPENED   ");
    Serial.println("Door opened");
    Serial.println("Motion detected! door opened");
  } else {
    digitalWrite(RelayPin1, LOW);
    lcd.setCursor(0, 1);
    lcd.print("  DOOR CLOSED   ");
    Serial.println("Door closed");
  }
}



void setup() {
  /*lcd.init();

  lcd.setRGB(colorR, colorG, colorB);

  // Print a message to the LCD.
  Serial.begin(115200);
  lcd.setCursor(0, 0);
  lcd.print("WIFI CONTROLLED ");

  lcd.setCursor(0, 1);
  lcd.print("   DOOR LOCK    ");
  Serial.println("Door lock");
  delay(2000);

  lcd.setCursor(0, 0);
  lcd.print("     STATUS     ");
  Serial.println("Door lock");*/
  Wire.begin(D2, D3);
  Serial.begin(115200);
  delay(100);

  pinMode(RelayPin1, OUTPUT);
  pinMode(wifiLed, OUTPUT);
  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(Sensor, INPUT);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, LOW);
  delay(300);
  digitalWrite(RelayPin1, HIGH);


  digitalWrite(wifiLed, LOW);

  BlynkEdgent.begin();

  Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
}

void loop() {

  BlynkEdgent.run();

  manual_control();  //Manual Switch Control
  long state = digitalRead(Sensor); 
  if (state == LOW) {
    //digitalWrite (Status, HIGH);
    Blynk.virtualWrite(V0, 1);
    Serial.println("Motion detected!");
    delay(1000);
  } else {
    Blynk.virtualWrite(V0, 0);
    //digitalWrite (Status, LOW);
    Serial.println("Motion absent!");
    delay(1000);
  }
}