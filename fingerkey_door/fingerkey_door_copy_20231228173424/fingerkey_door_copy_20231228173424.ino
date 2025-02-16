
/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLosEXdQG7"
#define BLYNK_DEVICE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN "LP3TDVWuQWTJjH-FEYC0XPNNgGMm5C4c"
#include <LiquidCrystal_I2C.h>


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Keypad.h>
#include <EEPROM.h>

#include "Gsender.h"

#pragma region Globals

#include <Adafruit_Fingerprint.h>


#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..
SoftwareSerial mySerial(9, 10); // (Tx node, Rx node)Tx of node to rx of sensor , Rx of node Tx of sensor

#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial1

#endif


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

char auth[] = "3KfaUs77vhUox4W1PoN_xSl66V-SE93c";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Infinix HOT 6 Pro";
char pass[] = "Dammy4real3";
// const char* ssid = "TP-Link_7D05";                           // WIFI network name
// const char* password = "123ABC___";                       // WIFI network password
uint8_t connection_state = 0;         // Connected to WIFI or not
uint16_t reconnect_interval = 10000;  // If not connected wait time to try again
#pragma endregion Globals

BlynkTimer timer;
#define Relay 15  //D8
#define Buzzer 10
WidgetLCD lcd(V7);

//Relay State

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0) {
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  if (value == 1) {
    digitalWrite(Relay, LOW);
  }

  // Update state
  Blynk.virtualWrite(V1, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED() {
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent() {
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}

uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr) {
  static uint16_t attempt = 0;
  Serial.print("Connecting to ");
  if (nSSID) {
    WiFi.begin(nSSID, nPassword);
    Serial.println(nSSID);
  } else {
    WiFi.begin(ssid, pass);
    Serial.println(ssid);
  }

  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 50) {
    delay(200);
    Serial.print(".");
  }
  ++attempt;
  Serial.println("");
  if (i == 51) {
    Serial.print("Connection: TIMEOUT on attempt: ");
    Serial.println(attempt);
    if (attempt % 2 == 0)
      Serial.println("Check if access point available or SSID and Password\r\n");
    return false;
  }
  Serial.println("Connection: ESTABLISHED");
  Serial.print("Got IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

void Awaits() {
  uint32_t ts = millis();
  while (!connection_state) {
    delay(50);
    if (millis() > (ts + reconnect_interval) && !connection_state) {
      connection_state = WiFiConnect();
      ts = millis();
    }
  }
}


//Keymap for 4x4 Keypad
const byte numRows = 4;  //number of rows on the keypad
const byte numCols = 4;  //number of columns on the keypad

char keymap[numRows][numCols] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

char keypressed;                       //Where the keys are stored it changes very often
char code[] = { '7', '8', '9', '9' };  //The default code, you can change it or make it a 'n' digits one

char code_buff1[sizeof(code)];  //Where the new key is stored - sizeof function gives the size of the character array
char code_buff2[sizeof(code)];  //Where the new key is stored again so it's compared to the previous one

int a = 0, i = 0, s = 0, j = 0;  //Variables used later

byte rowPins[numRows] = { 16, 5, 4, 0 };    //Rows 0 to 3 //if you modify your pins you should modify this too
byte colPins[numCols] = { 2, 14, 12, 13 };  //Columns 0 to 3

int access = 0;

 



void setup() {
  // Debug console
  Serial.begin(115200);
  pinMode(Relay, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  Blynk.begin(auth, ssid, pass);

  finger.begin(57600);

    // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  lcd.clear();                         //Use it to clear the LCD Widget
  lcd.print(0, 0, "FINGERPRINT KEY");  // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
  lcd.print(0, 1, "  ACCESS DOOR  ");
  lcd.clear();

  lcd.print(0, 0, "B = FINGERPRINT");
  lcd.print(0, 1, "* ENTER # CHANGE");

  digitalWrite(Buzzer, HIGH);
  delay(500);
  digitalWrite(Buzzer, LOW);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x"));
  Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x"));
  Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: "));
  Serial.println(finger.capacity);
  Serial.print(F("Security level: "));
  Serial.println(finger.security_level);
  Serial.print(F("Device address: "));
  Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: "));
  Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: "));
  Serial.println(finger.baud_rate);


  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
  connection_state = WiFiConnect();
  if (!connection_state)  // if not connected to WIFI
    Awaits();             // constantly trying to connect

  Serial.println("Standby");
  yield();
}

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (!Serial.available())
      ;
    num = Serial.parseInt();
  }
  return num;
}

void loop() {
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  keypressed = myKeypad.getKey();
  yield();                  //Constantly waiting for a key to be pressed
  if (keypressed == '*') {  // * to type the code
    digitalWrite(Buzzer, HIGH);
    Serial.print("Enter code: ");
    lcd.clear();  //Use it to clear the LCD Widget
    lcd.print(0, 0, "ENTER CODE");
    //Message to show
    GetCode();  //Getting code function
    yield();
    if (a == sizeof(code)) {  //The GetCode function assign a value to a (it's correct when it has the size of the code array)
      OpenDoor();
      access = 0;
      Serial.println("Access Granted");
      lcd.clear();  //Use it to clear the LCD Widget
      lcd.print(0, 0, "ACCESS GRANTED");
      // SendMail("authorized access of Secure facility","Someone has entered facility now. \n Code was entered incorrectly three times. \n Secure facility immediately.");//Message to print when the code is wrong
      Blynk.virtualWrite(V4, "650");

      yield();  //Open lock function if code is correct
    } else {
      access = access + 1;
      Serial.println("Access Denied");
      lcd.clear();  //Use it to clear the LCD Widget
      lcd.print(0, 0, "ACCESS DENIED");

      if (access == 3) {
        // SendMail("Unauthorized access of Secure facility","Someone tried to access facility. \n Code was entered incorrectly three times. \n Secure facility immediately.");//Message to print when the code is wrong
        Blynk.virtualWrite(V4, "511");
        access = 0;
      }
    }
    yield();
    delay(2000);
    Serial.println("Standby");  //Return to standby mode it's the message do display when waiting
    lcd.clear();                //Use it to clear the LCD Widget
    lcd.print(4, 0, "STAND BY");
    yield();
  }

  if (keypressed == '#') {  //To change the code it calls the changecode function
    ChangeCode();
    Serial.println("Standby");
    lcd.clear();  //Use it to clear the LCD Widget
    lcd.print(4, 0, "STAND BY");
    yield();  //When done it returns to standby mode
  }
  yield();

  if (keypressed == 'B') {
    Serial.println("Ready to enroll a fingerprint!");
    Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
    id = readnumber();
    if (id == 0) {  // ID #0 not allowed, try again!
      return;
    }
    Serial.print("Enrolling ID #");
    Serial.println(id);

    while (!getFingerprintEnroll())
      ;
  }

  if (keypressed == 'C') {
  }

  if (keypressed == 'D') {
  }
}
void GetCode() {  //Getting code sequence
  i = 0;          //All variables set to 0
  a = 0;

  while (keypressed != 'A') {  //The user press A to confirm the code otherwise he can keep typing
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'A') {  //If the char typed isn't A and neither "nothing"

      Serial.print(keypressed);
      //Use it to clear the LCD Widget
      lcd.print(i, 1, "*");

      if (keypressed == code[i] && i < sizeof(code)) {  //if the char typed is correct a and i increments to verify the next caracter
        a++;
        i++;
      } else
        a--;  //if the character typed is wrong a decrements and cannot equal the size of code []
    }
    yield();
  }
  Serial.println();
  keypressed = NO_KEY;
  yield();
}

void ChangeCode() {  //Change code sequence
  Serial.println("Changing code");
  lcd.clear();  //Use it to clear the LCD Widget
  lcd.print(0, 0, "CHANGING CODE");
  delay(1000);

  Serial.print("Enter old code:");
  lcd.clear();  //Use it to clear the LCD Widget
  lcd.print(0, 0, "ENTER OLD CODE");
  GetCode();  //verify the old code first so you can change it

  if (a == sizeof(code)) {  //again verifying the a value

    Serial.println("Changing code");
    lcd.clear();  //Use it to clear the LCD Widget
    lcd.print(0, 0, "CHANGING CODE");
    GetNewCode1();  //Get the new code
    yield();
    GetNewCode2();
    yield();  //Get the new code again to confirm it
    s = 0;
    for (i = 0; i < sizeof(code); i++) {  //Compare codes in array 1 and array 2 from two previous functions
      if (code_buff1[i] == code_buff2[i])
        s++;  //again this how we verifiy, increment s whenever codes are matching
    }
    if (s == sizeof(code)) {  //Correct is always the size of the array

      for (i = 0; i < sizeof(code); i++) {
        code[i] = code_buff2[i];  //the code array now receives the new code
        EEPROM.put(i, code[i]);   //And stores it in the EEPROM
      }
      Serial.println("Code Changed");
      lcd.clear();  //Use it to clear the LCD Widget
      lcd.print(0, 0, " CODE CHANGED ");
      // SendMail("Code Changed Successfully","The passcode for the Secure facility was changed successfully.");
      delay(2000);
      yield();
    } else {
      //In case the new codes aren't matching
      Serial.println("Codes are not matching");
      lcd.clear();  //Use it to clear the LCD Widget
      lcd.print(0, 0, "CODES NOT MATCH");
      delay(2000);
      yield();
    }

  }

  else {  //In case the old code is wrong you can't change it
    // SendMail("Unauthorized access of Secure facility" ,"Level 1 breach. Code change attempt denied.");
    Serial.println("Wrong");
    lcd.clear();  //Use it to clear the LCD Widget
    lcd.print(0, 0, "   WRONG     ");
    delay(2000);
  }
  yield();
}

void GetNewCode1() {
  i = 0;
  j = 0;

  Serial.println("Enter new code and press A");  //tell the user to enter the new code and press A
  lcd.clear();                                   //Use it to clear the LCD Widget
  lcd.print(0, 0, " ENTER NEW CODE ");
  delay(2000);


  while (keypressed != 'A') {  //A to confirm and quits the loop
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'A') {
      Serial.print(keypressed);
      //Use it to clear the LCD Widget
      lcd.print(0, 1, "*");
      code_buff1[i] = keypressed;  //Store caracters in the array
      i++;
      j++;
    }
    yield();
  }
  Serial.println();
  keypressed = NO_KEY;
}

void GetNewCode2() {  //This is exactly like the GetNewCode1 function but this time the code is stored in another array
  i = 0;
  j = 0;

  Serial.println("Confirm code and press A");
  lcd.clear();  //Use it to clear the LCD Widget
  lcd.print(0, 0, "CONFIRM CODE & A");

  while (keypressed != 'A') {
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'A') {

      Serial.print(keypressed);
      //Use it to clear the LCD Widget
      lcd.print(0, 1, "*");
      code_buff2[i] = keypressed;
      i++;
      j++;
    }
    yield();
  }
  Serial.println();
  keypressed = NO_KEY;
  yield();
}

void OpenDoor() {
  Serial.println("Access Granted");
  // SendMail2();
}
void SendMail(String subject, String message) {


  Gsender* gsender = Gsender::Instance();  // Getting pointer to class instance
  if (gsender->Subject(subject)->Send("dammykirchhoff2@gmail.com", message)) {
    Serial.println("Message send.");
  } else {
    Serial.print("Error sending message: ");
    Serial.println(gsender->getError());
  }
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #");
  Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID ");
  Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");
  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID ");
  Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return true;
}
