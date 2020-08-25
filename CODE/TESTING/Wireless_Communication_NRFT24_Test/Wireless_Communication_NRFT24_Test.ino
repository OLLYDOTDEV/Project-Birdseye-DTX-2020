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
const uint64_t pipes[2] = {"1NODE", "2NODE"};   // radio address 

// 1 byte can hold 1 character , there for the max amount of data that can be sent in one packect witht the NRF24 is 32bytes worth of character
String wireless_send[32]= {"test","test2"};  // store the data to be transmitted 
String wireless_receive[32] = {"test","test2"};  // store data that has been received

bool TX = 1, RX = 0, Role = 0; // assign bool value to text representatives

// error values
int error = 0;
unsigned long startTime, stopTime;
//---------
bool UnsentData = false;


void setup(){
 
  //Setup and configure rf radio//

 Serial.begin(115200);
 delay(2000); // allow time for start up 
 Serial.println("Initialising embedded software"); // Debug for when the start up function runs 
 radio.begin();  // called function to setup the radio.
 radio.setChannel(125); // select sport portion of the 2.4 gigahertz Spectrum it is broadcasting on in this case it is selected above the frequency of 2.4Ghz Wi-Fi thus was will not received interference. 
 radio.setPALevel(RF24_PA_LOW); // will be reaplaced with RF24_PA_MAX for larger range & penetration
 radio.setDataRate(RF24_250KBPS); // less speed means great transmission stability (values can be [RF24_250KBPS, RF24_1MBPS, RF24_2MBPS])
 radio.setAutoAck(1); // enables autoACK  this is what autoACK is https://forum.arduino.cc/index.php?topic=504412.0
 radio.setRetries(15, 15);  // delay,(max 15) count(max 15)
 radio.setCRCLength(RF24_CRC_8);  // Cyclic redundancy check used for error-detecting
 radio.openWritingPipe(pipes[0]); // radio address 
 radio.openReadingPipe(1, pipes[1]); 
 radio.startListening();                  // Start listening
 radio.printDetails();                    // Dump the configuration of the rf unit for debugging
 radio.powerUp();                         //Power up the radio

  
Serial.println("Initialising Main Program");
delay(100);

// debug

  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

}
void loop(){

// Transmission Mode change
 
if (UnsentData == true ){ // Looks if there is data that needs to be tranmitted 
if(Role == TX){
// do nothing as it will send data anyway also dont reset connect to other radio
}else {
  TXF(); // makesure role to TX so that it can set     
}  
}
  
  //TX
while (Role == TXF) {
 error = 0;
 if(!radio.writeFast(&wireless_send, 32)){ //Write to the FIFO buffers, also useds dynamic payload size
 error++ ;                      //Keep count of failed payloads
 Serial.print("Transmission error");;
 }

if(!radio.txStandBy()){
 error++;  
  Serial.print("Flush TX FIFO failed"); 
}    
UnsentData == false;

if(error != 0){ // checks if there is a error while transmission of data
error = 0;
RXF(); 
}

} // TX END




  //RX
if (Role == RXF){
while (radio.available()) {
  radio.read(&wireless_receive, 32);
  Serial.print("Wireless data Recived, Printing: ");
  Serial.println(wireless_receive[1]);
}
}


} // end of loop


// |---------|
// |Functions|
// |---------|

void TXF(){
  Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
  delay(10);  
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]); 
  Role = RXF; 
  radio.stopListening();                  // Stop listening 
 }
void RXF(void){
  Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
  delay(10);  
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]); 
  Role = RXF; 
  radio.startListening();                  // Start listening
}
