/*
 * Modified by Rui Santos, http://randomnerdtutorialscom
 * based on IRremote Library - Ken Shirriff
*/
 
#include <IRremote.h>
 
int IR_Recv = 13;   //IR Receiver Pin 3
int fan = 12;

 
IRrecv irrecv(IR_Recv);
decode_results results;
 
void setup(){
  Serial.begin(9600);  //starts serial communication
  irrecv.enableIRIn(); // Starts the receiver
  pinMode(fan, OUTPUT);      // sets the digital pin as output
      // sets the digital pin as output 

}
 
void loop(){
  //decodes the infrared input
  if (irrecv.decode(&results)){
    long int decCode = results.value;
    Serial.println(results.value);
    //switch case to use the selected remote control button
    switch (results.value){
      case 551520375: //when you press the 1 button
        digitalWrite(fan, HIGH);
        break;   
      case 551495895: //when you press the 4 button
        digitalWrite(fan, LOW);   
        break;
       
    }
    irrecv.resume(); // Receives the next value from the button you press
  }
  delay(10);
}
