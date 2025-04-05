//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Test P10
/*
 * Original source code : https://github.com/busel7/DMDESP/blob/master/examples/TeksDiamdanJalan/TeksDiamdanJalan.ino by  busel7
 * Links to download libraries : https://github.com/busel7/DMDESP
*/

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




// LiquidCrystal_I2C lcd(0x27, 16, 2);  // Default address of most PCF8574 modules, change according
//-----------------------------------------------------------------------
/* 1. Define the WiFi credentials */
// #define WIFI_SSID "ENTER_YOUR_WIFI_SSID"
// #define WIFI_PASSWORD "ENTER_YOUR_WIFI_PASSWORD"

// #define WIFI_SSID "Infinix HOT 6 Pro2"
// #define WIFI_PASSWORD "Dammy4real"


#define WIFI_SSID "Infinix HOT 6 Pro"
#define WIFI_PASSWORD "Dammy4real"
//----------------------------------------------------------------------
// #define WIFI_SSID "TECNO POP 2 Power"
// #define WIFI_PASSWORD "Akins010"
//-----------------------------------------------------------------------
/* 2. Define the API Key */
// #define API_KEY "AIzaSyDXv-zqKSyZsE1Yd9pOeUf8yYr6S7D0n8E"
// //-----------------------------------------------------------------------
// /* 3. Define the RTDB URL */
// #define DATABASE_URL "https://csd-app-e2a96-default-rtdb.firebaseio.com/"
// //-----------------------------------------------------------------------

/* 2. Define the API Key */
#define API_KEY "AIzaSyDHc1dMzCUhuvHbI9_0TV_3V13grhI4Sz4"
//-----------------------------------------------------------------------
/* 3. Define the RTDB URL */
#define DATABASE_URL "https://zucali-default-rtdb.firebaseio.com/"
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


//----------------------------------------------------------------------
// SETUP

void setup() {


  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);

  // randomSeed(analogRead(0));
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
  int a = random(100);
  
  Serial.printf("Set Test String... %s\n\n", Firebase.RTDB.setString(&fbdo, "/test", a) ? "ok" : fbdo.errorReason().c_str());
  digitalWrite(buzzer, HIGH);




  // int a = random(200);

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
