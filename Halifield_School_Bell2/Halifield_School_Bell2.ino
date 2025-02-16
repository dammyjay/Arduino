// now.pde
// Prints a snapshot of the current date and time along with the UNIX time
// Modified by Andy Wickert from the JeeLabs / Ladyada RTC library examples
// 5/15/11

#include <Wire.h>
#include <DS3231.h>
#include <LCD_I2C.h>

LCD_I2C lcd(0x27, 16, 2);  // Default address of most PCF8574 modules, change according



RTClib myRTC;
int myYear;
int myMon;
int myDay;
int myHour;
int myMin;
int mySec;
int count;

int buzzer = 13;  //buzzer to arduino pin 9
int relay = 12;
void setup() {
  Serial.begin(57600);
  lcd.begin();  // If you are using more I2C devices using the Wire library use lcd.begin(false)
                // this stop the library(LCD_I2C) from calling Wire.begin()
  lcd.backlight();
  Wire.begin();
  delay(500);
  Serial.println("Nano Ready!");
  pinMode(buzzer, OUTPUT);  // Set buzzer - pin 9 as an output
  pinMode(relay, OUTPUT);  // Set buzzer - pin 9 as an output
  lcd.setCursor(0, 0);      // Or setting the cursor in the desired position.
  lcd.print("HALIFIELD SCHOOL");

  lcd.setCursor(0, 1);  // Or setting the cursor in the desired position.
  lcd.print("      BELL      ");
  delay(1000);

  lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
  lcd.print("  PRESENTED BY  ");

  lcd.setCursor(0, 1);  // Or setting the cursor in the desired position.
  lcd.print("   STEM CLUB    ");
  delay(1000);
  lcd.clear();
  digitalWrite(relay,HIGH);
}

void loop() {

  delay(1000);

  DateTime now = myRTC.now();
  myMon = now.month();
  myYear = now.year();
  myDay = now.day();
  myHour = now.hour();
  myMin = now.minute();
  mySec = now.second();

  Serial.print(myYear, DEC);
  Serial.print('/');
  Serial.print(myMon, DEC);
  Serial.print('/');
  Serial.print(myDay, DEC);
  Serial.print(' ');
  Serial.print(myHour, DEC);
  Serial.print(':');
  Serial.print(myMin, DEC);
  Serial.print(':');
  Serial.print(mySec, DEC);
  Serial.println();


  Serial.print(" since midnight 1/1/1970 = ");
  Serial.print(now.unixtime());
  Serial.print("s = ");
  Serial.print(now.unixtime() / 86400L);
  Serial.println("d");

  lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
  lcd.print("Date:");
  lcd.print(myDay);
  lcd.print("/");
  lcd.print(myMon);
  lcd.print("/");
  lcd.print(myYear);

  lcd.setCursor(0, 1);  // Or setting the cursor in the desired position.
  lcd.print("Time:");
  lcd.print(myHour);
  lcd.print(":");
  lcd.print(myMin);
  lcd.print(":");
  lcd.print(mySec);


  // if ((myHour == 12) && (myMin == 7) && (mySec == 0)) {
  //   Serial.println("Alarm1");
  //   lcd.clear();
  //   lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
  //   lcd.print("     ALARM 1    ");
  //   buzz4();
  //   lcd.clear();
  // }

  // if ((myHour == 7) && (myMin == 30) && (mySec == 1)) {
  //   Serial.println("Assembly");
  // }


  JSperiod1();
  JSperiod2();
  JSperiod3();
  JSperiod4();
  JSperiod5();
  JSperiod6();
  JSperiod7();
  SSperiod3();
  SSperiod5();
  SSlunch();
  JSsnack();
  JSlunch();
  test();
}

void JSsnack() {
  if ((myHour == 9) && (myMin == 50) && (mySec == 0)) {
    Serial.println("JS Snack Break");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("JS SNACK BREAK");
    buzz2();
    lcd.clear();
  }
}

void JSlunch() {
  if ((myHour == 11) && (myMin == 55) && (mySec == 0)) {
    Serial.println("JS Lunch Break");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("JS LUNCH BREAK");
    breakbuzz();
    lcd.clear();
  }
}

void SSlunch() {
  if ((myHour == 12) && (myMin == 50) && (mySec == 0)) {
    Serial.println("SS Lunch Break");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("JS LUNCH BREAK");
    breakbuzz();
    lcd.clear();
  }
}

void JSperiod1() {
  if ((myHour == 8) && (myMin == 5) && (mySec == 0)) {
    Serial.println("JS Period1");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("  JS Period 1   ");
    buzz1();
    lcd.clear();
  }

  if ((myHour == 8) && (myMin == 50) && (mySec == 0)) {
    Serial.println("JS Period1 round up");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("JS PERIOD RND UP");
    buzz2();
    lcd.clear();
  }

  if ((myHour == 8) && (myMin == 55) && (mySec == 0)) {
    Serial.println("JS Period1 Ends");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print(" JS PERIOD1 ENDS ");
    buzz4();
    lcd.clear();
  }
}

void JSperiod2() {
  if ((myHour == 9) && (myMin == 0) && (mySec == 0)) {
    Serial.println("JS Period2");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("  JS Period 2   ");
    buzz1();
    lcd.clear();
  }

  if ((myHour == 9) && (myMin == 45) && (mySec == 0)) {
    Serial.println("JS Period2 round up");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("JS PERIOD2 ENDIN");
    buzz2();
    lcd.clear();
  }

  if ((myHour == 9) && (myMin == 50) && (mySec == 0)) {
    Serial.println("JS Period2 Ends");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print(" JS PERIOD2 ENDS ");
    buzz4();
    lcd.clear();
  }
}

void JSperiod3() {
  if ((myHour == 10) && (myMin == 10) && (mySec == 0)) {
    Serial.println("JS Period3");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("  JS Period 3   ");
    buzz1();
    lcd.clear();
  }

  if ((myHour == 10) && (myMin == 55) && (mySec == 0)) {
    Serial.println("JS Period3 round up");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("JS PERIOD3 ENDIN");
    buzz2();
    lcd.clear();
  }

  if ((myHour == 11) && (myMin == 0) && (mySec == 0)) {
    Serial.println("JS Period3 Ends");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print(" JS PERIOD3 ENDS ");
    buzz4();
    lcd.clear();
  }
}

void JSperiod4() {
  if ((myHour == 11) && (myMin == 5) && (mySec == 0)) {
    Serial.println("JS Period4");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("  JS Period 4   ");
    buzz1();
    lcd.clear();
  }

  if ((myHour == 11) && (myMin == 50) && (mySec == 0)) {
    Serial.println("JS Period4 round up");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("JS PERIOD4 ENDIN");
    buzz2();
    lcd.clear();
  }

}

void JSperiod5() {
  if ((myHour == 12) && (myMin == 25) && (mySec == 0)) {
    Serial.println("JS Period5");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("  JS Period 5   ");
    buzz1();
    lcd.clear();
  }

  if ((myHour == 13) && (myMin == 10) && (mySec == 0)) {
    Serial.println("JS Period5 round up");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("JS PERIOD5 ENDIN");
    buzz2();
    lcd.clear();
  }

  if ((myHour == 13) && (myMin == 15) && (mySec == 0)) {
    Serial.println("JS Period5 Ends");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print(" JS PERIOD5 ENDS ");
    buzz4();
    lcd.clear();
  }
}

void JSperiod6() {
  if ((myHour == 13) && (myMin == 20) && (mySec == 0)) {
    Serial.println("JS Period6");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("  JS Period 6   ");
    buzz1();
    lcd.clear();
  }

  if ((myHour == 14) && (myMin == 5) && (mySec == 0)) {
    Serial.println("JS Period6 round up");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("JS PERIOD2 ENDIN");
    buzz2();
    lcd.clear();
  }

  if ((myHour == 14) && (myMin == 10) && (mySec == 0)) {
    Serial.println("JS Period6 Ends");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print(" JS PERIOD6 ENDS ");
    buzz4();
    lcd.clear();
  }
}

void JSperiod7() {
  if ((myHour == 14) && (myMin == 15) && (mySec == 0)) {
    Serial.println("JS Period7");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("  JS Period 7   ");
    buzz1();
    lcd.clear();
  }

  if ((myHour == 15) && (myMin == 0) && (mySec == 0)) {
    Serial.println("JS Period7 round up");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("JS PERIOD7 ENDIN");
    buzz2();
    lcd.clear();
  }
}

void SSperiod3() {
  if ((myHour == 9) && (myMin == 55) && (mySec == 0)) {
    Serial.println("SS Period3");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("  SS Period 3   ");
    buzz1();
    lcd.clear();
  }

  if ((myHour == 10) && (myMin == 40) && (mySec == 0)) {
    Serial.println("SS Period3 round up");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("SS PERIOD3 ENDIN");
    buzz2();
    lcd.clear();
  }

  if ((myHour == 10) && (myMin == 45) && (mySec == 0)) {
    Serial.println("SS Period3 Ends");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("SS PERIOD3 ENDS");
    buzz4();
    lcd.clear();
  }
}

void SSperiod5() {
  if ((myHour == 12) && (myMin == 0) && (mySec == 0)) {
    Serial.println("SS Period5");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("  SS Period 5   ");
    buzz1();
    lcd.clear();
  }

  if ((myHour == 12) && (myMin == 45) && (mySec == 0)) {
    Serial.println("SS Period5 round up");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("SS PERIOD5 ENDIN");
    buzz2();
    lcd.clear();
  }

  if ((myHour == 12) && (myMin == 50) && (mySec == 0)) {
    Serial.println("SS Period5 Ends");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("SS PERIOD5 ENDS ");
    buzz4();
    lcd.clear();
  }
}

void close() {
  if ((myHour == 15) && (myMin == 5) && (mySec == 0)) {
    Serial.println("JS Period7");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("  CLOSING TIME  ");
    closebuzz();
    lcd.clear();
  }
}

void test() {
  if ((myHour == 8) && (myMin == 1) && (mySec == 0)) {
    Serial.println("JS Period7");
    buzz2();
  }

  if ((myHour == 8) && (myMin == 2) && (mySec == 0)) {
    Serial.println("JS Period7");
    breakbuzz();
  }
}

void buzz2() {
  for (count = 0; count < 4; count++) {
    digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
    digitalWrite(relay, LOW);  // Send 1KHz sound signal...
    delay(2000);                 // ...for 5 sec
    digitalWrite(buzzer, LOW);   // Stop sound...
    digitalWrite(relay, HIGH);  // Send 1KHz sound signal...
    delay(1000);                 // ...for 1sec
  }
}

void buzz1() {
  for (count = 0; count < 15; count++) {
    digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
    digitalWrite(relay, LOW);  // Send 1KHz sound signal...
    delay(300);                  // ...for 5 sec
    digitalWrite(buzzer, LOW);   // Stop sound...
    digitalWrite(relay, HIGH);  // Send 1KHz sound signal...
    delay(300);                  // ...for 1sec
  }
}

void buzz3() {
  digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
  digitalWrite(relay, LOW);  // Send 1KHz sound signal...
  delay(7000);                 // ...for 5 sec
  digitalWrite(buzzer, LOW);   // Stop sound...
  digitalWrite(relay, HIGH);  // Send 1KHz sound signal...
  // delay(126000);        // ...for 1sec
  // delay(60000);
  // buzz2();
}

void buzz4() {
  for (count = 0; count < 7; count++) {
    digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
    digitalWrite(relay, LOW);  // Send 1KHz sound signal...
    delay(500);                  // ...for 5 sec
    digitalWrite(buzzer, LOW);   // Stop sound...
    digitalWrite(relay, HIGH);  // Send 1KHz sound signal...
    delay(100);                  // ...for 1sec

    digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
    digitalWrite(relay, LOW);  // Send 1KHz sound signal...
    delay(500);                  // ...for 5 sec
    digitalWrite(buzzer, LOW);   // Stop sound...
    digitalWrite(relay, HIGH);  // Send 1KHz sound signal...
    delay(100);                  // ...for 1sec
  }
  delay(1000);
}

void breakbuzz() {
  for (count = 0; count < 10; count++) {
    digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
    digitalWrite(relay, LOW);  // Send 1KHz sound signal...
    delay(100);                  // ...for 5 sec
    digitalWrite(buzzer, LOW);   // Stop sound...
    digitalWrite(relay, HIGH);  // Send 1KHz sound signal...
    delay(100);
  }
  digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
  digitalWrite(relay, LOW);  // Send 1KHz sound signal...
  delay(2000);                 // ...for 5 sec
  digitalWrite(buzzer, LOW);   // Stop sound...
  digitalWrite(relay, HIGH);  // Send 1KHz sound signal...
}

void closebuzz() {
  for (count = 0; count < 7; count++) {
    digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
    digitalWrite(relay, LOW);  // Send 1KHz sound signal...
    delay(2000);                 // ...for 5 sec
    digitalWrite(buzzer, LOW);   // Stop sound...
    digitalWrite(relay, HIGH);  // Send 1KHz sound signal...
    delay(1000);
  }
  digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
  digitalWrite(relay, LOW);  // Send 1KHz sound signal...
  delay(2000);                 // ...for 5 sec
  digitalWrite(buzzer, LOW);   // Stop sound...
  digitalWrite(relay, HIGH);  // Send 1KHz sound signal...
}