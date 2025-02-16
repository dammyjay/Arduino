#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


int relay1= 8;
int relay2= 9;
int relay3= 10;

void setup() {
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print(" RELAY ENERGIZER ");
   lcd.setCursor(0,1);
  lcd.print(" GADGET CONTROL ");
  delay(4000);
  lcd.clear();
pinMode (relay1, OUTPUT);  
pinMode (relay2, OUTPUT);  
pinMode (relay3, OUTPUT);  
}

void loop() {
  digitalWrite (relay1, HIGH);
  lcd.setCursor (3,0);
  lcd.print ("1= ON ");
  delay (2000);
  digitalWrite (relay1,LOW);
  lcd.setCursor (3,0);
  lcd.print ("1= OFF");
  delay(300);

  digitalWrite (relay2, HIGH);
  lcd.setCursor (03,1);
  lcd.print ("2= ON ");
  delay (2000);
  digitalWrite (relay2,LOW);
  lcd.setCursor (3,1);
  lcd.print ("2= OFF");
  delay(300);

  digitalWrite (relay3, HIGH);
  lcd.setCursor (10,0);
  lcd.print ("3= ON ");
  delay (2000);
  digitalWrite (relay3,LOW);
  lcd.setCursor (10,0);
  lcd.print ("3= OFF");
  delay(300);

}
