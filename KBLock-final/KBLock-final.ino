
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

const char correctPIN[] = "1234";  // Change this to your desired PIN
char enteredPIN[5];                // Buffer to store entered PIN
int enteredIndex = 0;              // Index to keep track of the entered PIN

const int relayPin = 10;   // Change this to the digital pin connected to the relay
const int buzzerPin = 11;  // Change this to the digital pin connected to the buzzer

int wrongAttempts = 0;  // Counter for wrong attempts
int count;

void setup() {
  Serial.begin(9600);

  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  // Turn on the buzzer for 3 seconds at the beginning
  digitalWrite(buzzerPin, HIGH);
  delay(3000);
  digitalWrite(buzzerPin, LOW);
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    // Beep the buzzer whenever a key is pressed
    digitalWrite(buzzerPin, HIGH);
    delay(50);  // Adjust the duration of the beep
    digitalWrite(buzzerPin, LOW);

    if (key == '#' && enteredIndex > 0) {
      // Check the entered PIN when '#' is pressed and at least one digit has been entered
      enteredPIN[enteredIndex] = '\0';  // Null-terminate the entered PIN
      if (strcmp(enteredPIN, correctPIN) == 0) {
        // Correct PIN entered, activate the relay (you can replace this with your action)
        Serial.println("Correct PIN! Activating the relay.");
        buzz2();
        digitalWrite(relayPin, LOW);  // Turn ON the relay
        delay(5000);                   // Keep the relay ON for 5 seconds (adjust as needed)
        digitalWrite(relayPin, HIGH);   // Turn OFF the relay
        // Reset wrongAttempts on correct PIN entry
        wrongAttempts = 0;
      } else {
        // Incorrect PIN, reset the entered PIN
        Serial.println("Incorrect PIN! Try again.");
        buzz4();
        wrongAttempts++;
        if (wrongAttempts >= 3) {
          // If 3 or more wrong attempts, sound the buzzer for 5 seconds
          digitalWrite(buzzerPin, HIGH);
          delay(5000);
          digitalWrite(buzzerPin, LOW);
          wrongAttempts = 0;  // Reset wrongAttempts after sounding the buzzer
        }
      }
      memset(enteredPIN, 0, sizeof(enteredPIN));  // Clear the entered PIN
      enteredIndex = 0;                           // Reset the entered index
    } else if (key != '#') {
      // Add the pressed key to the entered PIN
      if (enteredIndex < sizeof(enteredPIN) - 1) {
        enteredPIN[enteredIndex] = key;
        enteredIndex++;
      }
    }
  }
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