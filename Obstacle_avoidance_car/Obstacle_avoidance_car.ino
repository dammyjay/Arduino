#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);



#define TRIG_PIN A0
#define ECHO_PIN A1
#define MAX_DISTANCE 200
#define MAX_SPEED 255  // sets speed of DC  motors
#define MAX_SPEED_OFFSET 20
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);



//initial motors pin
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
Servo myservo;

int BUZZER = A2;
int LED = A3;
int distance = 100;

boolean goesForward = false;
int speedSet = 0;


char command;
void setup() {
  Serial.begin(9600);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  lcd.init();  // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" DRIVERLESS CAR  ");

  lcd.setCursor(0, 1);
  lcd.print("  READY ");  
  delay(2000);
  lcd.clear();
}

void loop() {
  // cleardigit();
  distance = readPing();
  lcd.setCursor(0, 0);
  lcd.print("DISTANCE: ");

  lcd.setCursor(11, 0);
  lcd.print(distance);  
  Serial.println(distance);
  if (distance >= 250) {
    // forward();
    motor1.setSpeed(MAX_SPEED);  //Define maximum velocity
    motor1.run(FORWARD);   //rotate the motor clockwise
    motor2.setSpeed(MAX_SPEED);  //Define maximum velocity
    motor2.run(FORWARD);   //rotate the motor clockwise
    motor3.setSpeed(MAX_SPEED);  //Define maximum velocity
    motor3.run(FORWARD);   //rotate the motor clockwise
    motor4.setSpeed(MAX_SPEED);  //Define maximum velocity
    motor4.run(FORWARD);   //rotate the motor clockwise
    Serial.println("HIGH FORWARD");
    clearrow();
    lcd.setCursor(0, 1);
    lcd.print(" MOVING FORWARD "); 
    digitalWrite(BUZZER, LOW);
  } 

  else if (distance <= 30) {
    Stop();
    Serial.println("STOP");
    digitalWrite(BUZZER, HIGH);
  }

  else if (distance < 150) {
    //forward();
    clearrow();
    lcd.setCursor(0, 1);
    lcd.print("  SLOWING DOWN  ");  
    motor1.setSpeed(200);  //Define maximum velocity
    motor1.run(FORWARD);   //rotate the motor clockwise
    motor2.setSpeed(200);  //Define maximum velocity
    motor2.run(FORWARD);   //rotate the motor clockwise
    motor3.setSpeed(200);  //Define maximum velocity
    motor3.run(FORWARD);   //rotate the motor clockwise
    motor4.setSpeed(200);  //Define maximum velocity
    motor4.run(FORWARD);   //rotate the motor clockwise
    Serial.println("FORWARD");
    digitalWrite(BUZZER, HIGH);
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(BUZZER, LOW);
    digitalWrite(LED, LOW);
    delay(100);  // wait for a second
  }

  


  else {
    forward();
    Serial.println("forward");
    digitalWrite(BUZZER, LOW);
  }
  
}

int readPing() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 250;
  }
  return cm;
}
void forward() {
  /*lcd.setCursor(0, 1);
  lcd.print("MOVING FORWARD");*/

  // motor1.setSpeed(255);  //Define maximum velocity
  // motor1.run(FORWARD);   //rotate the motor clockwise
  // motor2.setSpeed(255);  //Define maximum velocity
  // motor2.run(FORWARD);   //rotate the motor clockwise
  // motor3.setSpeed(255);  //Define maximum velocity
  // motor3.run(FORWARD);   //rotate the motor clockwise
  // motor4.setSpeed(255);  //Define maximum velocity
  // motor4.run(FORWARD);   //rotate the motor clockwise
  
  clearrow();
  lcd.setCursor(0, 1);
  lcd.print(" MOVING FORWARD ");
  motor1.setSpeed(MAX_SPEED);  //Define maximum velocity
  motor1.run(FORWARD);   //rotate the motor clockwise
  motor2.setSpeed(MAX_SPEED);  //Define maximum velocity
  motor2.run(FORWARD);   //rotate the motor clockwise
  motor3.setSpeed(MAX_SPEED);  //Define maximum velocity
  motor3.run(FORWARD);   //rotate the motor clockwise
  motor4.setSpeed(MAX_SPEED);  //Define maximum velocity
  motor4.run(FORWARD);   //rotate the motor clockwise
}

void back() {
  /* lcd.setCursor(0, 1);
  lcd.print("MOVING BACKWARD");*/

  // motor1.setSpeed(255);  //Define maximum velocity
  // motor1.run(BACKWARD);  //rotate the motor anti-clockwise
  // motor2.setSpeed(255);  //Define maximum velocity
  // motor2.run(BACKWARD);  //rotate the motor anti-clockwise
  // motor3.setSpeed(255);  //Define maximum velocity
  // motor3.run(BACKWARD);  //rotate the motor anti-clockwise
  // motor4.setSpeed(255);  //Define maximum velocity
  // motor4.run(BACKWARD);  //rotate the motor anti-clockwise

  clearrow();
  lcd.setCursor(0, 1);
  lcd.print("MOVING BACKWARD");
  motor1.setSpeed(MAX_SPEED);  //Define maximum velocity
  motor1.run(BACKWARD);  //rotate the motor anti-clockwise
  motor2.setSpeed(MAX_SPEED);  //Define maximum velocity
  motor2.run(BACKWARD);  //rotate the motor anti-clockwise
  motor3.setSpeed(MAX_SPEED);  //Define maximum velocity
  motor3.run(BACKWARD);  //rotate the motor anti-clockwise
  motor4.setSpeed(MAX_SPEED);  //Define maximum velocity
  motor4.run(BACKWARD);  //rotate the motor anti-clockwise
}

void left() {
  /*lcd.setCursor(0, 1);
  lcd.print("TURNING LEFT");*/

  // motor1.setSpeed(255);  //Define maximum velocity
  // motor1.run(FORWARD);   //rotate the motor anti-clockwise
  // motor2.setSpeed(255);  //Define maximum velocity
  // motor2.run(FORWARD);   //rotate the motor anti-clockwise
  // motor3.setSpeed(0);  //Define maximum velocity
  // motor3.run(RELEASE);   //rotate the motor clockwise
  // motor4.setSpeed(0);  //Define maximum velocity
  // motor4.run(RELEASE);   //rotate the motor clockwise

  clearrow();
  lcd.setCursor(0, 1);
  lcd.print("  TURNING LEFT  ");
  motor1.setSpeed(MAX_SPEED);  //Define maximum velocity
  motor1.run(FORWARD);   //rotate the motor anti-clockwise
  motor2.setSpeed(MAX_SPEED);  //Define maximum velocity
  motor2.run(FORWARD);   //rotate the motor anti-clockwise
  motor3.setSpeed(0);  //Define maximum velocity
  motor3.run(RELEASE);   //rotate the motor clockwise
  motor4.setSpeed(0);  //Define maximum velocity
  motor4.run(RELEASE);   //rotate the motor clockwise
}

void right() {
  /*lcd.setCursor(0, 1);
  lcd.print("TURNING RIGHT");*/

  // motor1.setSpeed(0);  //Define maximum velocity
  // motor1.run(RELEASE);   //rotate the motor clockwise
  // motor2.setSpeed(0);  //Define maximum velocity
  // motor2.run(RELEASE);   //rotate the motor clockwise
  // motor3.setSpeed(255);  //Define maximum velocity
  // motor3.run(FORWARD);   //rotate the motor anti-clockwise
  // motor4.setSpeed(255);  //Define maximum velocity
  // motor4.run(FORWARD);   //rotate the motor anti-clockwise

  clearrow();
  lcd.setCursor(0, 1);
  lcd.print(" TURNING RIGHT  ");
  motor1.setSpeed(0);  //Define maximum velocity
  motor1.run(RELEASE);   //rotate the motor clockwise
  motor2.setSpeed(0);  //Define maximum velocity
  motor2.run(RELEASE);   //rotate the motor clockwise
  motor3.setSpeed(MAX_SPEED);  //Define maximum velocity
  motor3.run(FORWARD);   //rotate the motor anti-clockwise
  motor4.setSpeed(MAX_SPEED);  //Define maximum velocity
  motor4.run(FORWARD);   //rotate the motor anti-clockwise
}

void Stop() {
  lcd.setCursor(0, 1);
  lcd.print("MOTOR STOPPED");

  motor1.setSpeed(0);   //Define minimum velocity
  motor1.run(RELEASE);  //stop the motor when release the button
  motor2.setSpeed(0);   //Define minimum velocity
  motor2.run(RELEASE);  //rotate the motor clockwise
  motor3.setSpeed(0);   //Define minimum velocity
  motor3.run(RELEASE);  //stop the motor when release the button
  motor4.setSpeed(0);   //Define minimum velocity
  motor4.run(RELEASE);  //stop the motor when release the button
}

void clearrow(){
  lcd.setCursor(0, 1);
  lcd.print("                "); 
}

void cleardigit(){
  lcd.setCursor(11, 0);
  lcd.print("         "); 
  delay(200);
}
