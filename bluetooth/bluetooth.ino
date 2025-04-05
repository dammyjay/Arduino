#include <SoftwareSerial.h> // if using pins other than 0 and 1

// Define the HC-06 RX and TX pins
#define HC06_RX 10
#define HC06_TX 11

// Create a SoftwareSerial object (if using pins other than 0 and 1)
SoftwareSerial BT(HC06_RX, HC06_TX);

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  BT.begin(9600); // Set the baud rateu to match the HC-06

  // Print a message to the serial monitor
  Serial.println("Arduino is ready");

}

void loop() {
  // Check if data is available from the MIT App Inventor app
  int a = random(25);
  Serial.println(a);
  BT.println(a);
  delay(1000);
  if (BT.available()) {
    // Read the data
    char receivedChar = BT.read();

    // Print the received character to the serial monitor
    Serial.print("Received: ");
    Serial.println(receivedChar);

    // Process the received data (e.g., control an LED)
    if (receivedChar == '1') {
      digitalWrite(LED_BUILTIN, HIGH); // Turn on the LED
    } else if (receivedChar == '0') {
      digitalWrite(LED_BUILTIN, LOW); // Turn off the LED
    }

    
  }

  // Send data to the MIT App Inventor app (e.g., send the current time)
  // Serial.println(millis()); // Send the current time

}
