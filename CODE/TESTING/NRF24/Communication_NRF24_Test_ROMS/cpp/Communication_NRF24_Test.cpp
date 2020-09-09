/*
  Date 2020/8/20
  Title: NRF24 radio
  REPO: https://github.com/OLLYDOTDEV/Project-Birdseye-DTX-2020
  Description: create a testing platform for sendeding remote local data from one
  NRF24 radio to another. this code is and derived from
  http://tmrh20.github.io/RF24/gettingstarted_8cpp-example.html
  http://tmrh20.github.io/RF24/Transfer_8ino-example.html

  for the purpose of https://github.com/OLLYDOTDEV/Project-Birdseye-DTX-2020/issues/17

  make to run on RPI

*/

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <cstddef>

#include <RF24/RF24.h>

using namespace std;

//  |Config|
RF24 radio(22,0); // Set up nRF24L01 (makes OOP object) || RF24(_cepin _cspin )
// include needed libraries

 
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };   // radio address

// 1 byte can hold 1 character , there for the max amount of data that can be sent in one packect witht the NRF24 is 4bytes worth of character

bool TX = 1, RX = 0, Role = 0; // assign bool value to text representatives

// error values
int Transmission_error = 0;
int PacketSizeError = 0;

unsigned long startTime, stopTime;

bool Transmissiontime = false; // true means that the radio has been trying to tranmit for to long and failed

//---------
bool UnsentData = false;
char Serialdata = '0';

int radioread = 0;
bool received = false;
bool receiving = false;

// custom data types 
typedef struct // for Wireless_Receive packet 
{
  string Data; 
  }
Wireless_ReceiveDef;
Wireless_ReceiveDef Wireless_Receive;


typedef struct // for Wireless_Recieve Buffer 
{ 
  char Data[32];
  }
Buff_ReceiveDef ;
Buff_ReceiveDef Buff_Receive;

// --------------

typedef struct // for Wireless_Send packet 
{
  string Data; 
  }
Wireless_SendDef;
Wireless_SendDef Wireless_Send;

typedef struct // for Wireless_Send Buffer 
{
  char Data[32]; 
  }
Buff_SendDef;
Buff_SendDef Buff_Send;









// |---------|
// |Functions|
// |---------|


// 

void TXF() {
 cout << "\n*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK\n";
  radio.stopListening();                  // Stop listening
  delay(10);
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  Role = TX;
  delay(1000);
}
void RXF() {
  cout << "\n*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK\n" ;
  delay(10);
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);
  Role = RX;
  radio.startListening();                  // Start listening
  delay(1000);
}

void RECEIVE(){

 if(radio.available()) { // if there is information get prep for incomming otherwise 
    while(radio.available()){ // loop to read all the information in FIFO BUS
         radio.read(&Buff_Receive,sizeof(Buff_Receive));   
         received = true;
         cout << "\nreceiving...\n" ;
        }
             
    string TempBuffString(Buff_Receive.Data); // This Varible must be declare here so that the string constructor is called
    Wireless_Receive.Data = TempBuffString;

    cout << "\nPacket Size: " ;
    cout << sizeof(Wireless_Receive);
    cout << "\nPacket Length: " ;
    cout << Wireless_Receive.Data.length();  
     
      if(received == true){       
        cout << "\nreceived: " ;
        cout << Wireless_Receive.Data ;
        received = false;
       }
  }else{
       delay(100);
      cout << "\nNothing to Read in NRF24 Buffer\n";
       }   
}

void TRANSMIT(){
 // Wireless_Send.Data = "12341234123412341234123412341234"; // Set Max length temp data for testing
 Wireless_Send.Data = "Test Data " ;// set temp data for testing
 cout << "\nChecking Packet Integrity\n"; 
 // Checking for errors the packet that is to be sent
 if(Wireless_Send.Data.length() <= sizeof(Buff_Send.Data)){ // Stop overloading Char array with to large sized string
  
   for (int i = 0;i <= Wireless_Send.Data.length(); i++) {
    Buff_Send.Data[i] = Wireless_Send.Data[i]; // string to char Array  
  }
  
 }else{
      PacketSizeError = 1;
      cout << "\n\n\n" ;
      cout << "Warning failed to convert string to array: string to large, please make sure that the string is smaller then char array ";
      cout << "\n\n\n" ;
      }
    
 if(sizeof(Buff_Send)> 32){ // add check for length of all varible in Datapak and make sure they are also under 32 
   PacketSizeError = 1;
   cout << "Buff Size: " ;
   cout << sizeof(Buff_Send) ;
   cout << "Failed to Send, Data Structure to Large\n" ;
  }

  
if(PacketSizeError == 0){
  
  cout << "\nPacket Integrity Verified \n\nTransmitting..." ; 
  // handles transmission errors
  if(Transmission_error > 9){ // if error count great then 10 reset value
    Transmission_error = 0; 
   }
  if(Transmission_error == 0){
    startTime = millis();
    Transmissiontime = false;
   }
    // Sends data to Radio and then give feedback
   if(!radio.write(&Buff_Send, sizeof(Buff_Send))) { //Write to the FIFO buffers, also useds dynamic payload size
     Transmission_error++;                      //Keep count of failed payloads

     cout << "\nTransmission error number: " ;
     cout << Transmission_error ; 

     if( Transmission_error > 9 || Transmissiontime == true){ // keeps trying to send data for 5 seconds
       cout << "\nChecking if other Radio is Transmitting " ;
       RXF(); // change to 
    
       if(radio.available()){
         cout << "\nother radio transmitting waiting for available transmission slot" ;
         RECEIVE();
        }
         
        TXF();
        receiving = false;
       }
     }else{
          cout << "\nTransmission Successful\n" ;
          cout << "\nPacket Size: " ;
          cout << sizeof(Wireless_Send) ;
          cout << "\nPacket Length: ";
          cout << Wireless_Send.Data.length();  
          cout << "\ntransmitted: ";
          cout << Wireless_Send.Data ;
          cout << "\n" ;
          // Resets values for next Transmission
          UnsentData = false;
          Transmissiontime = false;
          Transmission_error = 0;
          }
    
     if(Transmission_error == 0) { // checks if there is a error while transmission of data 
      RXF();
      }else{
           stopTime = millis();
           delay(100); 
           
           if( stopTime - startTime  > 5000){ // did time to send take longer then 5 seconds
             delay(100);
             cout << "\nTime Taken: " ;
             cout << stopTime - startTime ;
             cout << "\nTransmition is taking too long" ;
             Transmissiontime = true;
             }
            }
  
}else{ // Runs if Packet failed to make aending criteria
      PacketSizeError = 0;
      delay(1000);
     }
}



void Serialread(void) { // Serial override

    cout << "Options, T or R" ;
    cin >> Serialdata ;

    Serialdata = toupper(Serialdata) ;
    if(Serialdata == 'T' && Role == RX){
      TXF();
    }else if(Serialdata == 'R' && Role == TX ){
            RXF();
           }
   


}






void setup(void) {

  //Setup and configure rf radio//
  cout << "Initialising embedded software\n" ; // Debug for when the start up function runs
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

  cout << "Initialising Main Program\n";
  cout << "defaulting RECEIVE State\n";
  cout << "\n *** R=RECEIVE | T=TRANSMIT ***\n";

  delay(1000);

  // debug
}
void loop(void) {
  Serialread(); // read if there is Serial information
  // Transmission Mode change
  if(UnsentData == true){ // Looks if there is data that needs to be tranmitted
     if(Role == TX){
     // do nothing as it will send data anyway also dont reset connect to other radio
     }else{
          TXF(); // makesure role to TX so that it can set
          }
   }

  //TX
  if(Role == TX){
    TRANSMIT();
   } // TX END
  //RX
  else if(Role == RX){
        RECEIVE();
        } 
} // end of loop





int main(){
        setup();
        while(1)
                loop();
        return 0;
}
