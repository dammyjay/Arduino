char command;

// left motor
int enA = 4;
int leftMotorForwardPin = 5;
int leftMotorBackwardPin = 6;

// right motor
int enB = 10;
int rightMotorForwardPin = 8;
int rightMotorBackwardPin = 9;


void setup() {
  pinMode(enA, OUTPUT);
  pinMode(leftMotorForwardPin, OUTPUT);
  pinMode(leftMotorBackwardPin, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(rightMotorForwardPin, OUTPUT);
  pinMode(rightMotorBackwardPin, OUTPUT);

  Serial.begin(9600);

}

void loop() {

  if (Serial.available() > 0) {
    command = Serial.read();
    Stop(); //initialize with motors stoped
    //Change pin mode only if new command is different from previous.
    //Serial.println(command);
    switch (command) {
      case 'A':
        goForward();
        Serial.println("Forward");
        Serial.println (digitalRead(5));
        Serial.println (digitalRead(6));
        Serial.println (digitalRead(8));
        Serial.println (digitalRead(9));
        break;
      case 'B':
        goBackward();
        Serial.println("Reverse");
        Serial.println (digitalRead(5));
        Serial.println (digitalRead(6));
        Serial.println (digitalRead(8));
        Serial.println (digitalRead(9));
        break;
      case 'C':
        goLeft();
        Serial.println("Left");
        Serial.println (digitalRead(5));
        Serial.println (digitalRead(6));
        Serial.println (digitalRead(8));
        Serial.println (digitalRead(9));
        break;
      case 'D':
        goRight();
        Serial.println("Right");
        Serial.println (digitalRead(5));
        Serial.println (digitalRead(6));
        Serial.println (digitalRead(8));
        Serial.println (digitalRead(9));
        break;
    }
  }
  
}

void goForward() {
  analogWrite(enA, 200);
  analogWrite(enB, 200);

  digitalWrite(leftMotorForwardPin, HIGH);
  digitalWrite(leftMotorBackwardPin, LOW);
  digitalWrite(rightMotorForwardPin, HIGH);
  digitalWrite(rightMotorBackwardPin, LOW);
}

void goBackward() {
  analogWrite(enA, 200);
  analogWrite(enB, 200);
  digitalWrite(leftMotorForwardPin, LOW);
  digitalWrite(leftMotorBackwardPin, HIGH);
  digitalWrite(rightMotorForwardPin, LOW);
  digitalWrite(rightMotorBackwardPin, HIGH);
}

void Stop() {
  digitalWrite(leftMotorForwardPin, LOW);
  digitalWrite(leftMotorBackwardPin, LOW);
  digitalWrite(rightMotorForwardPin, LOW);
  digitalWrite(rightMotorBackwardPin, LOW);
}

void goRight() {
  digitalWrite(leftMotorForwardPin, HIGH);
  digitalWrite(leftMotorBackwardPin, LOW);
  digitalWrite(rightMotorForwardPin, LOW);
  digitalWrite(rightMotorBackwardPin, LOW);
}

void goLeft() {
  digitalWrite(leftMotorForwardPin, LOW);
  digitalWrite(leftMotorBackwardPin, LOW);
  digitalWrite(rightMotorForwardPin, HIGH);
  digitalWrite(rightMotorBackwardPin, LOW);
}
