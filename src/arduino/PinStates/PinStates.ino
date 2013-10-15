#include <CapacitiveSensor.h>
#include "config.h"


// Using an array of pointers soas to not have to keep editing 
// the code when we want to change pin counts.
CapacitiveSensor *pins[MAX_KEY_COUNT];
char pinState[MAX_KEY_COUNT+1]; // +1 for null terminator

void setup() {
  //start serial connection
  Serial.begin(9600);

  // TODO: initialze using values read from Serial
  for (int i=0; i<KEY_COUNT; i++) {    
    CapacitiveSensor currentPin =  CapacitiveSensor(COMMON_PIN,START_PIN+0);
    currentPin.set_CS_AutocaL_Millis(0xFFFFFFFF);  // turn off auto-calibrate
    pins[i] = &currentPin;  // pins is an array of pointers
    pinState[i] = '0'; // start all pins in 'off' state
  }

  // Last character in our state array should be a null as we will
  // ultimately "print" it to serial output; this will effectively 
  // 'truncate' the pinState array to the actual number of keys
  // rather than the max number of keys.
  pinState[KEY_COUNT] = 0;
  
}

#define TRIGGER_THRESHOLD  150 // what reading triggers the note to play
#define SAMPLE_COUNT        10 // how many triggered readings do we take before deciding the note should play
long sensorReading;

void loop(){

  for (int i=0; i<KEY_COUNT; i++) {
    sensorReading = pins[i]->capacitiveSensor(SAMPLE_COUNT);
    if (sensorReading > TRIGGER_THRESHOLD)
      pinState[i] = '1';
    else
      pinState[i] = '0'; 
  }

  Serial.println(pinState);
  delay(100);
}



