#include <RCSwitch.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

RCSwitch mySwitch = RCSwitch();

int led = 8;
float level;


void setup() {
  mySwitch.enableReceive(0);

pinMode (led, OUTPUT);
  Serial.begin(9600);
    lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.backlight();
  // Print a message to the LCD.
  lcd.setCursor(0,0);
  lcd.print(" SMART SECURITY ");
  lcd.setCursor(0,1);
  lcd.print("  STREET LIGHT   ");
  delay(4000);
  lcd.clear();
 
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("SECURE MODE");
  if (mySwitch.available()) {
    
    level = mySwitch.getReceivedValue();

    Serial.println(level);
  lcd.setCursor(0,0);
  lcd.print("SECURE MODE");

    lcd.setCursor(0, 1);
    lcd.print("LEVEL ");
    lcd.print(level);
    lcd.print(" %       ");

     if (level<5) {
      digitalWrite (led, HIGH);
      delay (11000);
    lcd.setCursor(0,0);
    lcd.print ("LIGHT ON  ");
   

          
    lcd.setCursor(0,0);
    lcd.print ("BOMB DETECTED       ");
    delay (3000);

    lcd.setCursor(0,0);
    lcd.print ("DIFFUSER ACTIVATED");
    delay (1000);

    lcd.setCursor(0,0);
    lcd.print ("BOMB DIFFUSING...... ");
    delay (5000);

    lcd.setCursor(0,0);
    lcd.print("BOMB DIFFUSED       ");
    delay (500);

    lcd.setCursor(0,0);
    lcd.print ("ENVIRONMENT SAFE     ");
    
    
}
    
  mySwitch.resetAvailable();
    
  }   
  

    
    else  {
    lcd.setCursor(0,0);
    lcd.print ("ENVIRONMENT SAFE     ");
    lcd.setCursor(0,1);
    lcd.print ("LIGHT OFF");
  }
}
