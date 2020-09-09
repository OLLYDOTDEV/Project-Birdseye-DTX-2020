 
/*
  Date 2020/8/20
  Title: RPI 4 release v1
  REPO: https://github.com/OLLYDOTDEV/Project-Birdseye-DTX-2020
  Description: full functional outcome 
  this code is derived from other test codes 
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
RF24 radio(22,0); // Set up nRF24L01 (makes OOP object) || RF24(cepin cspin )
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
  String Header;
  String Data; 
  }
Wireless_ReceiveDef;
Wireless_ReceiveDef Wireless_Receive;


typedef struct // for Wireless_Recieve Buffer 
{ 

  char Header[6];
  char Data[26];
  }
Buff_ReceiveDef ;
Buff_ReceiveDef Buff_Receive;

// --------------

typedef struct // for Wireless_Send packet 
{
  String Header;
  String Data; 
  }
Wireless_SendDef;
Wireless_SendDef Wireless_Send;

typedef struct // for Wireless_Send Buffer 
{
  char Header[6];
  char Data[26];
  }
Buff_SendDef;
Buff_SendDef Buff_Send;

String Header; 
String Data;

bool connection = false ;





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

    string TempHeaderHeader(Buff_Receive.Header); // This Varible must be declare here so that the string constructor is called
    Wireless_Receive.Header = TempHeaderHeader; 

    string TempHeaderData(Buff_Receive.Data); // This Varible must be declare here so that the string constructor is called
    Wireless_Receive.Data = TempHeaderData;

      if(received == true){   
        cout << "\nReceived" ;    
        cout << "\nHeader: " ;
        cout << Wireless_Receive.Header ;
        cout << "\nData: " ;
        cout << Wireless_Receive.Data ;        
        received = false;
       }
  }else{
       delay(100);
      cout << "\nNothing to Read in NRF24 Buffer\n";
       }   
}

bool TRANSMIT(string header, string data ){ // returns `true` if transmission successfully  

Wireless_Send.Header = toupper(header) ;
Wireless_Send.Data = toupper(data) ;


 cout << "\nChecking Packet Integrity\n"; 
 // Checking for errors the packet that is to be sent
 if(Wireless_Send.Data.length() <= sizeof(Buff_Send.Data)|| Wireless_Send.Header.length() <= sizeof(Wireless_Send.Header))){ // Stop overloading Char array with to large sized string
 
     for (byte i = 0;i <= Wireless_Send.Header.length(); i++) {
    Buff_Send.Header[i] = Wireless_Send.Header[i]; // String to char Array  
   }
  
   for (byte i = 0;i <= Wireless_Send.Data.length(); i++) {
    Buff_Send.Data[i] = Wireless_Send.Data[i]; // String to char Array  
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
        delay(10);
       if(radio.available()){
         cout << "\nother radio transmitting waiting for available transmission slot" ;
         RECEIVE();
        }
         
        TXF();
        receiving = false;
        return false;
       }
     }else{

          cout << "\nTransmitted header: ";
          cout << Wireless_Send.Header;
          cout << "\n"; 


          cout << "\nTransmitted data: ";
          cout << Wireless_Send.Data ;
          cout << "\n";
          // Resets values for next Transmission
          UnsentData = false;
          Transmissiontime = false;
          Transmission_error = 0;
          return true;
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

// config for differcnt 
void ModeInterface(void){

// read .json file 
// will add get code to get modes from websever later 

Header = "ROLE" ;
Data = "IR"; // some role read from json || do to 
cout << "\n SENDING ROLE \n";
TRANSMIT(Header,Data);
}

void getstatus(){ // checks if ROMS if read to receive

Header = "Status" ;
Data = "CHECK"; // some role read from json || do to 
cout << "\n PINGING... \n";

while(connection == false){ // loop until connection is made with other radio
  if(TRANSMIT(Header,Data) == true ){ 
  connection == true;
  }
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

  cout << "\nAttempting to establish connection\n";
  
  getstatus();
  
  cout << "\nConnection Established \n\n\n";

  delay(500);

  cout << "\nInitialising Main Program\n";
  cout << "Defaulting RECEIVE State\n";


  delay(1000);

  // debug
}
void loop(void) {
  Serialread(); // read if there is Serial information
  
  // -// Transmission Mode change
  // if(UnsentData == true){ // Looks if there is data that needs to be tranmitted
  //    if(Role == TX){
  //    // do nothing as it will send data anyway also dont reset connect to other radio
  //    }else{
  //         TXF(); // makesure role to TX so that it can set
  //         }
  //  }

  // //TX
  // if(Role == TX){
  //   TRANSMIT();
  //  } // TX END
  // //RX
  // else if(Role == RX){
  //       RECEIVE();
  //       } 


} // end of loop

   



int main(){
        setup();
        while(1)
                loop();
        return 0;
}
