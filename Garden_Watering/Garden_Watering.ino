#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Default address of most PCF8574 modules, change according

int sensor1 = 4;
int sensor2 = 5;
int sensor3 = 6;
int sensor4 = 7;

// declaring the relay pins
int relay1 = 8;
int relay2 = 9;
int relay3 = 10;
int relay4 = 11;

int sense1state = 0;
int sense2state = 0;
int sense3state = 0;
int sense4state = 0;


void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);

  Serial.println("Automatic Garden Watering System");
  lcd.setCursor(0, 0);
  lcd.print("AUTOMATIC GARDEN");
  lcd.setCursor(0, 1);
  lcd.print("WATERING SYSTEM");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("DEVICE READY");
  delay(2000);
  lcd.clear();

  sense1state = 0;
  sense2state = 0;
  sense3state = 0;
  sense4state = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  int sense1 = digitalRead(sensor1);
  int sense2 = digitalRead(sensor2);
  int sense3 = digitalRead(sensor3);
  int sense4 = digitalRead(sensor4);

  Serial.print("S1: ");
  Serial.print(sense1);
  Serial.println("    ");
  Serial.print("  S2: ");
  Serial.println(sense2);

  Serial.print("S3: ");
  Serial.print(sense3);
  Serial.println("    ");
  Serial.print("S4: ");
  Serial.println(sense4);
  delay(300);

  lcd.setCursor(0, 0);
  lcd.print("S1: ");
  lcd.setCursor(4, 0);
  lcd.print(sense1state);

  lcd.setCursor(9, 0);
  lcd.print("S2: ");
  lcd.setCursor(13, 0);
  lcd.print(sense2state);

  lcd.setCursor(0, 1);
  lcd.print("S3: ");
  lcd.setCursor(4, 1);
  lcd.print(sense3state);

  lcd.setCursor(9, 1);
  lcd.print("S4: ");
  lcd.setCursor(13, 1);
  lcd.print(sense4state);

  if (sense1 == HIGH) {
    digitalWrite(relay1, HIGH);
    sense1state = 1;
    Serial.println("Valve 1 on for water flow");
    lcd.setCursor(0, 0);
    lcd.print("S1: ");
    lcd.setCursor(4, 0);
    lcd.print(sense1state);

  } else if (sense1 == LOW) {
    digitalWrite(relay1, LOW);
    sense1state = 0;
    Serial.println("Valve 1 Off no water flow");
    lcd.setCursor(0, 0);
    lcd.print("S1: ");
    lcd.setCursor(4, 0);
    lcd.print(sense1state);
  }

  if (sense2 == HIGH) {
    digitalWrite(relay2, HIGH);
    sense2state = 1;
    Serial.println("Valve 2 on for water flow");
    lcd.setCursor(9, 0);
    lcd.print("S2: ");
    lcd.setCursor(13, 0);
    lcd.print(sense2state);

  } else if (sense2 == LOW) {
    digitalWrite(relay2, LOW);
    sense2state = 0;
    Serial.println("Valve 2 off np water flow");
    lcd.setCursor(9, 0);
    lcd.print("S2: ");
    lcd.setCursor(13, 0);
    lcd.print(sense2state);
  }

  if (sense3 == HIGH) {
    digitalWrite(relay3, HIGH);
    sense3state = 1;
    Serial.println("Valve 3 on for water flow");
    lcd.setCursor(0, 1);
    lcd.print("S3: ");
    lcd.setCursor(4, 1);
    lcd.print(sense3state);

  } else if (sense3 == LOW) {
    digitalWrite(relay3, LOW);
    sense3state = 0;
    Serial.println("Valve 3 off no water flow");
    lcd.setCursor(0, 1);
    lcd.print("S3: ");
    lcd.setCursor(4, 1);
    lcd.print(sense3state);
  }

  if (sense4 == HIGH) {
    digitalWrite(relay4, HIGH);
    sense4state = 1;
    Serial.println("Valve 4 on for water flow");
    lcd.setCursor(9, 1);
    lcd.print("S4: ");
    lcd.setCursor(13, 1);
    lcd.print(sense4state);

  } else if (sense4 == LOW) {
    digitalWrite(relay4, LOW);
    sense4state = 0;
    Serial.println("Valve 4 off no water flow");
    lcd.setCursor(9, 1);
    lcd.print("S4: ");
    lcd.setCursor(13, 1);
    lcd.print(sense4state);
  }
}
