/**********************************************************************************
 *  TITLE: Telegram PIR Motion Camera with DHT11 using ESP32-CAM
 *  Click on the following links to learn more. 
 *  YouTube Video: https://youtu.be/dP5xG7NcoDE
 *  Related Blog : https://iotcircuithub.com/esp32-projects/
 *  
 *  This code is provided free for project purpose and fair use only.
 *  Please do mail us to techstudycell@gmail.com if you want to use it commercially.
 *  Copyrighted © by Tech StudyCell
 *  
 *  Preferences--> Aditional boards Manager URLs : 
 *  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json
 *  
 *  Download Board ESP32 (2.0.5) : https://github.com/espressif/arduino-esp32
 *
 *  Download the libraries 
 *  UniversalTelegramBot Library (1.3.0):  https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
 *  ArduinoJson Library (6.20.0): https://arduinojson.org/?utm_source=meta&utm_medium=library.properties
 *  DHT Library (1.4.4): https://github.com/adafruit/DHT-sensor-library
 **********************************************************************************/


#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
// #include <ESP32Servo.h>

// Replace with your network credentials
// const char* ssid = "Infinix HOT 6 Pro2";  //WiFi Name
// const char* password = "Dammy4real4";    //WiFi Password

// const char* ssid = "iPhone";  //WiFi Name
// const char* password = "awinrin17";    //WiFi Password

// const char* ssid = "Samsung";  //WiFi Name
// const char* password = "belovedtt";    //WiFi Password

// const char* ssid = "TECNO POP 4";  //WiFi Name
// const char* password = "walyjim22";    //WiFi Password

// Use @myidbot to find out the chat ID of an individual or a group
// You need to click "start" on a bot before it can message you
// Initialize Telegram BOT
String chatId = "1996438264";
// String chatId = "-1001957709959";
String BOTtoken = "6086948138:AAH1st4SDgYdvRZeHlduT5MRO4V0t3aZUec";

bool sendPhoto = false;
bool withFlash = false;
bool motionSensorFlag = false;

WiFiClientSecure clientTCP;


int totalColumns = 16;
int totalRows = 2;

LiquidCrystal_I2C lcd(0x27, totalColumns, totalRows);

UniversalTelegramBot bot(BOTtoken, clientTCP);
// Servo myservo;  // create servo object to control a servo

// Define GPIOs
// #define SDA 15
#define SCL 14
#define PIR1 12
// #define PIR2 13
#define RELAY 16
// #define servoPin 15
#define DHTPIN 2
#define FLASH_LED 4

#define DHTTYPE DHT11  // DHT 11

DHT dht(DHTPIN, DHTTYPE);

//CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22


String r_msg = "";

const unsigned long BOT_MTBS = 1000;  // mean time between scan messages
unsigned long bot_lasttime;           // last time messages' scan has been done

void handleNewMessages(int numNewMessages);
String sendPhotoTelegram();

String getWeather() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();  // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return "Failed to read from DHT sensor! /weather";
  } else {
    return "Temperature: " + String(t) + " Humidity: " + String(h) + ". /weather";
  }
}

String lightOn() {
  if (digitalRead(RELAY)) {
    return "Light is already ON. /lightOff";
    lcd.setCursor(0, 0);
    lcd.print("     LIGHT IS      ");
    lcd.setCursor(0, 1);
    lcd.print("    ALREADY OFF    ");
  } else {
    digitalWrite(RELAY, HIGH);
    digitalWrite(FLASH_LED, HIGH);
    delay(100);
    return "Light is ON. /lightOff";
    lcd.setCursor(0, 0);
    lcd.print("     LIGHT ON   ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }
}

String lightOff() {
  if (digitalRead(RELAY)) {
    digitalWrite(RELAY, LOW);
    digitalWrite(FLASH_LED, LOW);
    delay(100);
    return "Light is OFF. /lightOn";
    lcd.setCursor(0, 0);
    lcd.print("     LIGHT ON   ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
  } else {
    return "Light is already OFF. /lightOn";
    lcd.setCursor(0, 0);
    lcd.print("     LIGHT IS      ");
    lcd.setCursor(0, 1);
    lcd.print("    ALREADY OFF    ");
  }
}

String sendPhotoTelegram() {

  const char* myDomain = "api.telegram.org";
  String getAll = "";
  String getBody = "";

  if (withFlash) {
    digitalWrite(FLASH_LED, HIGH);
    digitalWrite(RELAY, HIGH);
    delay(200);
  }

  camera_fb_t* fb = NULL;
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
    return "Camera capture failed";
  }

  Serial.println("Connect to " + String(myDomain));

  if (clientTCP.connect(myDomain, 443)) {
    Serial.println("Connection successful");
    lcd.setCursor(0, 0);
    lcd.print(" CONNECTION ");
    lcd.setCursor(0, 1);
    lcd.print("SUCCESSFUL");

    Serial.println("Connected to " + String(myDomain));

    String head = "--IotCircuitHub\r\nContent-Disposition: form-data; name=\"chat_id\"; \r\n\r\n" + chatId + "\r\n--IotCircuitHub\r\nContent-Disposition: form-data; name=\"photo\"; filename=\"esp32-cam.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
    String tail = "\r\n--IotCircuitHub--\r\n";

    uint16_t imageLen = fb->len;
    uint16_t extraLen = head.length() + tail.length();
    uint16_t totalLen = imageLen + extraLen;

    clientTCP.println("POST /bot" + BOTtoken + "/sendPhoto HTTP/1.1");
    clientTCP.println("Host: " + String(myDomain));
    clientTCP.println("Content-Length: " + String(totalLen));
    clientTCP.println("Content-Type: multipart/form-data; boundary=IotCircuitHub");
    clientTCP.println();
    clientTCP.print(head);

    uint8_t* fbBuf = fb->buf;
    size_t fbLen = fb->len;
    for (size_t n = 0; n < fbLen; n = n + 1024) {
      if (n + 1024 < fbLen) {
        clientTCP.write(fbBuf, 1024);
        fbBuf += 1024;
      } else if (fbLen % 1024 > 0) {
        size_t remainder = fbLen % 1024;
        clientTCP.write(fbBuf, remainder);
      }
    }

    clientTCP.print(tail);

    esp_camera_fb_return(fb);

    int waitTime = 10000;  // timeout 10 seconds
    long startTimer = millis();
    boolean state = false;


    while ((startTimer + waitTime) > millis()) {
      Serial.print(".");
      delay(100);
      while (clientTCP.available()) {
        char c = clientTCP.read();
        if (c == '\n') {
          if (getAll.length() == 0) state = true;
          getAll = "";
        } else if (c != '\r') {
          getAll += String(c);
        }
        if (state == true) {
          getBody += String(c);
        }
        startTimer = millis();
      }
      if (getBody.length() > 0) break;
    }
    clientTCP.stop();
    Serial.println(getBody);
  } else {
    getBody = "Connected to api.telegram.org failed.";
    Serial.println("Connected to api.telegram.org failed.");
  }
  if (withFlash) {
    digitalWrite(FLASH_LED, LOW);
    digitalWrite(RELAY, LOW);
    withFlash = false;
  }
  return getBody;
}

void handleNewMessages(int numNewMessages) {
  Serial.print("Handle New Messages: ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != chatId) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String fromName = bot.messages[i].from_name;
    if (text == "/photo@ESP32_Cam_motion23_bot") {
      sendPhoto = true;
      Serial.println("New photo request");
      lcd.setCursor(0, 0);
      lcd.print("    NEW PHOTO   ");
      lcd.setCursor(0, 1);
      lcd.print("     REQUEST   ");
    }
    if (text == "/photoWithFlash@ESP32_Cam_motion23_bot") {
      sendPhoto = true;
      withFlash = true;
      Serial.println("New photo request");
      lcd.setCursor(0, 0);
      lcd.print("NEW PHOTO WITH ");
      lcd.setCursor(0, 1);
      lcd.print("FLASH REQUEST   ");
    }
    if (text == "/lightOn@ESP32_Cam_motion23_bot") {
      String r_msg = lightOn();
      bot.sendMessage(chatId, r_msg, "");
    }
    if (text == "/lightOff@ESP32_Cam_motion23_bot") {
      String r_msg = lightOff();
      bot.sendMessage(chatId, r_msg, "");
    }
    if (text == "/SpeedOn@ESP32_Cam_motion23_bot") {
      motionSensorFlag = true;
      String r_msg = "Motion Sensor is ON. /motionOff";
      lcd.setCursor(0, 0);
      lcd.print("  MOTION SENSOR  ");
      lcd.setCursor(0, 1);
      lcd.print("  IS ACTIVATED  ");
      bot.sendMessage(chatId, r_msg, "");
    }
    if (text == "/SpeedOff@ESP32_Cam_motion23_bot") {
      motionSensorFlag = false;
      String r_msg = "Motion Sensor is OFF. /motionOn";
      lcd.setCursor(0, 0);
      lcd.print("  MOTION SENSOR  ");
      lcd.setCursor(0, 1);
      lcd.print(" IS DEACTIVATED  ");
      bot.sendMessage(chatId, r_msg, "");
    }
    if (text == "/weather@ESP32_Cam_motion23_bot") {
      String r_msg = getWeather();
      bot.sendMessage(chatId, r_msg, "");
    }
    ////////////////////////////////////////////

    if (text == "/photo") {
      sendPhoto = true;
      Serial.println("New photo request");
      lcd.setCursor(0, 0);
      lcd.print("    NEW PHOTO   ");
      lcd.setCursor(0, 1);
      lcd.print("     REQUEST   ");
    }
    if (text == "/photoWithFlash") {
      sendPhoto = true;
      withFlash = true;
      Serial.println("New photo request");
      lcd.setCursor(0, 0);
      lcd.print("NEW PHOTO WITH ");
      lcd.setCursor(0, 1);
      lcd.print("FLASH REQUEST   ");
    }
    if (text == "/lightOn") {
      String r_msg = lightOn();
      bot.sendMessage(chatId, r_msg, "");
    }
    if (text == "/lightOff") {
      String r_msg = lightOff();
      bot.sendMessage(chatId, r_msg, "");
    }
    if (text == "/motionOn") {
      motionSensorFlag = true;
      
      String r_msg = "Motion Sensor is ON. /motionOff";
      lcd.setCursor(0, 0);
      lcd.print("  MOTION SENSOR  ");
      lcd.setCursor(0, 1);
      lcd.print("  IS ACTIVATED  ");
      bot.sendMessage(chatId, r_msg, "");
    }
    if (text == "/motionOff") {
      motionSensorFlag = false;
      String r_msg = "Motion Sensor is OFF. /motionOn";
      lcd.setCursor(0, 0);
      lcd.print("  MOTION SENSOR  ");
      lcd.setCursor(0, 1);
      lcd.print(" IS DEACTIVATED  ");
      bot.sendMessage(chatId, r_msg, "");
    }
    if (text == "/weather") {
      String r_msg = getWeather();
      bot.sendMessage(chatId, r_msg, "");
    }
    if (text == "/start") {
      String welcome = "Welcome to the ESP32-CAM Telegram Motion Camera.\n";
      welcome += "/photo : Takes a new photo\n";
      welcome += "/photoWithFlash : Takes photo with Flash\n";
      welcome += "/lightOn : Turn ON the Light\n";
      welcome += "/lightOff : Turn OFF the Light\n";
      welcome += "/motionOn : Turn ON Motion Sensor\n";
      welcome += "/motionOff : Turn OFF Motion Sensor\n";
      welcome += "/weather : Send Live Weather.\n";
      bot.sendMessage(chatId, welcome, "Markdown");
    }
  }
}

void configInitCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  //init with high specs to pre-allocate larger buffers
  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;  //0-63 lower number means higher quality
    config.fb_count = 2;
    config.grab_mode = CAMERA_GRAB_LATEST;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;  //0-63 lower number means higher quality
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }

  /*
   * UXGA   = 1600 x 1200 pixels
   * SXGA   = 1280 x 1024 pixels
   * XGA    = 1024 x 768  pixels
   * SVGA   = 800 x 600   pixels
   * VGA    = 640 x 480   pixels
   * CIF    = 352 x 288   pixels
   * QVGA   = 320 x 240   pixels
   * HQVGA  = 240 x 160   pixels
   * QQVGA  = 160 x 120   pixels
   */

  //  // Drop down frame size for higher initial frame rate
  //  sensor_t * s = esp_camera_sensor_get();
  //  s->set_framesize(s, FRAMESIZE_CIF);  // UXGA|SXGA|XGA|SVGA|VGA|CIF|QVGA|HQVGA|QQVGA

  sensor_t* s = esp_camera_sensor_get();
  //initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);        //flip it back
    s->set_brightness(s, 1);   //up the blightness just a bit
    s->set_saturation(s, -2);  //lower the saturation
  }
  //drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_CIF);
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  // myservo.attach(servoPin, 1000, 2000);

  Wire.begin(SDA, SCL);
  Serial.begin(115200);
  delay(1000);
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("   MOVEMENT    ");
  lcd.setCursor(0, 1);
  lcd.print("SECURITY CAMERA");
  delay(2000);
  lcd.clear();
  pinMode(RELAY, OUTPUT);
  pinMode(FLASH_LED, OUTPUT);
  // pinMode(servoPin, INPUT);
  pinMode(PIR1, INPUT);
  // pinMode(PIR2, INPUT);

  digitalWrite(RELAY, LOW);
  //digitalWrite(FLASH_LED, LOW);

  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Connecting to ");
  lcd.setCursor(0, 0);
  lcd.print("  CONNECTING TO  ");
  lcd.setCursor(0, 1);
  lcd.print(ssid);
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  clientTCP.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  //  int connecting_process_timed_out = 20; //--> 20 = 20 seconds.
  //  connecting_process_timed_out = connecting_process_timed_out * 2;
  //  while (WiFi.status() != WL_CONNECTED) {
  //    Serial.print(".");
  //    digitalWrite(FLASH_LED, HIGH);
  //    delay(250);
  //    digitalWrite(FLASH_LED, LOW);
  //    delay(250);
  //    if(connecting_process_timed_out > 0) connecting_process_timed_out--;
  //    if(connecting_process_timed_out == 0) {
  //      delay(1000);
  //      ESP.restart();
  //    }
  //  }
  //  digitalWrite(FLASH_LED, LOW);

  Serial.println();
  Serial.print("ESP32-CAM IP Address: ");
  Serial.println(WiFi.localIP());

  dht.begin();  // Enabling DHT sensor

  Serial.println("Start configuring and initializing the camera...");
  lcd.setCursor(0, 0);
  lcd.print("CONFIG & INIT...");
  lcd.setCursor(0, 1);
  lcd.print("    CAMERA      ");
  configInitCamera();
  Serial.println("Successfully configure and initialize the camera.");
  lcd.setCursor(0, 0);
  lcd.print("CAMERA CONFIG");
  lcd.setCursor(0, 1);
  lcd.print("  SUCCESSFUL  ");
  lcd.clear();
  delay(1000);
}

void loop() {

  if (sendPhoto) {
    lcd.setCursor(0, 0);
    lcd.print("Preparing photo");
    Serial.println("Preparing photo");
    sendPhotoTelegram();
    sendPhoto = false;
  }
  
    int p = digitalRead(PIR1);
    // int q = digitalRead(PIR2);
    Serial.println(p);
    if (digitalRead(PIR1) == LOW) {
      lcd.setCursor(0, 0);
      lcd.print("Motion Detected Right");
      lcd.setCursor(0, 1);
      lcd.print("Preparing photo");
      Serial.println("Motion Detected Right");
      Serial.println("Preparing photo");
      // myservo.write(180);  // tell servo to go to position in variable 'pos'
      delay(15);
      // withFlash = true;z
      withFlash = true;
      sendPhotoTelegram();
      sendPhoto = false;
      delay(1000);
    }

    // if (digitalRead(PIR2) == LOW) {
    //   lcd.setCursor(0, 0);
    //   lcd.print("Motion Detected Left");
    //   lcd.setCursor(0, 1);
    //   lcd.print("Preparing photo");
    //   Serial.println("Motion Detected Left");
    //   Serial.println("Preparing photo");
    //   // myservo.write(0);  // tell servo to go to position in variable 'pos'
    //   delay(15);
    //   // withFlash = true;z
    //   withFlash = true;
    //   sendPhotoTelegram();
    //   sendPhoto = false;
    //   delay(1000);
    // }
  

  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      lcd.setCursor(0, 1);
      lcd.print(" got response  ");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    bot_lasttime = millis();
  }
}
