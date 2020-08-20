/*
 Date 2020/8/20
  Title: NRF24 radio
  REPO: https://github.com/OLLYDOTDEV/Project-Birdseye-DTX-2020
  Description: create a testing platform for sendeding remote local data from one
  NRF24 radio to another. this code is and derived from
  http://tmrh20.github.io/RF24/gettingstarted_8cpp-example.html
  http://tmrh20.github.io/RF24/Transfer_8ino-example.html

  for the purpose of https://github.com/OLLYDOTDEV/Project-Birdseye-DTX-2020/issues/17
*/

// include needed libraries
#include <SPI.h>
#include "RF24.h"



//  |Config| 

RF24 radio(9,10);  // Set up nRF24L01 (makes OOP object)
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };   // radio address 
const byte payload_size = 4; // set side of wireless_data array
String wireless_data[payload_size] = {"ID","Data1","Data2","Data3"};  // store the data to be transmitted 
bool TX = 1, RX = 0, Role = 0; // assign bool value to text representatives

// counter values
int counter = 0;
unsigned long startTime, stopTime;
//---------
bool UnsentData = false;


void setup() {
 
  //Setup and configure rf radio//

 Serial.begin(115200);
 radio.begin();  // called function to setup the radio.
 radio.setChannel(125); // select sport portion of the 2.4 gigahertz Spectrum it is broadcasting on in this case it is selected above the frequency of 2.4Ghz Wi-Fi thus was will not received interference. 
 radio.setPALevel(RF24_PA_LOW); // will be reaplaced with RF24_PA_MAX for larger range & penetration
 radio.setDataRate(RF24_250KBPS); // less speed means great transmission stability (values can be [RF24_250KBPS, RF24_1MBPS, RF24_2MBPS])
 radio.setAutoAck(1); // enables autoACK  this is what autoACK is https://forum.arduino.cc/index.php?topic=504412.0
 radio.setRetries(15, 15);  // delay,(max 15) count(max 15)
 radio.setCRCLength(RF24_CRC_8);  // Cyclic redundancy check used for error-detecting

 radio.openWritintgPipe(pipes[0]); // radio address 
 radio.openReadingPipe(1, pipes[1]); 
 radio.startListening();                  // Start listening
 radio.printDetails();                    // Dump the configuration of the rf unit for debugging
 radio.powerUp();                         //Power up the radio

  
Serial.println("initialising programmed");

// debug

  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

}
void loop() {

// Transmission Mode change
 
if (UnsentData == true ){ // Looks if there is data that needs to be tranmitted 
if(Role == TX){
// do nothing as it will send data anyway also dont reset connect to other radio
}else {
  TX() // makesure role to TX so that it can set     
}  
}
  
  //TX
if (Role == TX) {

      
      if (!radio.writeFast(&wireless_data, payload_size)) { //Write to the FIFO buffers
        counter++;                      //Keep count of failed payloads
        Serial.print("transmission error: ");
        Serial.print(counter);
      }
     
}




  //RX





} // end of loop


// |---------|
// |Functions|
// |---------|

void TX(void){
  Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
  delay(10);  
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]); 
  Role = RX; 
  radio.stopListening();                  // Stop listening 
 }
void RX(void){
  Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
  delay(10);  
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]); 
  Role = RX; 
  radio.startListening();                  // Start listening
}
