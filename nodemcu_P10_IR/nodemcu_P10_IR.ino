//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Test P10
/*
 * Original source code : https://github.com/busel7/DMDESP/blob/master/examples/TeksDiamdanJalan/TeksDiamdanJalan.ino by  busel7
 * Links to download libraries : https://github.com/busel7/DMDESP
*/

//----------------------------------------Include Library
//----------------------------------------see here: https://www.youtube.com/watch?v=8jMr94B8iN0 to add NodeMCU ESP8266 library and board
#include <DMDESP.h>
#include <fonts/ElektronMart6x8.h>
#include <fonts/Mono5x7.h>
//----------------------------------------

#include <LCD_I2C.h>

LCD_I2C lcd(0x27, 16, 2);  // Default address of most PCF8574 modules, change according
int timer1;
int timer2;

float Time;

int flag1 = 0;
int flag2 = 0;

float distance = 61;  // space beteen the 2 sensors in cm
float speed;

int ir_s1 = 2;
int ir_s2 = 5;
char chr[5];

int buzzer = 3;

//----------------------------------------DMD Configuration (P10 Panel)
#define DISPLAYS_WIDE 2                     //--> Panel Columns
#define DISPLAYS_HIGH 1                     //--> Panel Rows
DMDESP Disp(DISPLAYS_WIDE, DISPLAYS_HIGH);  //--> Number of Panels P10 used (Column, Row)


static char *Text[] = { "CAR SPEED DETECTOR" };

void Scrolling_Text(int y, uint8_t scrolling_speed) {
  static uint32_t pM;
  static uint32_t x;
  int width = Disp.width();
  Disp.setFont(Mono5x7);
  int fullScroll = Disp.textWidth(Text[0]) + width;
  if ((millis() - pM) > scrolling_speed) {
    pM = millis();
    if (x < fullScroll) {
      ++x;
    } else {
      x = 0;
      return;
    }
    Disp.drawText(width - x, y, Text[0]);
  }
}


//----------------------------------------------------------------------
// SETUP

void setup() {

  //----------------------------------------DMDESP Setup
  Disp.start();            //--> Run the DMDESP library
  Disp.setBrightness(50);  //--> Brightness level
  Disp.setFont(Mono5x7);   //--> Determine the font used
  //----------------------------------------

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



//----------------------------------------------------------------------
// LOOP

void loop() {

  Disp.loop();  //--> Run "Disp.loop" to refresh the LED
  // Disp.drawText(4, 0, "UTEH");  //--> Display text "Disp.drawText(x position, y position, text)"
  Scrolling_Text(9, 80);  //--> Show running text "Scrolling_Text(y position, speed);"

  // Serial.print("Sensor1 ");
  // Serial.print(digitalRead(ir_s1));

  // Serial.print(" Sensor2 ");
  // Serial.println(digitalRead(ir_s2));
 

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
      Disp.loop();                  //--> Run "Disp.loop" to refresh the LED
      Disp.drawText(0, 0, "NO CAR YET");  //--> Display text "Disp.drawText(x position, y position, text)"
        // led_module.clearScreen(true);
        // led_module.selectFont(SystemFont5x7);
        // led_module.drawString(10, 0, "NO CAR", 7, GRAPHICS_NORMAL);
        // led_module.drawString(5, 9, "DETECTED", 10, GRAPHICS_NORMAL);
        // // delay(2000);
        // led_module.clearScreen(true);
        // led_module.selectFont(SystemFont5x7);
        // led_module.drawString(0, 0, "SPEED LIMIT", 11, GRAPHICS_NORMAL);
        // led_module.drawString(5, 9, "   40Km/hr ", 10, GRAPHICS_NORMAL);
        // delay(2000);

    } else {
      // lcd.print("Searching...    ");
      Serial.println("Searching...    ");
      Disp.loop();                  //--> Run "Disp.loop" to refresh the LED
      Disp.drawText(0, 0, "SEARCHING.....");  //--> Display text "Disp.drawText(x position, y position, text)"
    //   led_module.clearScreen(true);
    //   led_module.selectFont(SystemFont5x7);
    //   led_module.drawString(0, 9, "SEARCHING..........", 25, GRAPHICS_NORMAL);
    //   led_module.drawString(0, 0, "DETECTED ", 8, GRAPHICS_NORMAL);
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
      Disp.loop();                  //--> Run "Disp.loop" to refresh the LED
      Disp.drawText(0, 0, "OVER");  //--> Display text "Disp.drawText(x position, y position, text)"
      Disp.drawText(25, 0, chr, 5);  //--> Display text "Disp.drawText(x position, y position, text)"
      Disp.drawText(56, 0, "km/Hr");  //--> Display text "Disp.drawText(x position, y position, text)"
      // led_module.selectFont(SystemFont5x7);
      // // led_module.drawString(0, 4, chr, 5, GRAPHICS_NORMAL);
      // led_module.clearScreen(true);
      // led_module.drawString(0, 9, chr, 5, GRAPHICS_NORMAL);
      // led_module.drawString(35, 9, "Km/hr", 6, GRAPHICS_NORMAL);
      // led_module.selectFont(SystemFont5x7);
      // led_module.drawString(0, 0, "OVERSPEED", 9, GRAPHICS_NORMAL);
      digitalWrite(buzzer, HIGH);
      // led_module.drawString(0, 0, "", 0, GRAPHICS_NORMAL);

    } else {
      // lcd.print("  Normal Speed   ");
      Serial.print("  Normal Speed   ");
      Disp.loop();                  //--> Run "Disp.loop" to refresh the LED
      Disp.drawText(0, 0, "GOOD");  //--> Display text "Disp.drawText(x position, y position, text)"
       Disp.drawText(25, 0, chr, 5);  //--> Display text "Disp.drawText(x position, y position, text)"
      Disp.drawText(56, 0, "km/Hr");  //--> Display text "Disp.drawText(x position, y position, text)"
      // led_module.clearScreen(true);
      // led_module.drawString(0, 9, chr, 5, GRAPHICS_NORMAL);
      // led_module.drawString(35, 9, "Km/hr", 6, GRAPHICS_NORMAL);
      // led_module.selectFont(SystemFont5x7);
      // led_module.drawString(0, 0, "GOOD SPEED", 10, GRAPHICS_NORMAL);
      // led_module.drawString(4, 9, "SPEED", 5, GRAPHICS_NORMAL);
    }
    delay(3000);
    digitalWrite(buzzer, LOW);
    speed = 0;
    flag1 = 0;
    flag2 = 0;
  }
}