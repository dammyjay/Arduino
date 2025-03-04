//----------------------------------------------------------
// Test passive infrared presence sensor
//
// Dean Gienger, March 1, 2024
//
// PIR sensor is a small (about 1" square) sensor board with
// a half-spherical white plastic cover over it.  There are three
// leads - GND, VCC, and OUT
//
// There is an AM312 model that runs on 3.3V and an HC-SR501 that
// runs on 5V.   The AM312 has the half-dome on the end of the board
// the HC-SR501 has the some flat on the board.
//
// When the PIR sensor detects a presence - meaning it detects the
// heat generated by a warm body within it's range, it will set the
// output "open" - so if you have a pull-up resistor, the waveform will
// be at 0v when nothing is detected and 3.3v when something is detected.
//
// You can set the pin-mode for the GPIO pin connected to the PIR output
// to mode INPUT_PULLUP for this purpose, or you can use an external pull-up
// resistor.  That way even for 5V VCC it is compatible with 3.3V inputs
// on the ESP32
//
// Pulse goes high sometimes for a very short time - maybe 5-10us - so you can't poll
// it, you need to interrupt on rising edge.
//
// Credit: Rui Santos, randomnerdtutorials.com
//
// V1.0 - initial release
// V1.1 - deactivate on destruct

#include <Arduino.h>
#include "PIRSensor.h"
#include <FunctionalInterrupt.h>

//------------------------------------------------
// Constructor
PIRSensor::PIRSensor(int gpiopin)
{
    gpioPin = gpiopin;
    triggerCount = 0;
    lastReadTriggerCount = 0;
    trigTime = 0;
    pinMode(gpioPin,INPUT_PULLUP);
}

//------------------------------------------------
// activate a PIR sensor on a particular pin
int PIRSensor::activate()
{
  triggerCount = 0;
  lastReadTriggerCount = 0;
  trigTime = 0;
  pinMode(gpioPin,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(gpioPin), std::bind(&PIRSensor::isr,this), RISING);
  return true; // success
}

//------------------------------------------------
// detach a PIR sensor - make it inactive
int PIRSensor::deactivate()
{
  detachInterrupt(gpioPin);
  gpioPin = -1; // not assigned, not active
  return true; // success
}

//------------------------------------------------
// true if there was a trigger in last xms
int PIRSensor::triggeredWithin(unsigned long xms)
{
  unsigned long x = millis() - trigTime;
  return x <= xms;
}

//------------------------------------------------
// see how many triggers since last time
// this routine was called
unsigned long PIRSensor::currentTriggers() 
{ 
  unsigned long n = triggerCount - lastReadTriggerCount; 
  lastReadTriggerCount = triggerCount; 
  return n;
}
