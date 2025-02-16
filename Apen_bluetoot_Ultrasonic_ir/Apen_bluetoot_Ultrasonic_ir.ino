//Bluetooth Controlled Car
#include <AFMotor.h>
#include <Ultrasonic.h>


Ultrasonic ultrasonic(9, 8);
//initial motors pin
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);
int speed = 255;
int speed2 = 150;
char command;
int distance;
int i;
int light = 10;
int buzzer = 9;

void setup() {

  Serial.begin(9600);  //Set the baud rate to your Bluetooth module.
  pinMode(light, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  delay(200);
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  delay(200);
}

void loop() {

  distance = ultrasonic.read();
  Serial.println(distance);

  while (Serial.available() > 0) {
    command = Serial.read();
    // Stop(); //initialize with motors stoped
    //Change pin mode only if new command is different from previous.
    Serial.println(command);
    // if(distance > 10){
    switch (command) {
      case 'F':
        forward();
        Serial.println("Forward");
        break;
      case 'B':
        Serial.println("Backward");
        back();
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
      case 'p':
        Serial.println("Spin");
        spin();
        break;
      case 'i':
        Serial.println("Zig Zag");
        zigzag();
        break;
      case 'r':
        Serial.println("Spiral");
        spiral();
        break;
      case 'b':
        Serial.println("bounce");
        bounce();
        break;
      case 'c':
        Serial.println("chacha");
        chacha();
        break;
      case 'a':
        Serial.println("wave");
        wave();
        break;
      case 'f':
        Serial.println("figure8");
        figure8();
        break;
      case 'd':
        Serial.println("circle dance");
        circledance();
        break;
    }


    // }

    // else{
    //   stop();
    // }
  }

  if (command == 'W') {
    Serial.println("Lamp On");
    lampOn();
  }

  if (command == 'w') {
    Serial.println("Lamp Off");
    lampOff();
  }

  if (command == 'V') {
    Serial.println("Horn On");
    hornOn();
  }

  if (command == 'v') {
    Serial.println("Horn Off");
    hornOff();
  }

  if (command == '1') {
    speed = 25;
    Serial.println(speed);
  }

  if (command == '2') {
    speed = 50;
    Serial.println(speed);
  }

  if (command == '3') {
    speed = 75;
    Serial.println(speed);
  }

  if (command == '4') {
    speed = 100;
    Serial.println(speed);
  }

  if (command == '5') {
    speed = 125;
    Serial.println(speed);
  }

  if (command == '6') {
    speed = 150;
    Serial.println(speed);
  }

  if (command == '7') {
    speed = 175;
    Serial.println(speed);
  }

  if (command == '8') {
    speed = 200;
    Serial.println(speed);
  }

  if (command == '9') {
    speed = 225;
    Serial.println(speed);
  }

  if (command == '0') {
    speed = 250;
    Serial.println(speed);
  }
}

void forward() {
  // if (distance < 20) {
  //   motor1.setSpeed(0);   //Define minimum velocity
  //   motor1.run(RELEASE);  //stop the motor when release the button
  //   motor2.setSpeed(0);   //Define minimum velocity
  //   motor2.run(RELEASE);  //rotate the motor clockwise
  //   motor3.setSpeed(0);   //Define minimum velocity
  //   motor3.run(RELEASE);  //stop the motor when release the button
  //   motor4.setSpeed(0);   //Define minimum velocity
  //   motor4.run(RELEASE);  //stop the motor when release the button
  // }

  // else {
    motor1.setSpeed(speed);  //Define maximum velocity
    motor1.run(FORWARD);     //rotate the motor clockwise
    motor2.setSpeed(speed);  //Define maximum velocity
    motor2.run(FORWARD);     //rotate the motor clockwise
    motor3.setSpeed(speed);  //Define maximum velocity
    motor3.run(FORWARD);     //rotate the motor clockwise
    motor4.setSpeed(speed);  //Define maximum velocity
    motor4.run(FORWARD);     //rotate the motor clockwise
  // }
}

void back() {
  motor1.setSpeed(speed);  //Define maximum velocity
  motor1.run(BACKWARD);    //rotate the motor anti-clockwise
  motor2.setSpeed(speed);  //Define maximum velocity
  motor2.run(BACKWARD);    //rotate the motor anti-clockwise
  motor3.setSpeed(speed);  //Define maximum velocity
  motor3.run(BACKWARD);    //rotate the motor anti-clockwise
  motor4.setSpeed(speed);  //Define maximum velocity
  motor4.run(BACKWARD);    //rotate the motor anti-clockwise
}

void left() {
  // if (distance < 20) {
  //   motor1.setSpeed(0);   //Define minimum velocity
  //   motor1.run(RELEASE);  //stop the motor when release the button
  //   motor2.setSpeed(0);   //Define minimum velocity
  //   motor2.run(RELEASE);  //rotate the motor clockwise
  //   motor3.setSpeed(0);   //Define minimum velocity
  //   motor3.run(RELEASE);  //stop the motor when release the button
  //   motor4.setSpeed(0);   //Define minimum velocity
  //   motor4.run(RELEASE);  //stop the motor when release the button
  // }

  // else {
    motor1.setSpeed(speed2);  //Define maximum velocity
    motor1.run(RELEASE);      //rotate the motor anti-clockwise
    motor2.setSpeed(speed2);  //Define maximum velocity
    motor2.run(RELEASE);      //rotate the motor anti-clockwise
    motor3.setSpeed(speed2);  //Define maximum velocity
    motor3.run(FORWARD);      //rotate the motor clockwise
    motor4.setSpeed(speed2);  //Define maximum velocity
    motor4.run(FORWARD);      //rotate the motor clockwise
  // }
}

void right() {
  // if (distance < 20) {
  //   motor1.setSpeed(0);   //Define minimum velocity
  //   motor1.run(RELEASE);  //stop the motor when release the button
  //   motor2.setSpeed(0);   //Define minimum velocity
  //   motor2.run(RELEASE);  //rotate the motor clockwise
  //   motor3.setSpeed(0);   //Define minimum velocity
  //   motor3.run(RELEASE);  //stop the motor when release the button
  //   motor4.setSpeed(0);   //Define minimum velocity
  //   motor4.run(RELEASE);  //stop the motor when release the button
  // }

  // else {
    motor1.setSpeed(speed2);  //Define maximum velocity
    motor1.run(FORWARD);      //rotate the motor clockwise
    motor2.setSpeed(speed2);  //Define maximum velocity
    motor2.run(FORWARD);      //rotate the motor clockwise
    motor3.setSpeed(speed2);  //Define maximum velocity
    motor3.run(RELEASE);      //rotate the motor anti-clockwise
    motor4.setSpeed(speed2);  //Define maximum velocity
    motor4.run(RELEASE);      //rotate the motor anti-clockwise
  // }
}

void stop() {
  motor1.setSpeed(0);   //Define minimum velocity
  motor1.run(RELEASE);  //stop the motor when release the button
  motor2.setSpeed(0);   //Define minimum velocity
  motor2.run(RELEASE);  //rotate the motor clockwise
  motor3.setSpeed(0);   //Define minimum velocity
  motor3.run(RELEASE);  //stop the motor when release the button
  motor4.setSpeed(0);   //Define minimum velocity
  motor4.run(RELEASE);  //stop the motor when release the button
}

void spin() {
  for (i = 0; i < 2; i++) {
    forward();
    delay(1000);
    left();
    delay(500);
    back();
    delay(1000);
    right();
    delay(500);
    stop();
  }
}

void figure8() {
  forward();
  delay(2000);
  left();
  delay(1000);
  back();
  delay(2000);
  right();
  delay(1000);
  stop();
}

void zigzag() {
  forward();
  delay(1000);
  left();
  delay(500);
  forward();
  delay(1000);
  right();
  delay(500);
  stop();
}

//function for circledance animation
void circledance() {
  forward();
  delay(2000);
  left();
  delay(2000);
  back();
  delay(2000);
  right();
  delay(2000);
  stop();
}

//function for spiral animation
void spiral() {
  forward();
  delay(1000);
  left();
  delay(200);
  forward();
  delay(1000);
  left();
  delay(200);
  stop();
}

//function for bounce animation
void bounce() {
  forward();
  delay(1000);
  back();
  delay(1000);
  forward();
  delay(1000);
  back();
  delay(1000);
  stop();
}

//function for wave animation
void wave() {
  forward();
  delay(1000);
  left();
  delay(200);
  forward();
  delay(1000);
  right();
  delay(200);
  stop();
}

//function for chacha animation
void chacha() {
  forward();
  delay(1000);
  left();
  delay(500);
  back();
  delay(1000);
  right();
  delay(500);
  stop();
}

void lampOn() {
  digitalWrite(light, HIGH);
  delay(200);
  digitalWrite(light, LOW);
  delay(200);
}

void lampOff() {
  digitalWrite(light, LOW);
}

void hornOn() {
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  delay(400);
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  delay(1000);
}

void hornOff() {
  digitalWrite(light, LOW);
}
