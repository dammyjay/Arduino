#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = { 9, 8, 7, 6 };
byte colPins[COLS] = { 5, 4, 3, 2 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const char correctPIN[] = "1234"; // Change this to your desired PIN
char enteredPIN[5]; // Buffer to store entered PIN

const int relayPin = 10; // Change this to the digital pin connected to the relay
const int buzzerPin = 11; // Change this to the digital pin connected to the buzzer

// I2C LCD configuration
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change the address (0x27) based on your LCD

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("    KEYPAD    ");
  lcd.setCursor(0, 1);
  lcd.print("     BASED     ");
  delay(2000);

  lcd.setCursor(0, 0);
  lcd.print("     DOOR      ");
  lcd.setCursor(0, 1);
  lcd.print("     LOCK      ");
  delay(2000);
  lcd.clear();
  lcd.print("Enter PIN:");
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Initialize the relay as OFF
  digitalWrite(buzzerPin, HIGH); // Initialize the buzzer as OFF
  delay(3000);
  digitalWrite(buzzerPin, LOW); // Initialize the buzzer as OFF
}

void loop() {
  char key = keypad.getKey();
  int count = 0;

  if (key) {
    // Display the pressed key on the LCD
    lcd.setCursor(0, 1);
    lcd.print("Key: ");
    lcd.print(key);

    // Beep the buzzer whenever a key is pressed
    digitalWrite(buzzerPin, HIGH);
    delay(50); // Adjust the duration of the beep
    digitalWrite(buzzerPin, LOW);

    if (key == '#') {
      // Check the entered PIN when '#' is pressed
      if (strcmp(enteredPIN, correctPIN) == 0) {
        // Correct PIN entered, activate the relay (you can replace this with your action)
        lcd.clear();
        lcd.print("Correct PIN");
        Serial.println("Correct PIN! Activating the relay.");
        digitalWrite(relayPin, HIGH); // Turn ON the relay
        delay(2000); // Keep the relay ON for 2 seconds (adjust as needed)
        digitalWrite(relayPin, LOW); // Turn OFF the relay
      } else {
        // Incorrect PIN, reset the entered PIN
        lcd.clear();
        lcd.print("Incorrect PIN");
        Serial.println("Incorrect PIN! Try again.");
        count++;
      }
      memset(enteredPIN, 0, sizeof(enteredPIN)); // Clear the entered PIN
      delay(2000); // Display the result for 2 seconds before clearing
      lcd.clear();
      lcd.print("Enter PIN:");
    } else if (key == 'C') {
      // 'C' key pressed, activate the "get pin" function (replace with your logic)
      lcd.clear();
      lcd.print("Get PIN function");
      Serial.println("Get PIN function activated.");
    } else {
      // Add the pressed key to the entered PIN
      strcat(enteredPIN, &key);
    }
  }
}
