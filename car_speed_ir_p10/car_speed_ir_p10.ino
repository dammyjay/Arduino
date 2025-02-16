#include <LCD_I2C.h>

LCD_I2C lcd(0x27, 16, 2);  // Default address of most PCF8574 modules, change according

int timer1; // creating timer1
int timer2;

float Time;

int flag1 = 0;
int flag2 = 0;

float distance = 10;  // space beteen the 2 sensors in cm
float speed;

int ir_s1 = A0;
int ir_s2 = A1;

int buzzer = 12;

void setup() {
  Serial.begin(9600);
  pinMode(ir_s1, INPUT);
  pinMode(ir_s2, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.println("Car speed detector");
  lcd.begin();  // If you are using more I2C devices using the Wire library use lcd.begin(false)
                // this stop the library(LCD_I2C) from calling Wire.begin()
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   CAR SPEED   ");
  lcd.setCursor(0, 1);
  lcd.print("   DECTECTOR   ");
  delay(2000);
  lcd.clear();
}

void loop() {
  Serial.print("Sensor1 ");
  Serial.print(digitalRead(ir_s1));

  Serial.print(" Sensor2 ");
  Serial.println(digitalRead(ir_s2));
  
  if (digitalRead(ir_s1) == LOW && flag1 == 0) {
    timer1 = millis();
    flag1 = 1;
  }

  if (digitalRead(ir_s2) == LOW && flag2 == 0) {
    timer2 = millis();
    flag2 = 1;
  }

  if (flag1 == 1 && flag2 == 1) {
    if (timer1 > timer2) { Time = timer1 - timer2; }  // Checking if timer 1 is greater than timer 2 so the time differnce will always be positive value
    else if (timer2 > timer1) {
      Time = timer2 - timer1;
    }                           // Checking if timer 2 is greater than timer 1 so the time differnce will always be positive value
    Time = Time / 1000;         //convert millisecond to second
    speed = (distance / Time);  //v=d/t
    speed = speed * 3600;       //multiply by seconds per hr
    speed = speed / 1000;       //division by meters per Km
  }

  if (speed == 0) {
    lcd.setCursor(0, 1);
    if (flag1 == 0 && flag2 == 0) {
      lcd.print("No car  detected");
      Serial.println("No car  detected");
  
    } else {
      lcd.print("Searching...    ");
      Serial.println("Searching...    ");
    }
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Speed:");
    lcd.print(speed, 1);
    lcd.print("Km/Hr  ");
    lcd.setCursor(0, 1);

    Serial.print("Speed: ");
    Serial.print(speed, 3);
    Serial.println("Km/Hr  ");
    if (speed > 50) {
      digitalWrite(buzzer, HIGH);
      // delay(2000);
      lcd.print("  Over Speeding  ");
      Serial.print("  Over Speeding  ");
      
    } else {
      lcd.print("  Normal Speed   ");
      Serial.print("  Normal Speed   ");
    }
    delay(3000);
    digitalWrite(buzzer, LOW);
    speed = 0;
    flag1 = 0;
    flag2 = 0;
  }
}