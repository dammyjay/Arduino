#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 9, 8, 7, 6 };
byte colPins[COLS] = { 5, 4, 3, 2 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const char correctPIN[] = "1234D";  // Change this to your desired PIN
char enteredPIN[5];                // Buffer to store entered PIN

const int relayPin = 10;   // Change this to the digital pin connected to the relay
const int buzzerPin = 11;  // Change this to the digital pin connected to the buzzer
int count;
int i;

void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(relayPin, HIGH);   // Initialize the relay as OFF
  digitalWrite(buzzerPin, HIGH);  // Initialize the buzzer as OFF
  delay(3000);
  digitalWrite(buzzerPin, LOW);  // Initialize the buzzer as OFF
}

void loop() {
  char key = keypad.getKey();
  // int count = 0;
  if (key) {
    // Beep the buzzer whenever a key is pressed
    digitalWrite(buzzerPin, HIGH);
    delay(50);  // Adjust the duration of the beep
    digitalWrite(buzzerPin, LOW);
    Serial.print(key);

    if (key == '#') {
   
        // Check the entered PIN when '#' is pressed
        if (strcmp(enteredPIN, correctPIN) == 0) {
          // Correct PIN entered, activate the relay (you can replace this with your action)
          Serial.println("Correct PIN! Activating the relay.");
          digitalWrite(relayPin, LOW);  // Turn ON the relay
          delay(5000);
          digitalWrite(relayPin, HIGH);  // Turn OFF the relay
          
        } 
        else {
          // Incorrect PIN, reset the entered PIN
          Serial.println("Incorrect PIN! Try again.");
          Serial.println(enteredPIN);
          buzz4();
          }
      }

        memset(enteredPIN, 0, sizeof(enteredPIN));  // Clear the entered PIN
      
    } else if (key == 'C') {
      // 'C' key pressed, activate the "get pin" function (replace with your logic)
      Serial.println("Get PIN function activated.");
    } 
    // else {
    //   // Add the pressed key to the entered PIN
    //   strcat(enteredPIN, &key);
    // }
  }


void buzz4() {
  for (count = 0; count < 3; count++) {
    digitalWrite(buzzerPin, HIGH);  // Send 1KHz sound signal...
    delay(50);                      // ...for 5 sec
    digitalWrite(buzzerPin, LOW);   // Stop sound...
    delay(50);                      // ...for 1sec
  }
}

void buzz2() {
  for (count = 0; count < 2; count++) {
    digitalWrite(buzzerPin, HIGH);  // Send 1KHz sound signal...
    delay(200);                      // ...for 5 sec
    digitalWrite(buzzerPin, LOW);   // Stop sound...
    delay(200);                      // ...for 1sec
  }
}