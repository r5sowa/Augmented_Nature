/*
 Original code attributed below.
 I made only a few slight modifications: I cut out the print 
 statement "MPR121 found!" in setup(), and I changed the print
 statements in loop() to print "1" instead of "touched" and "0"
 instead of "released" for easier parsing in Max/MSP.
 */

/*********************************************************
This is a library for the MPR121 12-channel Capacitive touch sensor

Designed specifically to work with the MPR121 Breakout in the Adafruit shop 
  ----> https://www.adafruit.com/products/

These sensors use I2C communicate, at least 2 pins are required 
to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.  
BSD license, all text above must be included in any redistribution
**********************************************************/

#include <Wire.h>
#include "Adafruit_MPR121.h"

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

int started = 0; // flag for whether we've received serial yet

void setup() {
  while (!Serial);        // needed to keep leonardo/micro from starting too fast!

  Serial.begin(9600);
  //Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  //Serial.println("MPR121 found!");
}

void loop() {
  // Get the currently touched pads

  if(Serial.available()) // check to see if there's serial data in the buffer
  {
     Serial.read(); // read a byte of serial data
     started = 1; // set the started flag to on
  }

  if (started) {
    currtouched = cap.touched();
    
    for (uint8_t i=0; i<12; i++) {
      // it if *is* touched and *wasnt* touched before, alert!
      if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
        Serial.print(i); Serial.println(" 1");
      }
      // if it *was* touched and now *isnt*, alert!
      if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
        Serial.print(i); Serial.println(" 0");
      }
    }
  
    // reset our state
    lasttouched = currtouched;
  
    // comment out this line for detailed data from the sensor!
    return;
    
    // debugging info, what
    Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap.touched(), HEX);
    Serial.print("Filt: ");
    for (uint8_t i=0; i<12; i++) {
      Serial.print(cap.filteredData(i)); Serial.print("\t");
    }
    Serial.println();
    Serial.print("Base: ");
    for (uint8_t i=0; i<12; i++) {
      Serial.print(cap.baselineData(i)); Serial.print("\t");
    }
    Serial.println();
    
    // put a delay so it isn't overwhelming
    delay(100);
  }
}
