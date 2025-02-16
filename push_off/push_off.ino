#include <Ultrasonic.h>

Ultrasonic ultrasonic(4, 3);
const int trigPin = 4;
const int echoPin = 3;
int m1a = 9;
int m1b = 10;
int m2a = 11;
int m2b = 12;
int en1 = 7;
int en2 = 8;
char val;
#define IR_sensor_front A0  // front sensor
#define IR_sensor_back A1   // rear senson

// defines variables
long duration;
int distance;

void setup() {
  while (Serial.available() > 0) {
    val = Serial.read();
    Serial.println(val);
  }
  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);   // Sets the echoPin as an Input
  pinMode(m1a, OUTPUT);      // Digital pin 9 set as output Pin
  pinMode(m1b, OUTPUT);      // Digital pin 10 set as output Pin
  pinMode(m2a, OUTPUT);      // Digital pin 11 set as output Pin
  pinMode(m2b, OUTPUT);      // Digital pin 12 set as output Pin
  Serial.begin(9600);
  // Starts the serial communication
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);
  stop();
  delay(2000);
}
void loop() {
  int IR_front = digitalRead(IR_sensor_front);
  int IR_back = digitalRead(IR_sensor_back);
  Serial.println("IR front :");
  Serial.println(IR_front);
  Serial.println("IR back :");
  Serial.println(IR_back);
  distance = ultrasonic.read();
  Serial.println(distance);


  if (distance < 70) {
    
    if ( IR_front == 1 || IR_back == 1 ) {
      forward();
      // break;

    }
    if ( IR_front == 0 || IR_back == 0 ) {
      stop();
      delay(500);
      backward();
      delay(1000);
      // break;

    }
    delay(10);
    

  } 
  
   

  else {
    left();
    delay(500);
    right();
    delay(500);
    
  }
}

void forward() {
  digitalWrite(m1a, HIGH);
  digitalWrite(m1b, LOW);
  digitalWrite(m2a, HIGH);
  digitalWrite(m2b, LOW);
  Serial.println("Moving forward");
  analogWrite(en1, 150);
  analogWrite(en2, 150);
}

void backward() {
  digitalWrite(m1a, LOW);
  digitalWrite(m1b, HIGH);
  digitalWrite(m2a, LOW);
  digitalWrite(m2b, HIGH);
  Serial.println("Moving backward");
  analogWrite(en1, 150);
  analogWrite(en2, 150);
}

void left() {
  digitalWrite(m1a, HIGH);
  digitalWrite(m1b, LOW);
  digitalWrite(m2a, HIGH);
  digitalWrite(m2b, LOW);
  Serial.println("Turning left");
  analogWrite(en1, 0);
  analogWrite(en2, 150);
}
void right() {
  digitalWrite(m1a, HIGH);
  digitalWrite(m1b, LOW);
  digitalWrite(m2a, HIGH);
  digitalWrite(m2b, LOW);
  Serial.println("Turning right");
  analogWrite(en1, 150);
  analogWrite(en2, 0);
}

void stop() {
  digitalWrite(m1a, LOW);
  digitalWrite(m1b, LOW);
  digitalWrite(m2a, LOW);
  digitalWrite(m2b, LOW);
  Serial.println("stop");
}