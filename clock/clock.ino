/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-ds1307-rtc-module
 */

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <RTClib.h>

// event from 13:50 to 14:10
uint8_t DAILY_EVENT_START_HH = 13;  // event start time: hour
uint8_t DAILY_EVENT_START_MM = 50;  // event start time: minute
uint8_t DAILY_EVENT_END_HH = 14;    // event end time: hour
uint8_t DAILY_EVENT_END_MM = 10;    // event end time: minute

RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {
  "Sun",
  "Mon",
  "Tue",
  "Wed",
  "Thu",
  "Fri",
  "Sat"
};

void setup() {
  Serial.begin(9600);

  // SETUP RTC MODULE
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }

  // sets the RTC to the date & time on PC this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // sets the RTC with an explicit date & time, for example to set
  // January 21, 2021 at 3am you would call:
  // rtc.adjust(DateTime(2021, 1, 21, 3, 0, 0));
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
}

void printTime(DateTime time) {

  int myYear;
  int myMon;
  int myDay;
  int myHour;
  int myMin;
  int mySec;


  // Serial.print("TIME: ");
  // Serial.print(time.year(), DEC);
  // Serial.print('/');
  // Serial.print(time.month(), DEC);
  // Serial.print('/');
  // Serial.print(time.day(), DEC);
  // Serial.print(" (");
  // Serial.print(daysOfTheWeek[time.dayOfTheWeek()]);
  // Serial.print(") ");
  // Serial.print(time.hour(), DEC);
  // Serial.print(':');
  // Serial.print(time.minute(), DEC);
  // Serial.print(':');
  // Serial.println(time.second(), DEC);

  
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
