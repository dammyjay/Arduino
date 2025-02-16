int relay_1 = 8;
int relay_2 = 5;
int relay_3 = 6;
int relay_4 = 7;
char app;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Bluetooth Controlled Device");
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  pinMode(relay_3, OUTPUT);
  pinMode(relay_4, OUTPUT);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0){
    app = Serial.read();
    switch(app){
      case '1':
        digitalWrite(relay_1, HIGH);
        break;
      case '2':
        digitalWrite(relay_2, HIGH);
        break;
      case '3':
        digitalWrite(relay_3, HIGH);
        break;
      case '4':
        digitalWrite(relay_4, HIGH);
        break;
    }
  }

}
