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
#define numReadings 10        // Used to give amount to data to keep a average of
    int _readings[numReadings];      // the readings from the analog input
    int _readIndex = 0;              // the index of the current reading
    int _total = 0;                  // the running total
    int _average = 0;                // the average
    int _inputpin;                  // Input
  public:
    void Smooth_setup() {
    int Smooth(int inputpin) {
 };
 void AnalogSmooth::Smooth_setup(){
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
  readings[thisReading] = 0;
  }
}
  
}

void setup() {


}

void loop() {

}

/*

http://mypractic.com/lesson-7-classes-in-c-language-for-arduino-button-as-an-object/
// Define the number of samples to keep track of. The higher the number, the
// more the readings will be smoothed, but the slower the output will respond to
// the input. Using a constant rather than a normal variable lets us use this
// value to determine the size of the readings array.
class AnalogSmooth(){
  
  
}
const int numReadings = 15;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int inputPin;

void setup() {

  pinMode(inputPin, INPUT);
  // initialize serial communication with computer:
  Serial.begin(2000000);
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(inputPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  Serial.println(average);
}
*/
