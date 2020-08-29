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
#include "printf.h"


//  |Config|

RF24 radio(10, 9); // Set up nRF24L01 (makes OOP object)
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };   // radio address

// 1 byte can hold 1 character , there for the max amount of data that can be sent in one packect witht the NRF24 is 4bytes worth of character

bool TX = 1, RX = 0, Role = 0; // assign bool value to text representatives

// error values
int error = 0;
unsigned long startTime, stopTime;
//---------
bool UnsentData = false;
char Serialdata = "0";

int radioread = 0;
bool received = false;



void setup() {

  //Setup and configure rf radio//

  Serial.begin(115200);
  delay(2000); // allow time for start up
  printf_begin();
  Serial.println("Initialising embedded software"); // Debug for when the start up function runs
  radio.begin();  // called function to setup the radio.

  radio.setChannel(125); // select sport portion of the 2.4 gigahertz Spectrum it is broadcasting on in this case it is selected above the frequency of 2.4Ghz Wi-Fi thus was will not received interference.
  radio.setPALevel(RF24_PA_LOW); // will be reaplaced with RF24_PA_MAX for larger range & penetration

  radio.setDataRate(RF24_250KBPS); // less speed means great transmission stability (values can be [RF24_250KBPS, RF24_1MBPS, RF24_2MBPS])
  radio.setAutoAck(true); // enables autoACK  this is what autoACK is https://forum.arduino.cc/index.php?topic=504412.0
  
  radio.enableDynamicPayloads() ;
  radio.enableAckPayload();         

  radio.setRetries(1, 15);  // delay,(max 15) count(max 15)
  radio.setCRCLength(RF24_CRC_8);  // Cyclic redundancy check used for error-detecting

  radio.openWritingPipe(pipes[1]); // radio address
  radio.openReadingPipe(1, pipes[0]);

  radio.startListening();                  // Start listening
  radio.printDetails();                    // Dump the configuration of the rf unit for debugging || #include "printf.h" and also   printf_begin();

  radio.powerUp();                         //Power up the radio

  delay(1000);

  Serial.println("Initialising Main Program");
  Serial.println("defaulting RECEIVE State");
  Serial.println("\n *** R=RECEIVE | T=TRANSMIT ***");

  delay(1000);

  // debug
}
void loop() {
  Serialread(); // read if there is Serial information
  // Transmission Mode change

  if (UnsentData == true ) { // Looks if there is data that needs to be tranmitted
    if (Role == TX) {
      // do nothing as it will send data anyway also dont reset connect to other radio
    } else {
      TXF(); // makesure role to TX so that it can set
    }
  }

  //TX
  if (Role == TX) {
  Serial.println("");
    
    byte wireless_send[4];  // store data to be transmitted
    error = 0; // reset if there has been a error


    wireless_send[0] = 1 ;
    wireless_send[1] = 2 ; 
    wireless_send[2] = 3 ; 
    wireless_send[3] = 4 ;
    
 Serial.println("Transmitting...");
    if (!radio.write(&wireless_send, 4)) { //Write to the FIFO buffers, also useds dynamic payload size
      error = 1 ;                      //Keep count of failed payloads
      Serial.println("Transmission error");;
    }else{
    Serial.println("Transmission Successful\n");
    UnsentData == false;
    }
    
    if (error == 0) { // checks if there is a error while transmission of data
      
      RXF();
    }else{
      
     }
    
  } // TX END




  //RX
  if (Role == RX) {
    byte wireless_receive[4];  // store data that has been received
   
    
    if (radio.available()) { // if there is information get prep for incomming otherwise 
      while (radio.available()) { // loop to read all the information in FIFO BUS
        radio.read(&wireless_receive, 4);   
        received = true;
        for (int i = 0; i < 4; i++) {
          radioread = wireless_receive[i];                //Load the buffer with random data
          Serial.print("Data Received: ");
          Serial.println(radioread);
        }
        Serial.println("\n\n");
        for (int i = 0; i < 4; i++) { // clears data
          wireless_receive[i] = 0;
        }

      }
      if (received == true) {
        Serial.println("Clearing Array\n\n\n");
        received = false;

      }
  } else {
  delay(100);
  Serial.println("\nNothing to Read in NRF24 Buffer\n");

  }
 } 
} // end of loop


// |---------|
// |Functions|
// |---------|

void TXF() {
  Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
  radio.stopListening();                  // Stop listening
  delay(10);
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  Role = TX;
  delay(1000);

}
void RXF(void) {
  Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
  delay(10);
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);
  Role = RX;
  radio.startListening();                  // Start listening
  delay(1000);
}



// Serial override
void Serialread(void) {
  if ( Serial.available()) {
    Serialdata = toupper(Serial.read());
    if (Serialdata == 'T') {
      TXF();

    } else if (Serialdata == 'R') {
      RXF();
    }
  }
}
