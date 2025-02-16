

// #define FIREBASE_HOST "https://ses-project-2c8b9-default-rtdb.firebaseio.com"
// #define FIREBASE_AUTH "BibS5OQKlF6d5ZePNIFJGJDzgSkOlH8qXpCZzDSU"
// #define WIFI_SSID "Infinix HOT 6 Pro2"
// #define WIFI_PASSWORD "Dammy4real"
// #define ACS712_PIN A0        // Connect ACS712 analog output to A0
// #define PIR_SENSOR_PIN D2    // Connect PIR motion sensor to digital pin D2
// #define RELAY_PIN D1         // Connect relay to digital pin D1

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FirebaseESP8266.h>

// Replace with your network credentials
const char *ssid = "Infinix HOT 6 Pro2";
const char *password = "Dammy4real";

// Replace with your Firebase project URL and secret
#define FIREBASE_HOST "https://ses-project-2c8b9-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "BibS5OQKlF6d5ZePNIFJGJDzgSkOlH8qXpCZzDSU"

// Create an instance of the ESP8266WebServer class
ESP8266WebServer server(80);

// ACS712 analog pin
const int analogInPin = A0;
 
// PIR motion sensor and relay pins
const int motionSensorPin = D2;
const int relayPin = D1;

// Voltage divider constants
const float voltageDivider = 5.0; // Voltage divider ratio
const float voltageOffset = 0.76; // Voltage offset

// Create an instance of the LiquidCrystal_I2C library
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust the I2C address and LCD dimensions based on your module

// Initialize Firebase
FirebaseData firebaseData;

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  // Initialize the LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Energy Meter");

  // Set motion sensor and relay pins as input/output
  pinMode(motionSensorPin, INPUT);
  pinMode(relayPin, OUTPUT);

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  // Define the web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/setRelay", HTTP_POST, handleSetRelay);

  // Start the server
  server.begin();
}

void loop() {
  // Handle client requests
  server.handleClient();

  // Read motion sensor
  int motionValue = digitalRead(motionSensorPin);

  if (motionValue == HIGH) {
    Serial.println("Motion detected!");
    // Perform actions when motion is detected, e.g., turn on the relay
    digitalWrite(relayPin, HIGH);
  } else {
    Serial.println("No motion");
    // Perform actions when no motion is detected, e.g., turn off the relay
    digitalWrite(relayPin, LOW);
  }

  // Read ACS712 analog input
  int sensorValue = analogRead(analogInPin);

  // Convert analog reading to current (in mA)
  float current = (sensorValue / 1023.0) * 5000.0 / 185.0;

  // Read ESP8266 VCC (in mV)
  int vcc = ESP.getVcc();

  // Calculate real voltage (in V)
  float voltage = (vcc / 1000.0) * voltageDivider + voltageOffset;

  // Calculate power (in Watts)
  float power = voltage * current;

  // Calculate energy rate (Watt-hours) - you may need to integrate this over time for more accurate results
  float energyRate = power * (millis() / 1000.0) / 3600.0;

  // Update Firebase with the data
  Firebase.setString(firebaseData, "/voltage", String(voltage, 2));
  Firebase.setString(firebaseData, "/current", String(current, 2));
  Firebase.setString(firebaseData, "/power", String(power, 2));
  Firebase.setString(firebaseData, "/energyRate", String(energyRate, 2));
  Firebase.setString(firebaseData, "/motionStatus", motionValue == HIGH ? "Motion Detected" : "No Motion");

  // Additional logic goes here, if needed
  delay(1000); // Adjust delay based on your data update frequency
}

void handleRoot() {
  // Display data on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Voltage: " + String(Firebase.getString(firebaseData, "/voltage").c_str()) + "V");
  lcd.setCursor(0, 1);
  lcd.print("Current: " + String(Firebase.getString(firebaseData, "/current").c_str()) + "mA");

  // Build the HTML response
  String html = "<html><body>";
  html += "<h1>Energy Meter</h1>";
  html += "<p>Voltage: " + String(Firebase.getString(firebaseData, "/voltage").c_str()) + " V</p>";
  html += "<p>Current: " + String(Firebase.getString(firebaseData, "/current").c_str()) + " mA</p>";
  html += "<p>Power: " + String(Firebase.getString(firebaseData, "/power").c_str()) + " W</p>";
  html += "<p>Energy Rate: " + String(Firebase.getString(firebaseData, "/energyRate").c_str()) + " Wh</p>";
  html += "<p>Motion Status: " + String(Firebase.getString(firebaseData, "/motionStatus").c_str()) + "</p>";
  html += "</body></html>";

  // Send the response to the client
  server.send(200, "text/html", html);
}

void handleSetRelay() {
  // Get the relay state from the request
  String relayState = server.arg("state");
  
  // Control the relay based on the received state
  if (relayState.equalsIgnoreCase("on")) {
    digitalWrite(relayPin, HIGH);
  } else if (relayState.equalsIgnoreCase("off")) {
    digitalWrite(relayPin, LOW);
  }

  // Update Firebase with the current relay state
  Firebase.setString(firebaseData, "/relayState", relayState);

  // Send a response to the client
  server.send(200, "text/plain", "Relay state set to: " + relayState);
}
