#include <Adafruit_Fingerprint.h>
#include <Keypad.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(12, 11);
#else
#define mySerial Serial1
#endif

#define relay 10
#define nodemcu 13
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int buzzer = 13;
int process = 0;
int numberpressed = 0;
int IDNumber;
String fingerID = "";
String num = "";
uint8_t id;
const byte ROWS = 4;  //four rows
const byte COLS = 4;  //four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};


char customKey;                        //Where the keys are stored it changes very often
char code[] = { '1', '2', '2', '3' };  //The default code, you can change it or make it a 'n' digits one

char code_buff1[sizeof(code)];  //Where the new key is stored - sizeof function gives the size of the character array
char code_buff2[sizeof(code)];  //Where the new key is stored again so it's compared to the previous one

byte rowPins[ROWS] = { 5, 4, 3, 2 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 8, 7, 6, 9 };  //connect to the column pinouts of the keypad

int a = 0, i = 0, s = 0, j = 0;  //Variables used later
int access = 0;

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("  FINGERPRINT  ");
  lcd.setCursor(0, 1);
  lcd.print("     BASED     ");
  delay(2000);

  lcd.setCursor(0, 0);
  lcd.print("     DOOR      ");
  lcd.setCursor(0, 1);
  lcd.print("     LOCK      ");
  delay(2000);
  lcd.clear();
  pinMode(relay, OUTPUT);
  pinMode(nodemcu, OUTPUT);
  digitalWrite(relay, HIGH);

  // while (!Serial)
  //   ;  // For Yun/Leo/Micro/Zero/...
  // delay(100);
  // Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

  // // set the data rate for the sensor serial port
  // finger.begin(57600);

  // if (finger.verifyPassword()) {
  //   Serial.println("Found fingerprint sensor!");
  //   lcd.setCursor(0, 0);
  //   lcd.print("    I.M.E     ");
  //   lcd.setCursor(0, 1);
  //   lcd.print("   WELCOME     ");
  //   delay(2000);
  // } else {
  //   Serial.println("Did not find fingerprint sensor :(");
  //   lcd.setCursor(0, 0);
  //   lcd.print(" FINGER SENSOR  ");
  //   lcd.setCursor(0, 1);
  //   lcd.print("  NOT FOUND   ");
  //   delay(2000);
  //   while (1) {
  //     delay(1);
  //   }
  // }

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
}

uint8_t readnumber(void) {
  //uint8_t num = 0;

  int num = numberpressed;
  Serial.println(num);
  if (num) {
    //num = Serial.parseInt();
    Serial.println(num);
    while (!getFingerprintEnroll())
      ;
  }
}

void loop()  // run over and over again
{

  char customKey = customKeypad.getKey();
  if (customKey) {
    process = 0;
    /**if (customKey == '0') {
      numberpressed = 0;
      }**/
    if (customKey == '1') {
      numberpressed = 1;
      Serial.println("1");
    }
    if (customKey == '2') {
      numberpressed = 2;
      //Serial.println("2");
    }
    if (customKey == '3') {
      numberpressed = 3;
      //Serial.println("3");
    }
    if (customKey == '4') {
      numberpressed = 4;
      //Serial.println("4");
    }
    if (customKey == '5') {
      numberpressed = 5;
      //Serial.println("5");
    }
    if (customKey == '6') {
      numberpressed = 6;
      //Serial.println("6");
    }
    if (customKey == '7') {
      numberpressed = 7;
      //Serial.println("7");
    }
    if (customKey == '8') {
      numberpressed = 8;
      //Serial.println("8");
    }
    if (customKey == '9') {
      numberpressed = 9;
      //Serial.println("0");
    }

    // if (customKey == '*') {

    //   // Serial.println("Ready to enroll a fingerprint!");
    //   // Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
    //   lcd.setCursor(0, 1);
    //   lcd.print("ENTER FINGER ID");
    //   id = readnumber();

    //   if (id == 0) {  // ID #0 not allowed, try again!
    //     return;
    //   }
    // }

    if (customKey == '#') {
      // getFingerprintID();
      delay(50);
      //Serial.println("0");
    }
  }

  if (customKey == 'A') {
    lcd.clear();
  }

  if (customKey == 'C') {  // * to type the code

    Serial.print(F("Enter code: "));
    lcd.clear();  //Use it to clear the LCD Widget
    // lcd.print(0, 0, "ENTER CODE");
    lcd.setCursor(0, 1);
    lcd.print("ENTER CODE");
    //Message to show
    GetCode();  //Getting code function
    yield();
    if (a == sizeof(code)) {  //The GetCode function assign a value to a (it's correct when it has the size of the code array)
      OpenDoor();
      access = 0;
      Serial.println(F("Access Granted"));
      lcd.clear();  //Use it to clear the LCD Widget
      lcd.setCursor(0, 1);
      lcd.print("ACCESS GRANTED");
      // digitalWrite(relay, LOW);
      digitalWrite(nodemcu, HIGH);
      Serial.println(digitalRead(nodemcu));
      delay(4000);
      // digitalWrite(relay, HIGH);
      digitalWrite(nodemcu, LOW);
      Serial.println(digitalRead(nodemcu));


      yield();  //Open lock function if code is correct
    } else {
      access = access + 1;
      Serial.println(F("Access Denied"));
      lcd.clear();  //Use it to clear the LCD Widget
      lcd.setCursor(0, 1);
      lcd.print("ACCESS DENIED  ");

      if (access == 3) {
        lcd.setCursor(0, 1);
        lcd.print("MULTI ATTEMPT");
        access = 0;
      }
    }
    yield();
    delay(2000);
    Serial.println(F("Standby"));  //Return to standby mode it's the message do display when waiting
    lcd.clear();                   //Use it to clear the LCD Widget
    // lcd.print(4, 0, "STAND BY");
    lcd.setCursor(0, 1);
    lcd.print("STAND BY");
    yield();
  }

  // if (customKey == 'D') {  //To change the code it calls the changecode function
  //   ChangeCode();
  //   Serial.println(F("Standby"));
  //   lcd.clear();  //Use it to clear the LCD Widget
  //   // lcd.print(4, 0, "STAND BY");
  //   lcd.setCursor(0, 1);
  //   lcd.print("STAND BY");
  //   yield();  //When done it returns to standby mode
  // }
}
uint8_t getFingerprintEnroll() {
  int num = numberpressed;
  int p = -1;
  Serial.print(F("Waiting for valid finger to enroll as #"));
  Serial.println(num);
  lcd.setCursor(0, 0);
  lcd.print("WAITING FOR A");
  lcd.setCursor(0, 1);
  lcd.print(" VALID FINGER  ");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println(F("Image taken"));
        lcd.setCursor(0, 0);
        lcd.print("  IMAGE TAKEN   ");
        lcd.setCursor(0, 1);
        lcd.print("                ");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println(F("Communication error"));
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println(F("Imaging error"));
        break;
      default:
        Serial.println(F("Unknown error"));
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image converted"));
      lcd.setCursor(0, 0);
      lcd.print("IMAGE CONVERTED ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println(F("Image too messy"));
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    default:
      Serial.println(F("Unknown error"));
      return p;
  }

  Serial.println(F("Remove finger"));
  lcd.setCursor(0, 0);
  lcd.print(" REMOVE FINGER  ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID ");
  Serial.println(num);
  p = -1;
  Serial.println(F("Place same finger again"));
  lcd.setCursor(0, 0);
  lcd.print("   PLACE SAME   ");
  lcd.setCursor(0, 1);
  lcd.print("  FINGER AGAIN  ");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println(F("Image taken"));
        lcd.setCursor(0, 0);
        lcd.print("  IMAGE TAKEN   ");
        lcd.setCursor(0, 1);
        lcd.print("                ");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(F("."));
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println(F("Communication error"));
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println(F("Imaging error"));
        break;
      default:
        Serial.println(F("Unknown error"));
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image converted"));
      lcd.setCursor(0, 0);
      lcd.print(" IMAGE CONVERTED");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println(F("Image too messy"));
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    default:
      Serial.println(F("Unknown error"));
      return p;
  }

  // OK converted!
  Serial.print(F("Creating model for #"));
  Serial.println(num);
  lcd.setCursor(0, 0);
  lcd.print(" CREATING MODEL ");
  lcd.setCursor(0, 1);
  lcd.print("  FOR ID");
  lcd.print(num);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println(F("Prints matched!"));
    lcd.setCursor(0, 0);
    lcd.print("  PRINTS MATCHED ");
    lcd.setCursor(0, 1);
    lcd.print("                 ");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println(F("Communication error"));
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println(F("Fingerprints did not match"));

    return p;
  } else {
    Serial.println(F("Unknown error"));
    return p;
  }

  Serial.print(F("ID "));
  Serial.println(num);
  p = finger.storeModel(num);
  if (p == FINGERPRINT_OK) {
    Serial.println(F("Stored!"));
    lcd.setCursor(0, 0);
    lcd.print("     STORED     ");
    lcd.setCursor(0, 1);
    lcd.print("               ");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println(F("Communication error"));
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println(F("Could not store in that location"));
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println(F("Error writing to flash"));
    return p;
  } else {
    Serial.println(F("Unknown error"));
    return p;
  }

  return true;
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image taken"));
      lcd.setCursor(0, 0);
      lcd.print("  IMAGE TAKEN  ");
      lcd.setCursor(0, 1);
      lcd.print("               ");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(F("No finger detected"));
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println(F("Imaging error"));
      return p;
    default:
      Serial.println(F("Unknown error"));
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image converted"));
      lcd.setCursor(0, 0);
      lcd.print(" IMAGE CONVERTED   ");
      lcd.setCursor(0, 1);
      lcd.print("               ");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println(F("Image too messy"));
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    default:
      Serial.println(F("Unknown error"));
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println(F("Found a print match!"));
    lcd.setCursor(0, 0);
    lcd.print("    FOUND A     ");
    lcd.setCursor(0, 1);
    lcd.print(" PRINT MATCH   ");
    Serial.println(F("ACcess granted, Door opened"));
    lcd.setCursor(0, 0);
    lcd.print(" ACCESS GRANTED ");
    delay(500);
    digitalWrite(relay, LOW);
    // digitalWrite(nodemcu, HIGH);
    delay(5000);
    digitalWrite(relay, HIGH);
    digitalWrite(nodemcu, LOW);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println(F("Communication error"));
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println(F("Did not find a match"));
    lcd.setCursor(0, 0);
    lcd.print(" DID NOT FIND   ");
    lcd.setCursor(0, 1);
    lcd.print("   A MATCH     ");
    return p;
  } else {
    Serial.println(F("Unknown error"));
    return p;
  }

  // found a match!
  Serial.print(F("Found ID #"));
  Serial.print(finger.fingerID);
  lcd.setCursor(0, 0);
  lcd.print("   FOUND ID");
  lcd.print(finger.fingerID);
  lcd.setCursor(13, 0);
  lcd.print("      ");
  lcd.setCursor(0, 1);
  lcd.print("               ");
  Serial.print(" with confidence of ");
  Serial.println((finger.confidence));

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;

  // found a match!
  Serial.print(F("Found ID #"));
  Serial.print((finger.fingerID));
  lcd.setCursor(0, 0);
  lcd.print("FOUND ID");
  lcd.print(finger.fingerID);
  lcd.setCursor(0, 1);
  lcd.print("               ");
  Serial.print(F(" with confidence of "));
  Serial.println(finger.confidence);
  return finger.fingerID;
}

void GetCode() {  //Getting code sequence
  i = 0;          //All variables set to 0
  a = 0;

  while (customKey != '0') {  //The user press A to confirm the code otherwise he can keep typing
    customKey = customKeypad.getKey();
    if (customKey != NO_KEY && customKey != '0') {  //If the char typed isn't A and neither "nothing"

      Serial.print(customKey);
      //Use it to clear the LCD Widget
      // lcd.print(i, 1, "*");
      lcd.setCursor(i, 1);
      lcd.print(customKey);

      if (customKey == code[i] && i < sizeof(code)) {  //if the char typed is correct a and i increments to verify the next caracter
        a++;
        i++;
      } else
        a--;  //if the character typed is wrong a decrements and cannot equal the size of code []
    }
    yield();
  }
  Serial.println();
  customKey = NO_KEY;
  yield();
}

void ChangeCode() {  //Change code sequence
  Serial.println(F("Changing code"));
  lcd.clear();  //Use it to clear the LCD Widget
  // lcd.print(0, 0, "CHANGING CODE");
  lcd.setCursor(0, 0);
  lcd.print("CHANGING CODE");
  delay(1000);

  Serial.print(F("Enter old code:"));
  lcd.clear();  //Use it to clear the LCD Widget
  // lcd.print(0, 0, "ENTER OLD CODE");
  lcd.setCursor(0, 0);
  lcd.print("ENTER OLD CODE");
  GetCode();  //verify the old code first so you can change it

  if (a == sizeof(code)) {  //again verifying the a value

    Serial.println(F("Changing code"));
    lcd.clear();  //Use it to clear the LCD Widget
    // lcd.print(0, 0, "CHANGING CODE");
    lcd.setCursor(0, 1);
    lcd.print("CHANGING CODE");
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
      Serial.println(F("Code Changed"));
      lcd.clear();  //Use it to clear the LCD Widget
      // lcd.print(0, 0, " CODE CHANGED ");
      lcd.setCursor(0, 1);
      lcd.print("CODE CHANGED");
      lcd.setCursor(0, 0);
      lcd.print(code);
      // SendMail("Code Changed Successfully","The passcode for the Secure facility was changed successfully.");
      delay(2000);
      yield();
    } else {
      //In case the new codes aren't matching
      Serial.println(F("Codes are not matching"));
      lcd.clear();  //Use it to clear the LCD Widget
      // lcd.print(0, 0, "CODES NOT MATCH");
      lcd.setCursor(0, 1);
      lcd.print("CODE NOT MATCH");
      delay(2000);
      // lcd.setCursor(0, 2);
      yield();
    }

  }

  else {  //In case the old code is wrong you can't change it
    // SendMail("Unauthorized access of Secure facility" ,"Level 1 breach. Code change attempt denied.");
    Serial.println("Wrong");
    lcd.clear();  //Use it to clear the LCD Widget
    // lcd.print(0, 0, "   WRONG     ");
    lcd.setCursor(0, 1);
    lcd.print("    WRONG      ");
    delay(2000);
  }
  yield();
}

void GetNewCode1() {
  i = 0;
  j = 0;

  Serial.println("Enter new code and press A");  //tell the user to enter the new code and press A
  lcd.clear();                                   //Use it to clear the LCD Widget
  // lcd.print(0, 0, " ENTER NEW CODE ");
  lcd.setCursor(0, 1);
  lcd.print("ENTER NEW CODE");
  delay(2000);


  while (customKey != '0') {  //A to confirm and quits the loop
    customKey = customKeypad.getKey();
    if (customKey != NO_KEY && customKey != '0') {
      Serial.print(customKey);
      //Use it to clear the LCD Widget
      // lcd.print(0, 1, "*");
      lcd.setCursor(0, 1);
      lcd.print(customKey);
      code_buff1[i] = customKey;  //Store caracters in the array
      i++;
      j++;
    }
    yield();
  }
  Serial.println();
  customKey = NO_KEY;
}

void GetNewCode2() {  //This is exactly like the GetNewCode1 function but this time the code is stored in another array
  i = 0;
  j = 0;

  Serial.println("Confirm code and press A");
  lcd.clear();  //Use it to clear the LCD Widget
  // lcd.print(0, 0, "CONFIRM CODE & A");
  lcd.setCursor(0, 1);
  lcd.print("0 TO CONFIRM");

  while (customKey != '0') {
    customKey = customKeypad.getKey();
    if (customKey != NO_KEY && customKey != '0') {

      Serial.print(customKey);
      //Use it to clear the LCD Widget
      // lcd.print(0, 1, "*");
      lcd.setCursor(0, 1);
      lcd.print(customKey);
      code_buff2[i] = customKey;
      i++;
      j++;
    }
    yield();
  }
  Serial.println();
  customKey = NO_KEY;
  yield();
}

void OpenDoor() {
  Serial.println("Access Granted");
  lcd.setCursor(0, 1);
  lcd.print("ACCESS GRANTED");
  // SendMail2();
}