#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
#include <AFMotor.h>  
#include <NewPing.h>
#include <Servo.h> 
#include <LiquidCrystal_I2C.h>

int motor1Pin1 = 3;
int motor1Pin2 = 4;
int motor2Pin1 = 5;
int motor2Pin2 = 6;
int enable1Pin = 8;
int enable2Pin = 9;
int ir_right = 12;
int ir_left = 13;
int TRIG_PIN = A0 ;
int ECHO_PIN = A1; 

int left_sensor;
int right_sensor;
int BUZZER= A2;
int LED= A3;

int distance = 100;
char command; 

void setup() {
  Serial.begin(57600);
  mySerial.begin(57600);
  
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
  pinMode(ir_right, INPUT);
  pinMode(ir_left, INPUT);
  pinMode (BUZZER, OUTPUT);
  pinMode (LED, OUTPUT);
}

void loop() {
  // Serial.println (distance);
  // left_sensor = digitalRead(ir_left);
  // right_sensor = digitalRead(ir_right);

  // Serial.print("Left sensor");
  // Serial.println(left_sensor);
  // delay(300);

  // Serial.print("Rightsensor");
  // Serial.println(right_sensor);

  if(Serial.available() > 0){ 
    command = Serial.read(); 
    stop(); //initialize with motors stoped
    //Change pin mode only if new command is different from previous.   
    //Serial.println(command);
    switch(command){
      case 'F':  
        Serial.println("Forward");
        forward();
        break;
      case 'B':   
        Serial.println("Backward");
        backward();
        break;
      case 'L':  
        Serial.println("Left");
        left();
        break;
      case 'R':
        Serial.println("Right");
        right();
        break;
      case 'S':
        Serial.println("Stop");
        stop();
        break;
      case 'W':
        analogWrite(BUZZER,HIGH); 
        break;
      case 'w':
        analogWrite(BUZZER,LOW); 
        break;
      case 'V':
        analogWrite(LED,HIGH); 
        break;
      case 'v':
        analogWrite(LED,LOW); 
        break;

    }
  }
  
  

  // find();

  // if(distance < 20 ){
  //   forward();
    
  //   if((left_sensor == LOW) || (right_sensor ==LOW) ){
  //   backward();
  // }
  // }

  // if((left_sensor == LOW) || (right_sensor ==LOW) ){
  //   backward();
  // }

  // else{
  //   forward();
  // }
}

void forward() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  analogWrite(enable1Pin, 255);
      
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  analogWrite(enable2Pin, 255);
}

void backward(){
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  analogWrite(enable1Pin, 255);
      
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  analogWrite(enable2Pin, 255);
}

void left(){
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  analogWrite(enable1Pin, 255);
      
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  analogWrite(enable2Pin, 255);
}

void right(){
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  analogWrite(enable1Pin, 255);
      
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  analogWrite(enable2Pin, 255);
}

void stop(){
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  analogWrite(enable1Pin, 255);
      
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  analogWrite(enable2Pin, 255);
}

// void find() {
//   distance = readPing();
//   forward();
//   delay(3000;)
//   stop();
//   right();
//   delay(1000)
// }

// void Ping(){
//    int readPing() { 
//     delay(70);
//     int cm = sonar.ping_cm();
//     if(cm==0)
//     {
//       cm = 250;
//     }
//     return cm;
//   }
// } 
