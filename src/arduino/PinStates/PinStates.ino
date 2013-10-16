
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

// Using an array of pointers soas to not have to keep editing 
// the code when we want to change pin counts.
CapacitiveSensor *pins[MAX_PIN_COUNT];
char pinState[MAX_PIN_COUNT+1]; // +1 for null terminator

void setup() {
  //start serial connection
  Serial.begin(9600);

  // TODO: initialze using values read from Serial
  for (int i=0; i<PIN_COUNT; i++) {    
    CapacitiveSensor currentPin =  CapacitiveSensor(COMMON_PIN,START_PIN+0);
    currentPin.set_CS_AutocaL_Millis(0xFFFFFFFF);  // turn off auto-calibrate
    pins[i] = &currentPin;  // pins is an array of pointers
    pinState[i] = '0'; // start all pins in 'off' state
  }

  // Last character in our state array should be a null as we will
  // ultimately "print" it to serial output; this will effectively 
  // 'truncate' the pinState array to the actual number of keys
  // rather than the max number of keys.
  pinState[PIN_COUNT] = 0;
  
}

#define TRIGGER_THRESHOLD  150 // what reading triggers the note to play
#define SAMPLE_COUNT        10 // how many triggered readings do we take before deciding the note should play
long sensorReading;

void loop(){

  for (int i=0; i<PIN_COUNT; i++) {
    sensorReading = pins[i]->capacitiveSensor(SAMPLE_COUNT);
    if (sensorReading > TRIGGER_THRESHOLD)
      pinState[i] = '1';
    else
      pinState[i] = '0'; 
  }

  Serial.println(pinState);
  delay(100);
}



