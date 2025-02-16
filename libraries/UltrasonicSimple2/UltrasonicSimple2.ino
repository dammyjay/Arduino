
#include <Ultrasonic.h>
Ultrasonic ultrasonic(12, 13);
int distance;
int buzzer = 8

void setup() {
  Serial.begin(9600);
  pinMode(buzzer , OUTPUT);  
}

void loop() {
  // Pass INC as a parameter to get the distance in inches
  distance = ultrasonic.read();
  Serial.print("Distance in CM: ");
  Serial.println(distance);
  delay(500);

  if (distance < 70) {
    digitalWrite (buzzer, HIGH);    
  }

  else {
    digitalWrite (buzzer, LOW);
  }
}
