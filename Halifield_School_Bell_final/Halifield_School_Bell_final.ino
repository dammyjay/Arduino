// now.pde
// Prints a snapshot of the current date and time along with the UNIX time
// Modified by Andy Wickert from the JeeLabs / Ladyada RTC library examples
// 5/15/11


#include <LCD_I2C.h>


LCD_I2C lcd(0x27, 16, 2);  // Default address of most PCF8574 modules, change according


#include <RTClib.h>

// event from 13:50 to 14:10
uint8_t DAILY_EVENT_START_HH = 13;  // event start time: hour
uint8_t DAILY_EVENT_START_MM = 50;  // event start time: minute
uint8_t DAILY_EVENT_END_HH = 14;    // event end time: hour
uint8_t DAILY_EVENT_END_MM = 10;    // event end time: minute

RTC_DS1307 rtc;
int myYear;
int myMon;
int myDay;
int myHour;
int myMin;
int mySec;
int count;

char daysOfTheWeek[7][12] = {
  "Sun",
  "Mon",
  "Tue",
  "Wed",
  "Thu",
  "Fri",
  "Sat"
};

int buzzer = 13;  //buzzer to arduino pin 13
int relay = 12;
void setup() {
  Serial.begin(9600);

  // SETUP RTC MODULE
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("Couldn't find RTC");
    while (1)
      ;
  }

  // sets the RTC to the date & time on PC this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // sets the RTC with an explicit date & time, for example to set
  // January 21, 2021 at 3am you would call:
  // rtc.adjust(DateTime(2021, 1, 21, 3, 0, 0));
  lcd.begin();  // If you are using more I2C devices using the Wire library use lcd.begin(false)
                // this stop the library(LCD_I2C) from calling Wire.begin()
  lcd.backlight();
  Wire.begin();
  delay(500);
  Serial.println("Nano Ready!");
  pinMode(buzzer, OUTPUT);  // Set buzzer - pin 9 as an output
  pinMode(relay, OUTPUT);   // Set buzzer - pin 9 as an output
  digitalWrite(relay, HIGH);
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

}

void loop() {

  DateTime now = rtc.now();
  if (now.hour() >= DAILY_EVENT_START_HH && now.minute() >= DAILY_EVENT_START_MM && now.hour() < DAILY_EVENT_END_HH && now.minute() < DAILY_EVENT_END_MM) {
    Serial.println("It is on scheduled time");
    // TODO: write your code"
  } else {
    Serial.println("It is NOT on scheduled time");
  }

  printTime(now);

  lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
  lcd.print(" ");
  lcd.print(myDay);
  lcd.print("/");
  lcd.print(myMon);
  lcd.print("/");
  lcd.print(myYear);

  lcd.setCursor(0, 1);  // Or setting the cursor in the desired position.
  lcd.print("Time:");
  lcd.print(myHour);
  lcd.print(":");
  lcd.print(myMin + 8);
  lcd.print(":");
  lcd.print(mySec);


  if ((myHour == 12) && (myMin == 7) && (mySec == 0)) {
    Serial.println("Alarm1");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("     ALARM 1    ");
    buzz4();
    lcd.clear();
  }

  if ((myHour == 7) && (myMin == 30) && (mySec == 1)) {
    Serial.println("Assembly");
  }


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
  test();
  close();
}

void printTime(DateTime time) {


  
  myMon = time.month();
  myYear = time.year();
  myDay = time.day();
  myHour = time.hour();
  myMin = time.minute();
  mySec = time.second();

  Serial.print(daysOfTheWeek[time.dayOfTheWeek()]);
  Serial.print(' ');
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
  delay(1000);
}

void JSsnack() {
  if ((myHour == 9) && (myMin == 55) && (mySec == 0)) {
    Serial.println("JS Snack Break");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("JS SNACK BREAK");
    buzz2();
    lcd.clear();
  }
}

void JSlunch() {
  if ((myHour == 12) && (myMin == 0) && (mySec == 0)) {
    Serial.println("JS Lunch Break");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("JS LUNCH BREAK");
    breakbuzz();
    lcd.clear();
  }
}

void SSlunch() {
  if ((myHour == 12) && (myMin == 55) && (mySec == 0)) {
    Serial.println("SS Lunch Break");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("JS LUNCH BREAK");
    breakbuzz();
    lcd.clear();
  }
}

void JSperiod1() {
  if ((myHour == 8) && (myMin == 10) && (mySec == 0)) {
    Serial.println("JS Period1");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("  JS Period 1   ");
    buzz1();
    lcd.clear();
  }

  if ((myHour == 8) && (myMin == 55) && (mySec == 0)) {
    Serial.println("JS Period1 round up");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("JS PERIOD RND UP");
    buzz2();
    lcd.clear();
  }

  if ((myHour == 9) && (myMin == 0) && (mySec == 0)) {
    Serial.println("JS Period1 Ends");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print(" JS PERIOD1 ENDS ");
    buzz4();
    lcd.clear();
  }
}

void JSperiod2() {
  if ((myHour == 9) && (myMin == 5) && (mySec == 0)) {
    Serial.println("JS Period2");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("  JS Period 2   ");
    buzz1();
    lcd.clear();
  }

  if ((myHour == 9) && (myMin == 50) && (mySec == 0)) {
    Serial.println("JS Period2 round up");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("JS PERIOD2 ENDIN");
    buzz2();
    lcd.clear();
  }

  if ((myHour == 9) && (myMin == 55) && (mySec == 0)) {
    Serial.println("JS Period2 Ends");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print(" JS PERIOD2 ENDS ");
    buzz4();
    lcd.clear();
  }
}

void JSperiod3() {
  if ((myHour == 10) && (myMin == 15) && (mySec == 0)) {
    Serial.println("JS Period3");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("  JS Period 3   ");
    buzz1();
    lcd.clear();
  }

  if ((myHour == 11) && (myMin == 0) && (mySec == 0)) {
    Serial.println("JS Period3 round up");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("JS PERIOD3 ENDIN");
    buzz2();
    lcd.clear();
  }

  if ((myHour == 11) && (myMin == 5) && (mySec == 0)) {
    Serial.println("JS Period3 Ends");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print(" JS PERIOD3 ENDS ");
    buzz4();
    lcd.clear();
  }
}

void JSperiod4() {
  if ((myHour == 11) && (myMin == 10) && (mySec == 0)) {
    Serial.println("JS Period4");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("  JS Period 4   ");
    buzz1();
    lcd.clear();
  }

  if ((myHour == 11) && (myMin == 55) && (mySec == 0)) {
    Serial.println("JS Period4 round up");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("JS PERIOD4 ENDIN");
    buzz2();
    lcd.clear();
  }
}

void JSperiod5() {
  if ((myHour == 12) && (myMin == 30) && (mySec == 0)) {
    Serial.println("JS Period5");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("  JS Period 5   ");
    buzz1();
    lcd.clear();
  }

  if ((myHour == 13) && (myMin == 15) && (mySec == 0)) {
    Serial.println("JS Period5 round up");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("JS PERIOD5 ENDIN");
    buzz2();
    lcd.clear();
  }

  if ((myHour == 13) && (myMin == 20) && (mySec == 0)) {
    Serial.println("JS Period5 Ends");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print(" JS PERIOD5 ENDS ");
    buzz4();
    lcd.clear();
  }
}

void JSperiod6() {
  if ((myHour == 13) && (myMin == 25) && (mySec == 0)) {
    Serial.println("JS Period6");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("  JS Period 6   ");
    buzz1();
    lcd.clear();
  }

  if ((myHour == 14) && (myMin == 10) && (mySec == 0)) {
    Serial.println("JS Period6 round up");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("JS PERIOD2 ENDIN");
    buzz2();
    lcd.clear();
  }

  if ((myHour == 14) && (myMin == 15) && (mySec == 0)) {
    Serial.println("JS Period6 Ends");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print(" JS PERIOD6 ENDS ");
    buzz4();
    lcd.clear();
  }
}

void JSperiod7() {
  if ((myHour == 14) && (myMin == 20) && (mySec == 0)) {
    Serial.println("JS Period7");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("  JS Period 7   ");
    buzz1();
    lcd.clear();
  }

  if ((myHour == 15) && (myMin == 5) && (mySec == 0)) {
    Serial.println("JS Period7 round up");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("JS PERIOD7 ENDIN");
    buzz2();
    lcd.clear();
  }
}

void SSperiod3() {
  if ((myHour == 10) && (myMin == 0) && (mySec == 0)) {
    Serial.println("SS Period3");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("  SS Period 3   ");
    buzz1();
    lcd.clear();
  }

  if ((myHour == 10) && (myMin == 45) && (mySec == 0)) {
    Serial.println("SS Period3 round up");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("SS PERIOD3 ENDIN");
    buzz2();
    lcd.clear();
  }

  if ((myHour == 10) && (myMin == 50) && (mySec == 0)) {
    Serial.println("SS Period3 Ends");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("SS PERIOD3 ENDS");
    buzz4();
    lcd.clear();
  }
}

void SSperiod5() {
  if ((myHour == 12) && (myMin == 5) && (mySec == 0)) {
    Serial.println("SS Period5");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("  SS Period 5   ");
    buzz1();
    lcd.clear();
  }

  if ((myHour == 12) && (myMin == 50) && (mySec == 0)) {
    Serial.println("SS Period5 round up");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("SS PERIOD5 ENDIN");
    buzz2();
    lcd.clear();
  }

  if ((myHour == 12) && (myMin == 55) && (mySec == 0)) {
    Serial.println("SS Period5 Ends");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("SS PERIOD5 ENDS ");
    buzz4();
    lcd.clear();
  }
}

void close() {
  if ((myHour == 15) && (myMin == 10) && (mySec == 0)) {
    Serial.println("JS Period7");
    lcd.clear();
    lcd.setCursor(0, 0);  // Or setting the cursor in the desired position.
    lcd.print("  CLOSING TIME  ");
    closebuzz();
    lcd.clear();
  }
}

void test() {
  if ((myHour == 7) && (myMin == 45) && (mySec == 0)) {
    Serial.println("JS Period7");
    buzz1();
  }

  if ((myHour == 7) && (myMin == 46) && (mySec == 0)) {
    Serial.println("JS Period7");
    breakbuzz();
  }
}

void buzz2() {
  for (count = 0; count < 3; count++) {
    digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
    digitalWrite(relay, LOW);   // Send 1KHz sound signal...
    delay(2000);                 // ...for 5 sec
    digitalWrite(buzzer, LOW);   // Stop sound...
    digitalWrite(relay, HIGH);    // Send 1KHz sound signal...
    delay(1000);                 // ...for 1sec
  }
}

void buzz1() {
  for (count = 0; count < 15; count++) {
    digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
    digitalWrite(relay, LOW);   // Send 1KHz sound signal...
    delay(300);                  // ...for 5 sec
    digitalWrite(buzzer, LOW);   // Stop sound...
    digitalWrite(relay, HIGH);    // Send 1KHz sound signal...
    delay(300);                  // ...for 1sec
  }
}

void buzz3() {
  digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
  digitalWrite(relay, LOW);   // Send 1KHz sound signal...
  delay(7000);                 // ...for 5 sec
  digitalWrite(buzzer, LOW);   // Stop sound...
  digitalWrite(relay, HIGH);    // Send 1KHz sound signal...
  // delay(126000);        // ...for 1sec
  // delay(60000);
  // buzz2();
}

void buzz4() {
  for (count = 0; count < 15; count++) {
    digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
    digitalWrite(relay, LOW);   // Send 1KHz sound signal...
    delay(500);                  // ...for 5 sec
    digitalWrite(buzzer, LOW);   // Stop sound...
    digitalWrite(relay, HIGH);    // Send 1KHz sound signal...
    delay(100);                  // ...for 1sec

    digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
    digitalWrite(relay, LOW);   // Send 1KHz sound signal...
    delay(500);                  // ...for 5 sec
    digitalWrite(buzzer, LOW);   // Stop sound...
    digitalWrite(relay, HIGH);    // Send 1KHz sound signal...
    delay(100);                  // ...for 1sec
  }
  delay(1000);
}

void breakbuzz() {
  for (count = 0; count < 10; count++) {
    digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
    digitalWrite(relay, LOW);   // Send 1KHz sound signal...
    delay(100);                  // ...for 5 sec
    digitalWrite(buzzer, LOW);   // Stop sound...
    digitalWrite(relay, HIGH);    // Send 1KHz sound signal...
    delay(100);
  }
  digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
  digitalWrite(relay, LOW);   // Send 1KHz sound signal...
  delay(2000);                 // ...for 5 sec
  digitalWrite(buzzer, LOW);   // Stop sound...
  digitalWrite(relay, HIGH);    // Send 1KHz sound signal...
}

void closebuzz() {
  for (count = 0; count < 7; count++) {
    digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
    digitalWrite(relay, LOW);   // Send 1KHz sound signal...
    delay(2000);                 // ...for 5 sec
    digitalWrite(buzzer, LOW);   // Stop sound...
    digitalWrite(relay, HIGH);    // Send 1KHz sound signal...
    delay(1000);
  }
  digitalWrite(buzzer, HIGH);  // Send 1KHz sound signal...
  digitalWrite(relay, LOW);   // Send 1KHz sound signal...
  delay(2000);                 // ...for 5 sec
  digitalWrite(buzzer, LOW);   // Stop sound...
  digitalWrite(relay, HIGH);   // Send 1KHz sound signal...
}