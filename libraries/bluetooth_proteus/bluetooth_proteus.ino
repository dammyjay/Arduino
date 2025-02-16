#include <LiquidCrystal.h>

const int rs = 11, en = 10, d4 = 9, d5 = 8, d6 = 7, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int output1= 13;
int output2= 12;
char command;

void setup() 
{
  Serial.begin(9600);  //Set the baud rate to your Bluetooth module.
  lcd.begin(20, 4);
  lcd.setCursor(0,0);
  lcd.print("Automatic Energy");
   lcd.setCursor(0,1);
  lcd.print(" Meter ");
  delay(2000);
 lcd.clear();
}

void loop(){
  if(Serial.available() > 0){ 
    command = Serial.read(); 
    //Change pin mode only if new command is different from previous.   
    //Serial.println(command);
    switch(command){
    case 'F':  
      Output1();
      break;
    case 'B':  
       Output2();
      break;
    }
  } 
}


void Output1()
{
  digitalWrite (output1, HIGH);
  delay (10000);
  digitalWrite (output1, LOW);
  lcd.setCursor (0,0);
  lcd.print ("OUTPUT1= ON"); 
  delay (10000);
  lcd.setCursor (0,0);
  lcd.print ("OUTPUT1= OFF");   
}


void Output2()
{
  digitalWrite (output2, HIGH);
  lcd.setCursor (0,0);
  lcd.print ("OUTPUT2= ON"); 
  delay (10000);
  
  digitalWrite (output2, LOW);
  lcd.setCursor (0,0);
  lcd.print ("OUTPUT2= OFF"); 
}
