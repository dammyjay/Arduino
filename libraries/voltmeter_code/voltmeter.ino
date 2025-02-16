/* code for making a DC digital voltmeter*/
#include <LiquidCrystal.h>
/* declaring the pin of the lcd port to the arduino port*/

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/* staiting the value type of  the input pin A0 */
int analogInput= A0;
float Vout= 0.00;
float Vin= 0.00;
float R1= 90000.00;/* the value of the first resistor used as the potential divider*/
float R2= 10000.00;/*the value of the first resistor used as the potential divider*/
int val= 0;


void setup() {
  pinMode (analogInput, INPUT);
  Serial.begin (9600);
   lcd.begin(16, 2);/* setting the number of column and the number of row*/
  lcd.setCursor (0,0);
 lcd.print ("FINAL YEAR     ");
 lcd.setCursor (0,1);
 lcd.print ("PROJECT ");
 delay (500);
 
 lcd.setCursor (0,0);
 lcd.print ("BATTERY CHARGER ");
 lcd.setCursor (0,1);
 lcd.print ("WITH TESTER  BY");
 delay (500);
 
 lcd.setCursor (0,0);
 lcd.print ("AYOMIDE ADENLE     ");
 lcd.setCursor (0,1);
 lcd.print ("F/ND/16/3420038    ");
 delay (500);

 lcd.setCursor (0,0);
 lcd.print ("LAWAL AHMED         ");
 lcd.setCursor (0,1);
 lcd.print ("F/ND/16/3420037    ");
 delay (500);
 
 lcd.setCursor (0,0);
 lcd.print ("SUPERVISED BY     ");
 lcd.setCursor (0,1);
 lcd.print ("ENGR.S.O OSOKOYA");
 delay (500);
 
 lcd.setCursor (0,0);
 lcd.print ("TESTER READY     ");
 lcd.setCursor (0,1);
 lcd.print ("                  ");
 delay (500);}

void loop() {
   int val= analogRead(analogInput);
   Vout= (val*5.0)/1024.0; /*converting the anlog value to the digital value*/
  Vin= Vout/(R2/(R2+R1)); /*calculating the value of the input voltage using the voltage divider*/

  if (Vin <0.09)/* checking if the voltage is less than zero or there is no input battery connected*/
  {Vin = 0.0;}
  /* the serial monitor should display the value of the battery voltage, 
  digital value of the battery voltage A0
  */
  Serial.print ("Vin =");
  Serial.print (Vin);
   Serial.print ("Vout =");
  Serial.print (Vout);
  Serial.print ("input =");
  Serial.print (val);
 /* displaying the battery bolage on LCD*/
  lcd.setCursor (0, 1);
  lcd.print ("voltage =");
  lcd.print (Vin); 
  lcd.setCursor (0, 0);
  lcd.print ("READING INPUT");
  

}

