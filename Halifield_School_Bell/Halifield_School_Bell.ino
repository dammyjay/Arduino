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
void setup() {
  Serial.begin(57600);
  lcd.begin();  // If you are using more I2C devices using the Wire library use lcd.begin(false)
                // this stop the library(LCD_I2C) from calling Wire.begin()
  lcd.backlight();
  Wire.begin();
  delay(500);
  Serial.println("Nano Ready!");
  pinMode(buzzer, OUTPUT);  // Set buzzer - pin 9 as an output
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


  if ((myHour == 11) && (myMin == 17) && (mySec == 0)) {
    Serial.println("Alarm1");
    buzz3();
  }
  assembly();
  JSperiod1();
  JSperiod2();
  JSperiod3();
  JSperiod4();
  JSperiod5();
  JSperiod6();
  JSperiod7();
  test();
}

//Assmebly Bell ring
void assembly() {
  if ((myHour == 7) && (myMin == 30) && (mySec == 1)) {
    Serial.println("Assembly");
  }
}

//JS Snack break
void JSsnack() {
  if ((myHour == 9) && (myMin == 55) && (mySec == 0)) {
    Serial.println("JS Snack Break");
    buzz2();
  }
}

//JS Lunch Time
void JSlunch() {
  if ((myHour == 12) && (myMin == 0) && (mySec == 0)) {
    Serial.println("JS Lunch Break");
  }
}

//JS Period1
void JSperiod1() {
  if ((myHour == 8) && (myMin == 10) && (mySec == 0)) {
    Serial.println("JS Period1");
    buzz1();
  }

  if ((myHour == 8) && (myMin == 55) && (mySec == 0)) {
    Serial.println("JS Period1 round up");
    buzz2();
  }
}

//JS Period2
void JSperiod2() {
  if ((myHour == 9) && (myMin == 5) && (mySec == 0)) {
    Serial.println("JS Period2");
    buzz1();
  }

  if ((myHour == 9) && (myMin == 50) && (mySec == 0)) {
    Serial.println("JS Period2 round up");
    buzz2();
  }

  if ((myHour == 9) && (myMin == 55) && (mySec == 0)) {
    Serial.println("JS Period2 Ends");
    buzz2();
  }
}

//JS Period3
void JSperiod3() {
  if ((myHour == 10) && (myMin == 15) && (mySec == 0)) {
    Serial.println("JS Period3");
    buzz1();
  }

  if ((myHour == 11) && (myMin == 0) && (mySec == 0)) {
    Serial.println("JS Period3 round up");
    buzz2();
  }

  if ((myHour == 11) && (myMin == 5) && (mySec == 0)) {
    Serial.println("JS Period3 Ends");
    buzz();
  }

  //JS Period4
  void JSperiod4() {
    if ((myHour == 11) && (myMin == 10) && (mySec == 0)) {
      Serial.println("JS Period4");
      buzz1();
    }

    if ((myHour == 11) && (myMin == 55) && (mySec == 0)) {
      Serial.println("JS Period4 round up");
      buzz2();
    }
  }

  //JS Period5
  void JSperiod5() {
    if ((myHour == 12) && (myMin == 30) && (mySec == 0)) {
      Serial.println("JS Period5");
      buzz1();
    }

    if ((myHour == 13) && (myMin == 15) && (mySec == 0)) {
      Serial.println("JS Period5 round up");
      buzz2();
    }
  }

  //JS Period6
  void JSperiod6() {
    if ((myHour == 13) && (myMin == 25) && (mySec == 0)) {
      Serial.println("JS Period6");
      buzz1();
    }

    if ((myHour == 14) && (myMin == 10) && (mySec == 0)) {
      Serial.println("JS Period6 round up");
      buzz2();
    }
  }

  //JS Period7
  void JSperiod7() {
    if ((myHour == 14) && (myMin == 20) && (mySec == 0)) {
      Serial.println("JS Period7");
      buzz1();
    }

    if ((myHour == 15) && (myMin == 5) && (mySec == 0)) {
      Serial.println("JS Period7 round up");
      buzz2();
    }
  }

  void SSperiod3() {
    if ((myHour == 10) && (myMin == 0) && (mySec == 0)) {
      Serial.println("SS Period3");
      buzz1();
    }

    if ((myHour == 15) && (myMin == 5) && (mySec == 0)) {
      Serial.println("JS Period7 round up");
      buzz2();
    }
  }

  //Closing Time
  void close() {
    if ((myHour == 15) && (myMin == 10) && (mySec == 0)) {
      Serial.println("JS Period7");
      closebuzz();
    }
  }


  void test() {
    if ((myHour == 13) && (myMin == 15) && (mySec == 0)) {
      Serial.println("JS Period7");
      buzz1();
    }

    if ((myHour == 13) && (myMin == 15) && (mySec == 50)) {
      Serial.println("JS Period7");
      breakbuzz();
    }
  }

  void buzz2() {
    for (count = 0; count < 3; count++) {
      digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
      delay(3000);                 // ...for 5 sec
      digitalWrite(buzzer, LOW);   // Stop sound...
      delay(1000);                 // ...for 1sec
    }
  }

  void buzz1() {
    for (count = 0; count < 7; count++) {
      digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
      delay(300);                  // ...for 5 sec
      digitalWrite(buzzer, LOW);   // Stop sound...
      delay(300);                  // ...for 1sec
    }
  }

  void buzz3() {
    digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
    delay(7000);                 // ...for 7 sec
    digitalWrite(buzzer, LOW);   // Stop sound...
    // delay(126000);        // ...for 1sec
    // delay(60000);
    // buzz2();
  }

  void buzz4() {
    for (count = 0; count < 15; count++) {
      digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
      delay(300);                  // ...for 5 sec
      digitalWrite(buzzer, LOW);   // Stop sound...
      delay(300);                  // ...for 1sec
    }
  }

  void breakbuzz() {
    for (count = 0; count < 10; count++) {
      digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
      delay(100);                  // ...for 5 sec
      digitalWrite(buzzer, LOW);   // Stop sound...
      delay(100);
    }
    digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
    delay(2000);                 // ...for 5 sec
    digitalWrite(buzzer, LOW);   // Stop sound...
  }

  void closebuzz() {
    for (count = 0; count < 7; count++) {
      digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
      delay(2000);                 // ...for 5 sec
      digitalWrite(buzzer, LOW);   // Stop sound...
      delay(1000);
    }
    digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
    delay(2000);                 // ...for 5 sec
    digitalWrite(buzzer, LOW);   // Stop sound...
  }
