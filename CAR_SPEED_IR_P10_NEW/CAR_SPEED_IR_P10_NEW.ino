#include <SPI.h>

#include <DMD.h>

#include <TimerOne.h>

#include "SystemFont5x7.h"

#include "Arial_black_16.h"

#include <LCD_I2C.h>

LCD_I2C lcd(0x27, 16, 2);  // Default address of most PCF8574 modules, change according
int timer1;
int timer2;

float Time;

int flag1 = 0; //creating flags1
int flag2 = 0;

float distance = 10;  // space beteen the 2 sensors in cm
float speed;

int ir_s1 = A0;
int ir_s2 = A1;
char chr[5];

int buzzer = 3;

#define ROW 2

#define COLUMN 1

#define FONT Arial_Black_16


DMD led_module(ROW, COLUMN);


void scan_module()

{

  led_module.scanDisplayBySPI();
}


void setup()

{

  Timer1.initialize(1000);

  Timer1.attachInterrupt(scan_module);

  led_module.clearScreen(true);

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
  led_module.clearScreen(true);
  led_module.selectFont(FONT);

  led_module.drawMarquee("CAR SPEED DETECTOR", 18, (32 * ROW) - 1, 0);  // the text, length of text to display, to display accros the row

  long start = millis();

  long timming = start;

  boolean flag = false;

  while (!flag)

  {

    if ((timming + 30) < millis())  //speed of the text

    {

      flag = led_module.stepMarquee(-1, 0);

      timming = millis();
    }
  }
}

void loop()

{
  // led_module.selectFont(SystemFont5x7);
  // led_module.drawString(8, 0, " NO CAR ", 10, GRAPHICS_NORMAL);
  // led_module.drawString(0, 9, " DETECTED ", 10, GRAPHICS_NORMAL);


  Serial.print("Sensor1 ");
  Serial.print(digitalRead(ir_s1));

  Serial.print(" Sensor2 ");
  Serial.println(digitalRead(ir_s2));

  if (digitalRead(ir_s1) == 0 && flag1 == 0) {
    timer1 = millis();
    flag1 = 1;
  }

  if (digitalRead(ir_s2) == 0 && flag2 == 0) {
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
      led_module.clearScreen(true);
      led_module.selectFont(SystemFont5x7);
      led_module.drawString(10, 0, "NO CAR", 7, GRAPHICS_NORMAL);
      led_module.drawString(5, 9, "DETECTED", 10, GRAPHICS_NORMAL);
      // delay(2000);
      // led_module.clearScreen(true);
      // led_module.selectFont(SystemFont5x7);
      // led_module.drawString(0, 0, "SPEED LIMIT", 11, GRAPHICS_NORMAL);
      // led_module.drawString(5, 9, "   40Km/hr ", 10, GRAPHICS_NORMAL);
      // delay(2000);

    } 
    else {
      // lcd.print("Searching...    ");
      Serial.println("Searching...    ");
      led_module.clearScreen(true);
      led_module.selectFont(SystemFont5x7);
      led_module.drawString(0, 9, "SEARCHING..........", 25, GRAPHICS_NORMAL);
      led_module.drawString(0, 0, "DETECTED ", 8, GRAPHICS_NORMAL);
    }
  } 
  
  else {
    // lcd.clear();
    // lcd.setCursor(0, 0);
    // lcd.print("Speed:");
    // lcd.print(speed, 1);
    // lcd.print("Km/Hr  ");
    // lcd.setCursor(0, 1);

    Serial.print("Speed: ");
    Serial.print(speed, 3);
    Serial.println("Km/Hr  ");

    dtostrf(speed, 4, 2, chr);
    // led_module.clearScreen(true);
    // led_module.selectFont(SystemFont5x7);
    // led_module.drawString(30, 0, chr, 7, GRAPHICS_NORMAL);
    // led_module.drawString(25, 9, "Km/hr", 6, GRAPHICS_NORMAL);
    
    if (speed > 40) {
      
      // delay(2000);

      dtostrf(speed, 4, 2, chr);
      // led_module.clearScreen(true);
      
      

      // lcd.print("  Over Speeding  ");
      Serial.print("  Over Speeding  ");
      led_module.selectFont(SystemFont5x7);
      // led_module.drawString(0, 4, chr, 5, GRAPHICS_NORMAL);
      led_module.clearScreen(true);
      led_module.drawString(0, 9, chr, 5, GRAPHICS_NORMAL);
      led_module.drawString(35, 9, "Km/hr", 6, GRAPHICS_NORMAL);
      led_module.selectFont(SystemFont5x7);
      led_module.drawString(0, 0, "OVERSPEED", 9, GRAPHICS_NORMAL);
      digitalWrite(buzzer, HIGH);
      // led_module.drawString(0, 0, "", 0, GRAPHICS_NORMAL);

    } else {
      // lcd.print("  Normal Speed   ");
      Serial.print("  Normal Speed   ");
      led_module.clearScreen(true);
      led_module.drawString(0, 9, chr, 5, GRAPHICS_NORMAL);
      led_module.drawString(35, 9, "Km/hr", 6, GRAPHICS_NORMAL);
      led_module.selectFont(SystemFont5x7);
      led_module.drawString(0, 0, "GOOD SPEED", 10, GRAPHICS_NORMAL);
      // led_module.drawString(4, 9, "SPEED", 5, GRAPHICS_NORMAL);
    }
    delay(3000);
    digitalWrite(buzzer, LOW);
    speed = 0;
    flag1 = 0;
    flag2 = 0;
  }
}
