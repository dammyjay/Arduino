int red = 7;
int yellow = 8;
int green = 9;

void setup() {
  // put your setup code here, to run once:
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(red,HIGH);
  delay(5000);
  digitalWrite(red,LOW);
  digitalWrite(yellow,HIGH);
  delay(3000);
  digitalWrite(yellow,LOW);
  digitalWrite(green,HIGH);
  delay(7000);
  digitalWrite(green,LOW);
  digitalWrite(yellow,HIGH);
  delay(3000);
  digitalWrite(yellow,LOW);

}
