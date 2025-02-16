#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 8

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);
float Celcius = 0;
float Fahrenheit = 0;

const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int RawInput = 0;
float Vin = 0.00;
double voltage = 0;


int batt = A1;
int Relay = 3;
int Relay1 = 4;
int Relay2 = 5;
int Relay3 = 6;

float R1 = 390000;
float R2 = 10000;

void setup() {
  lcd.begin(20, 4);
  Serial.begin(9600);
  sensors.begin();

  pinMode (batt, INPUT);
  pinMode (Relay, OUTPUT);
  pinMode (Relay1, OUTPUT);
  pinMode (Relay2, OUTPUT);
  pinMode (Relay3, OUTPUT);



  lcd.setCursor (0, 0);
  lcd.print ("TEMPERATURE CONTROL");
  lcd.setCursor (0, 1);
  lcd.print ("BLADELESS FAN");
  delay (2000);
  lcd.clear();

  lcd.setCursor (0, 0);
  lcd.print ("SYSTEM ACTIVE");

  digitalWrite(Relay, LOW);
  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);
  digitalWrite(Relay3, LOW);
}

void loop() {
  RawInput = analogRead(batt);
  voltage = (RawInput / 1024) * 5;
  Vin = voltage / (R2 / (R1 + R2));

  lcd.setCursor (0, 1);
  lcd.print ("V=" + String(Vin , 1));

 sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);
  Serial.print(" C  ");
  Serial.print(Celcius);
  Serial.print(" F  ");
  Serial.println(Fahrenheit);
  delay(1000);

  lcd.setCursor (6, 1);
  lcd.print ("T " + String(Celcius, 1) + " C    ");

  if (Celcius < 23) {
    digitalWrite (Relay, HIGH);
    lcd.setCursor (6, 1);
    lcd.print ("T " + String(Celcius, 1) + " C   ");
  }

  else if (23 < Celcius < 27) {
    digitalWrite (Relay, HIGH);
    lcd.setCursor (6, 1);
    lcd.print ("T " + String(Celcius, 1) + " C   ");
  }

  else if (27 < Celcius < 30) {
    digitalWrite (Relay, HIGH);
    lcd.setCursor (6, 1);
    lcd.print ("T " + String(Celcius, 1) + " C   ");
  }

  else if (Celcius > 30) {
    digitalWrite (Relay, HIGH);
    lcd.setCursor (6, 1);
    lcd.print ("T " + String(Celcius, 1) + " C   ");
  }

  else {
    digitalWrite(Relay, LOW);
    digitalWrite(Relay1, LOW);
    digitalWrite(Relay2, LOW);
    digitalWrite(Relay3, LOW);
  }
}
