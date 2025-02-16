
#include <Ultrasonic.h>

Ultrasonic ultrasonic(4, 3);

const int IN1 = 9;
const int IN2 = 10;
const int IN3 = 11;
const int IN4 = 12;
int en1 = 7;
int en2 = 8;
#define IR_sensor_front A0 // front sensor
#define IR_sensor_back A1 // rear senson
int speed = 200;
int distance ;

void setup()
{
  Serial.begin(9600);
  delay (100); // as per sumo compat roles
}
void loop(){
  
  int IR_front = analogRead(IR_sensor_front);
  int IR_back = analogRead(IR_sensor_back);
  distance = ultrasonic.read();
  Serial.println(distance);
  ROTATE(); // start rotete if (distance < 20){
  Stop();
  while (distance < 70 ) {
    FORWARD();
    distance = ultrasonic.read();
    // IR_front = analogRead(IR_sensor_front);
    // IR_back = analogRead(IR_sensor_back);
    if ( IR_front > 650 || IR_back > 650 ) {
      break;
    }
    delay(10);
  }

  if (distance >= 70){
    ROTATE();
    delay(500);
    Stop();
    delay(300);
  }
  if (IR_front < 650 )  // < 650 means white line
  {
    Stop();
    delay (50);
    BACKWARD();
    delay (500);
  }

  if (IR_back < 650 )  //
  {
    Stop();
    delay (50);
    FORWARD();
    delay (500);
  }

  // /----------- debugging ---------------- 
  Serial.print(distance);
  Serial.println("cm");
  Serial.println("IR front :");
  Serial.println(IR_front);
  Serial.println("IR back :");
  Serial.println(IR_back);


} //--------------------------------------------
void BACKWARD () {
  Serial.println("Backward");
  analogWrite(en1, 200);
  analogWrite(en2, 200);
  //When we want to let Motor To move forward,
  // just void this part on the loop section .
  analogWrite(IN1, 200);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 200);
}//--------------------------------------------
void FORWARD () {
  Serial.println("Forward");
  analogWrite(en1, 200);
  analogWrite(en2, 200);
  //When we want to let Motor To move forward,
  // just void this part on the loop section .
  analogWrite(IN1, 0);
  analogWrite(IN2, 200);
  analogWrite(IN3, 200);
  analogWrite(IN4, 0);
}//--------------------------------------------
void ROTATE ()
{
  Serial.println("Rotate");
  analogWrite(en1, 200);
  analogWrite(en2, 200);
  //When we want to let Motor To Rotate ,
  // just void this part on the loop section .
  analogWrite(IN1, 200);
  analogWrite(IN2, 0);
  analogWrite(IN3, 200);
  analogWrite(IN4, 0);
}//--------------------------------------------
void Stop() {
  //When we want to  Motor To stop ,
  // just void this part on the loop section .
  Serial.println("Stop");
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}
