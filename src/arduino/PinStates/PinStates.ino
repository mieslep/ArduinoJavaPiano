
/* This program will monitor a number of pins and send their states
 * via the serial bus as a string of 0's and 1's, indicating if the 
 * pin is "off" or "on" respectively.
 *
 * Configuration is currently specified in config.h as hard-coded
 * values, eventually it would be nifty to allow some amount of 
 * configuration and control by reading values from the serial bus.
 *
 * Note that this program uses CapacitiveSensor library, available at
 * http://playground.arduino.cc//Main/CapacitiveSensor
 */

#include <CapacitiveSensor.h>
#include "config.h"

#define SAMPLE_COUNT         1 // how many readings do we sum to determine the sensor value
#define TRIGGER_THRESHOLD   20 // what reading triggers the pin to be on


// Using an array of pointers soas to not have to keep editing 
// the code when we want to change pin counts - CapacitiveSensor doesn't
// have a default constructor.
CapacitiveSensor *pins[MAX_PIN_COUNT];
long pinReadings[MAX_PIN_COUNT];
char pinStates[MAX_PIN_COUNT+1]; // +1 for null terminator
CapacitiveSensor currentPin = CapacitiveSensor(1,2);

void setup() {
  //start serial connection
  Serial.begin(9600);

  for (int i=0; i<PIN_COUNT; i++) {    
    currentPin =  CapacitiveSensor(COMMON_PIN,START_PIN+i);
    currentPin.set_CS_AutocaL_Millis(0xFFFFFFFF);  // turn off auto-calibrate
    pins[i] = &currentPin;  // pins is an array of pointers
    pinReadings[i] = 0; // start all readings as 0
    pinStates[i] = '0'; // start all pins in 'off' state
  }

  // Last character in our state array should be a null as we will
  // ultimately "print" it to serial output; this will effectively 
  // 'truncate' the pinState array to the actual number of pins
  // rather than the max number of pins.
  pinStates[PIN_COUNT] = 0;

  // This is "loop()", but in setup() so that we don't de-reference
  // our objects
  while (true) {
  
    // Sweep quickly through the pins to capture the readings
    for (int i=0; i<PIN_COUNT; i++) {
//      pinReadings[i] = max(0,pins[i]->capacitiveSensor(SAMPLE_COUNT));
      pinReadings[i] = pins[i]->capacitiveSensor(SAMPLE_COUNT);
    }
  
    for (int i=0; i<PIN_COUNT; i++) {
      if (pinReadings[i] > TRIGGER_THRESHOLD)
        pinStates[i] = '1';
      else
        pinStates[i] = '0'; 
    }
  
    Serial.print(pinStates);
    for (int i=0; i<PIN_COUNT; i++) {
      Serial.print(",");
      Serial.print(pinReadings[i]);
    }    
    Serial.println();
    
    delay(100);
  }
  
}

void loop(){
  // loop will never be reached
}



