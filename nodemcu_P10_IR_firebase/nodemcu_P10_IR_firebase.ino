//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Test P10
/*
 * Original source code : https://github.com/busel7/DMDESP/blob/master/examples/TeksDiamdanJalan/TeksDiamdanJalan.ino by  busel7
 * Links to download libraries : https://github.com/busel7/DMDESP
*/

//----------------------------------------Include Library
//----------------------------------------see here: https://www.youtube.com/watch?v=8jMr94B8iN0 to add NodeMCU ESP8266 library and board
#include <DMDESP.h>
#include <fonts/ElektronMart6x8.h>
#include <fonts/Mono5x7.h>
//----------------------------------------

#include <Wire.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>
//-----------------------------------------------------------------------
//Provide the token generation process info.
#include <addons/TokenHelper.h>

//Provide the RTDB payload printing
//info and other helper functions.
#include <addons/RTDBHelper.h>

int timer1;
int timer2;

float Time;

int flag1 = 0;
int flag2 = 0;

float distance = 60;  // space beteen the 2 sensors in cm
float speed;

int ir_s1 = 2; //D4 on nodemcu
int ir_s2 = 5; //D1 on Nodemcu
char chr[5];

int buzzer = 3;

//----------------------------------------DMD Configuration (P10 Panel)
#define DISPLAYS_WIDE 2                     //--> Panel Columns
#define DISPLAYS_HIGH 1                     //--> Panel Rows
DMDESP Disp(DISPLAYS_WIDE, DISPLAYS_HIGH);  //--> Number of Panels P10 used (Column, Row)

// LiquidCrystal_I2C lcd(0x27, 16, 2);  // Default address of most PCF8574 modules, change according
//-----------------------------------------------------------------------
/* 1. Define the WiFi credentials */
// #define WIFI_SSID "ENTER_YOUR_WIFI_SSID"
// #define WIFI_PASSWORD "ENTER_YOUR_WIFI_PASSWORD"

// #define WIFI_SSID "Infinix HOT 6 Pro2"
// #define WIFI_PASSWORD "Dammy4real"


#define WIFI_SSID "TECNO POP 2 Power"
#define WIFI_PASSWORD "Akins010"
//-----------------------------------------------------------------------
/* 2. Define the API Key */
#define API_KEY "AIzaSyDXv-zqKSyZsE1Yd9pOeUf8yYr6S7D0n8E"
//-----------------------------------------------------------------------
/* 3. Define the RTDB URL */
#define DATABASE_URL "https://csd-app-e2a96-default-rtdb.firebaseio.com/"
//-----------------------------------------------------------------------
// String room_no = "room1";
String room_no = "Speed";
//-----------------------------------------------------------------------


/* Uncomment only if, you have selected the email authentication from 
firebase authentication settings */
//#define USER_EMAIL "ENTER_USER_EMAIL"
//#define DATABASE_URL "USER_PASSWORD"
//-----------------------------------------------------------------------

/***********************************************************
 0. complete_path = /room1/L1 or /room1/L2 etc.
    this is the complete path to firebase database
 1. stream_path = /room1
    this is the top nodes of firebase database
***********************************************************/
String stream_path = "";
/***********************************************************
 2. event_path = /L1 or /L2 or /L3 or /L4
    this is the data node in firbase database
***********************************************************/
String event_path = "";
/***********************************************************
 3. stream_data - use to store the current command to
    turn ON or OFF the relay 
***********************************************************/
String stream_data = "";
/***********************************************************
 5. jsonData - use to store "all the relay states" from 
    firebase database jsonData object used only once when 
    you reset the nodemcu or esp32 check the following:
    else if(event_path == "/") in the loop() function
***********************************************************/
FirebaseJson jsonData;

/***********************************************************
 it becomes TRUE when data is changed in Firebase
 used in streamCallback function
***********************************************************/
volatile bool dataChanged = false;

/***********************************************************
 this variable is based on the authentication method you 
 have selected while making firebase database project.
 authentication method: anonymus user
***********************************************************/
bool signupOK = false;

/***********************************************************
 resetPressed variable is used only once when you
 pressed the reset button. it is used to send test data to
 Firebase database. If we will not test send data then the 
 project will not work. used in the loop function.
***********************************************************/
bool resetPressed = true;

/***********************************************************
 when there is some data to upload to theFirebase
 then the value of uploadBucket will TRUE. This
 variable is used in listenSwitches() function
***********************************************************/
bool uploadBucket = false;

//i.e bucketData = "1" or "0"
//i.e bucketPath = "L1" or "L2" etc.
String bucketData = "", bucketPath = "";

//-----------------------------------------------------------------------
//Define Firebase Data object
FirebaseData stream;
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;


/******************************************************************************************
 * void streamCallback(FirebaseStream data)
 * This function execute automatically 
 * 1. when you press the reset button of the microcontroller. 
 * 2. when any data is changed in the firebase basebase.
 * microcontroller. 
 ******************************************************************************************/
void streamCallback(FirebaseStream data) {
  stream_path = data.streamPath().c_str();
  event_path = data.dataPath().c_str();

  if (String(data.dataType().c_str()) == "json") {
    jsonData = data.to<FirebaseJson>();
  } else {
    //intData(), floatData()
    stream_data = data.stringData();
  }
  Serial.printf("stream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
                stream_path,
                event_path,
                data.dataType().c_str(),
                data.eventType().c_str());
  printResult(data);  //see addons/RTDBHelper.h
  Serial.println();

  //This is the size of stream payload received (current and max value)
  //Max payload size is the payload size under the stream path since
  //the stream connected and read once and will not update until
  //stream reconnection takes place. This max value will be zero
  //as no payload received in case of ESP8266 which
  //BearSSL reserved Rx buffer size is less than the actual stream payload.
  Serial.printf("Received stream payload size: %d (Max. %d)\n\n",
                data.payloadLength(), data.maxPayloadLength());

  //Due to limited of stack memory, do not perform any task that
  //used large memory here especially starting connect to server.
  //Just set this flag and check it status later.
  dataChanged = true;
}

/******************************************************************************************
 * void streamTimeoutCallback(bool timeout)
 ******************************************************************************************/
void streamTimeoutCallback(bool timeout) {
  if (timeout)
    Serial.println("stream timed out, resuming...\n");

  if (!stream.httpConnected())
    Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(),
                  stream.errorReason().c_str());
}

static char* Text[] = { "CAR SPEED DETECTOR SPEED LIMIT : 40KM/HR" };

void Scrolling_Text(int y, uint8_t scrolling_speed) {
  static uint32_t pM;
  static uint32_t x;
  int width = Disp.width();
  Disp.setFont(Mono5x7);
  int fullScroll = Disp.textWidth(Text[0]) + width;
  if ((millis() - pM) > scrolling_speed) {
    pM = millis();
    if (x < fullScroll) {
      ++x;
    } else {
      x = 0;
      return;
    }
    Disp.drawText(width - x, y, Text[0]);
  }
}

void updateDisplay(const char* text) {
  // Disp.clearScreen();  // Clear the back buffer
  Disp.drawText(0, 0, text);  // Draw new content
  Disp.swapBuffers();         // Swap the buffers to display the new content
}

//----------------------------------------------------------------------
// SETUP

void setup() {

  //----------------------------------------DMDESP Setup
  Disp.start();  //--> Run the DMDESP library
  // Disp.setDoubleBuffer(true);
  Disp.setBrightness(50);  //--> Brightness level
  Disp.setFont(Mono5x7);   //--> Determine the font used

  //----------------------------------------

  Serial.begin(9600);
  pinMode(ir_s1, INPUT);
  pinMode(ir_s2, INPUT);
  pinMode(buzzer, OUTPUT);

  randomSeed(analogRead(0));
  // pinMode(WIFI_LED, OUTPUT);

  Serial.println("Car speed detector");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;  //see addons/TokenHelper.h
  //-----------------------------------------------------------------------
  /*Or Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase signUp ok");
    signupOK = true;

  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  //-----------------------------------------------------------------------

  /*Or Assign the user sign in credentials */
  //auth.user.email = USER_EMAIL;
  //auth.user.password = USER_PASSWORD;

  //Or use legacy authenticate method
  //config.database_url = DATABASE_URL;
  //config.signer.tokens.legacy_token = "<database secret>";
  //-----------------------------------------------------------------------
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
//-----------------------------------------------------------------------
//Recommend for ESP8266 stream, adjust the buffer size to match your stream data size
#if defined(ESP8266)
  stream.setBSSLBufferSize(2048 /* Rx in bytes, 512 - 16384 */, 512 /* Tx in bytes, 512 - 16384 */);
#endif
  //-----------------------------------------------------------------------
  if (!Firebase.RTDB.beginStream(&stream, room_no))
    Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());
  //-----------------------------------------------------------------------
  Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);
}



//----------------------------------------------------------------------
// LOOP

void loop() {

  Disp.loop();  //--> Run "Disp.loop" to refresh the LED
  // Disp.drawText(4, 0, "UTEH");  //--> Display text "Disp.drawText(x position, y position, text)"
  Scrolling_Text(9, 100);  //--> Show running text "Scrolling_Text(y position, speed);"
  Disp.drawText(32, 0, chr);
  // Disp.drawText(7, 0, time);
  // Serial.print("Sensor1 ");
  // Serial.print(digitalRead(ir_s1));

  // Serial.print(" Sensor2 ");
  // Serial.println(digitalRead(ir_s2));

  if (digitalRead(ir_s1) == 0 && flag1 == 0) {
    timer1 = millis();
    flag1 = 1;
  }

  if (digitalRead(ir_s2) == 0 && flag2 == 0) {
    timer2 = millis();
    flag2 = 1;
  }

  if (flag1 == 1 && flag2 == 1) {
    if (timer1 > timer2) { Time = timer1 - timer2; }  // Checking if timer 1 is greater than timer 2 so the time differnce will always be positive value
    else if (timer2 > timer1) {
      Time = timer2 - timer1;
    }                           // Checking if timer 2 is greater than timer 1 so the time differnce will always be positive value
    Time = Time / 1000;         //convert millisecond to second
    speed = (distance / Time);  //v=d/t
    speed = speed * 1000;       //multiply by seconds per hr
    speed = speed / 1000;       //division by meters per Km
  }

  if (speed == 0) {
    if (flag1 == 0 && flag2 == 0) {
      // Serial.println("No car  detected");
      Disp.loop();                   //--> Run "Disp.loop" to refresh the LED
      Disp.drawText(0, 0, "CSD||");  //--> Display text "Disp.drawText(x position, y position, text)"

    }

    else {
      Serial.println("Searching...    ");
      Disp.loop();                            //--> Run "Disp.loop" to refresh the LED
      Disp.drawText(0, 0, "SEARCHING.....");  //--> Display text "Disp.drawText(x position, y position, text)"
    }
  }

  else {

    Serial.print("Speed: ");
    Serial.print(speed, 3);
    Serial.println("Km/Hr  ");
    Serial.print("Time: ");
    Serial.print(Time);
    dtostrf(speed, 4, 2, chr);
    // dtostrf(Time, 4, 2, time);

    if (speed > 40) {
      dtostrf(speed, 4, 2, chr);
      Serial.print("  Over Speeding  ");
      Disp.loop();                    //--> Run "Disp.loop" to refresh the LED
      Disp.drawText(0, 0, "OVER");    //--> Display text "Disp.drawText(x position, y position, text)"
      Disp.drawText(25, 0, chr, 5);   //--> Display text "Disp.drawText(x position, y position, text)"
      Disp.drawText(56, 0, "km/Hr");  //--> Display text "Disp.drawText(x position, y position, text)"

      Serial.printf("Set Test String... %s\n\n", Firebase.RTDB.setString(&fbdo, "/test", chr) ? "ok" : fbdo.errorReason().c_str());
      digitalWrite(buzzer, HIGH);

    }

    else {
      Serial.print("  Normal Speed   ");
      Disp.loop();                    //--> Run "Disp.loop" to refresh the LED
      Disp.drawText(0, 0, "GOOD");    //--> Display text "Disp.drawText(x position, y position, text)"
      Disp.drawText(25, 0, chr, 5);   //--> Display text "Disp.drawText(x position, y position, text)"
      Disp.drawText(56, 0, "km/Hr");  //--> Display text "Disp.drawText(x position, y position, text)"
    }
    delay(3000);
    digitalWrite(buzzer, LOW);
    speed = 0;
    flag1 = 0;
    flag2 = 0;
  }

  int a = random(200);

  if (Firebase.ready() && signupOK) {
    if (uploadBucket == true) {
      uploadBucket = false;
      String URL = room_no + "/" + bucketPath;
      Serial.println(URL);
      Serial.printf("Set String... %s\n\n", Firebase.RTDB.setString(&fbdo, URL, bucketData) ? "ok" : fbdo.errorReason().c_str());
    }
    if (resetPressed == true) {
      resetPressed = false;
      Serial.printf("Set Test String... %s\n\n", Firebase.RTDB.setString(&fbdo, "/test", "Speed") ? "ok" : fbdo.errorReason().c_str());
    }
    bucketPath = "";
    bucketData = "";
  }
}
