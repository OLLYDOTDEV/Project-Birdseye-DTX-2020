#include "Arduino.h"

/*
  Smoothing

  Reads repeatedly from an analog input, calculating a running average and
  printing it to the computer. Keeps ten readings in an array and continually
  averages them.

  created 22 Apr 2007
  by David A. Mellis  <dam@mellis.org>
  modified 9 Apr 2012
  by Tom Igoe
  convered into class
  by Oliver Bell
  This example code is based of http://www.arduino.cc/en/Tutorial/Smoothing
*/

// Define the number of samples to keep track of. The higher the number, the
// more the readings will be smoothed, but the slower the output will respond to
// the input. Using a constant rather than a normal variable lets us use this
// value to determine the size of the readings array.

class AnalogSmooth {
  private:
    #define _numReadings 8          // Used to give amount to data to keep a average of
    int _ValuecCount = 0;           // count first amount of values while less then StartSkip
    int _StartSkip = 11;            // amount for ^^^ to skip
    int _readings[_numReadings];    // the readings from the analog input
    int _readIndex = 0;             // the index of the current reading
    int _total = 0;                 // the running total
    int _average = 0;               // the average
  public:
    void Smooth_setup();
    int Smooth(int Sinputpin);
    int Sinputpin;                  // Input
};

void AnalogSmooth::Smooth_setup(){
     for (int thisReading = 0; thisReading < _numReadings; thisReading++) {
     _readings[thisReading] = 0; // sets all array 0
     }
}

int AnalogSmooth::Smooth(int Sinputpin) {
 
  // subtract the last reading:
  _total = _total - _readings[_readIndex];
  // read from the sensor:
  _readings[_readIndex] = analogRead(Sinputpin);
  // add the reading to the total:
  _total = _total + _readings[_readIndex];
  // advance to the next position in the array:
  _readIndex = _readIndex + 1;

  // if we're at the end of the array...
  if (_readIndex >= _numReadings) {
    // ...wrap around to the beginning:
    _readIndex = 0;
  }
  _average = _total / _numReadings;




if(_ValuecCount < _StartSkip){

 Serial.print("Skipping start value number:");
 Serial.println(_ValuecCount);
 _ValuecCount++;
 // give temp output value of first reading
  return(_readings[0]);
}else{
  // return value of object
  return(_average);
  }
}
