#include "Arduino.h"
#include <L298NDriver.h>

const int left_motor_in1_pin = 4;
const int left_motor_in2_pin = 5;
const int right_motor_in3_pin = 7;
const int right_motor_in4_pin = 8;

const int ena_pin=3;
const int enb_pin=11;

L298NDriver motor_driver;

void setup() {
  motor_driver.init(left_motor_in1_pin, left_motor_in2_pin, right_motor_in3_pin, right_motor_in4_pin, ena_pin, enb_pin);
  motor_driver.stop();
}

void loop() {
  motor_driver.move(80, true);
  delay(100);
}·