<<<<<<< HEAD
     
/*
  Date 2020/8/20
  Title: ROMS 4 release v1
  REPO: https://github.com/OLLYDOTDEV/Project-Birdseye-DTX-2020
  Description: create a testing platform for sendeding remote local data from one
  this code is derived from other test codes 
*/

#include "RF24.h"
#include "printf.h"

//  |Config|

String Mode = "OFF";

RF24 radio(10, 9); // Set up nRF24L01 (makes OOP object) || RF24(_cepin _cspin )
// include needed libraries
#include <SPI.h>
 
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };   // radio address

// 1 byte can hold 1 character , there for the max amount of data that can be sent in one packect witht the NRF24 is 4bytes worth of character

bool TX = 1, RX = 0, Role = 0; // assign bool value to text representatives

// error values
byte Transmission_error = 0;
byte PacketSizeError = 0;

unsigned long startTime, stopTime;

bool Transmissiontime = false; // true means that the radio has been trying to tranmit for to long and failed
bool JustTransmitted = false;

//---------
bool UnsentData = false;
char Serialdata = "0";

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

// Sensor Variblers 

#include "Smooth.h"
   // count variable 
      int _ValuecCount = 0;           // count first amount of values while less then StartSkip
    int _StartSkip = 11;            // amount for ^^^ to skip
  // IR variables
  bool IR_Status = 0;
  int IR_DATA = 1000 ;
  int IR_Threshold = 900 ;
  int IR_OUT = A3; // connect ir sensor to arduino pin 2
  bool IR_Enabled;
  // PIR variables
  bool Pir_Status = 0;
  bool Pir_Data = 0;
  int Pir_Out = 4;
  bool Pir_Enabled;
  // general sensor variables 
 String  Alert_Status;
  int calibration_trigger = 1; // 1 = calibration mode on | 0 = calibration mode off
 AnalogSmooth IRsensor; // contruct an object with the custom smooth OOP class



// |---------|
// |Functions|
// |---------|

void TXF() {
  Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK *** "));
  radio.stopListening();                  // Stop listening
  delay(10);
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  Role = TX;
  delay(1000);
}
void RXF() {
  Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK *** "));
  delay(10);
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);
  Role = RX;
  radio.startListening();                  // Start listening
  delay(1000);
}

void RECEIVE(){
if(Role == TX){
    RXF();
   } 
   
 if(radio.available()) { // if there is information get prep for incomming otherwise 
    while(radio.available()){ // loop to read all the information in FIFO BUS
         radio.read(&Buff_Receive,sizeof(Buff_Receive));   
         received = true;
         Serial.println("receiving...\n");
        }
             
    String TempHeader(Buff_Receive.Header); // This Varible must be declare here so that the String constructor is called
    Wireless_Receive.Header = TempHeader; 

    String TempData(Buff_Receive.Data); // This Varible must be declare here so that the String constructor is called
    Wireless_Receive.Data = TempData;

        if(received == true){   
        Serial.print("Received Header: ");
        Serial.println(Wireless_Receive.Header);

        Serial.print("Received Data: ");
        Serial.println(Wireless_Receive.Data);

        if(Wireless_Receive.Header == "MODE"){
          Mode = Wireless_Receive.Data;
          SecurityMode();
           Serial.print("Received a Mode Packet");
          }else if( Wireless_Receive.Header == "PING"){
                    Serial.print("Received a Ping Packet\n");
      // no nothing as everthing for this is all ready done due to this is just to check if the radios are connected 
      }else{
              Serial.println("Invalid Packet") ;  
        }
        received = false;
       }
  }else{
       delay(100);
      Serial.println("\nNothing to Read in NRF24 Buffer\n");
       }   
}

void TRANSMIT(String header, String data ){ // returns `true` if transmission successfully  
  if(Role == RX){
    TXF();
   } 

Wireless_Send.Header = header;
Wireless_Send.Data = data;

 // Checking for errors the packet that is to be sent
 if(Wireless_Send.Data.length() <= sizeof(Buff_Send.Data) && Wireless_Send.Header.length() <= sizeof(Buff_Send.Header)){ // Stop overloading Char array with to large sized string
 
     for (unsigned int i = 0;i <= Wireless_Send.Header.length(); i++) {
    Buff_Send.Header[i] = Wireless_Send.Header[i]; // String to char Array  
   }
  
   for (unsigned int i = 0;i <= Wireless_Send.Data.length(); i++) {
    Buff_Send.Data[i] = Wireless_Send.Data[i]; // String to char Array  
   }

  for (unsigned int i=0; i<strlen(Buff_Send.Header); i++)
     Buff_Send.Header[i] = toupper(Buff_Send.Header[i]);

  for (unsigned int i=0; i<strlen(Buff_Send.Data); i++)
     Buff_Send.Data[i] = toupper(Buff_Send.Data[i]);
     
 }else{
      PacketSizeError = 1;
      Serial.println( "\n\n\n Warning failed to convert string to array: string to large, please make sure that the string is smaller then char array \n\n\n" );

      }
    
 if(sizeof(Buff_Send)> 32){ // add check for length of all varible in Datapack and make sure they are also under 32 
   PacketSizeError = 1;
   Serial.print("Buff Size: ") ;
    Serial.println(sizeof(Buff_Send)) ;
  Serial.println( "Failed to Send, Data Structure to Large\n") ;
  }

  
if(PacketSizeError == 0){
  
 Serial.println("\nPacket Integrity Verified \n\nTransmitting...") ; 
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

      Serial.print(  "\nTransmission error number: ") ;
     Serial.println( Transmission_error ); 

     if( Transmission_error > 9 || Transmissiontime == true){ // keeps trying to send data for 5 seconds
      Serial.print( "\nChecking if other Radio is Transmitting ") ;
       RXF(); // change to 
        delay(10);
       if(radio.available()){
          Serial.println( "\nother radio transmitting waiting for available transmission slot" );
         RECEIVE();
        }
         
        TXF();
        receiving = false;
        return false; // didnt get ack responce 
       }
     }else{

          Serial.print( "\nTransmitted header: ");
           Serial.println( Wireless_Send.Header);
           Serial.print("\nTransmitted data: ");
           Serial.println(Wireless_Send.Data) ;

          // Resets values for next Transmission
          UnsentData = false;
          Transmissiontime = false;
          Transmission_error = 0;
          RXF();
          return true; // received ack responce 
          }
    
     if(Transmission_error == 0) { // checks if there is a error while transmission of data 
      RXF();
      }else{
           stopTime = millis();
           delay(100); 
           
           if( stopTime - startTime  > 5000){ // did time to send take longer then 5 seconds
             delay(100);
            Serial.println( "\nTime Taken: " );
             Serial.println( stopTime - startTime );
              Serial.println( "\nTransmition is taking too long") ;
             Transmissiontime = true;
              return false; // didnt get ack responce 
             }
            }
  
}else{ // Runs if Packet failed to make aending criteria
      PacketSizeError = 0;
      delay(1000);
     }
}



void Serialread(void) { // Serial override
  if(Serial.available()){
    Serialdata = toupper(Serial.read());
    if(Serialdata == 'T' && Role == RX){
      TXF();
    }else if(Serialdata == 'R' && Role == TX ){
            RXF();
           }
   }
}


void SecurityMode(){

if(Mode == "IR"){
    // enable only IR sensors
       IR_Enabled = true;
       Pir_Enabled = false; 
}else if(Mode == "PIR"){
     // enable only PIR sensors
       IR_Enabled = false;
       Pir_Enabled = true;
}else if(Mode == "ALL"){
     // enable all sensors
       IR_Enabled = true;
       Pir_Enabled = true;
}else if(Mode == "OFF"){
  // disable alert system do nothing 
      IR_Enabled = false;
      Pir_Enabled = false;
}else{
Serial.println("Invalid Mode");  
}
Serial.print("\n\n\nSet mode to:");
Serial.print(Mode);
Serial.print("\n\n\n");
delay(2000);
}



// sersor debug functions 
void SensorValueDebug(){ 
  // value debug
Serial.println("\n");
Serial.print("IR Data: ");
Serial.println(IR_DATA);
Serial.print("PIR Data: ");
Serial.println(Pir_Data);
}
void SensorAlertDebug(){
// alert debug
Serial.println("\n");
Serial.print("IR Status: ");
Serial.println(IR_Status);
Serial.print("PIR Status: ");
Serial.println(Pir_Status);
Serial.print("\nAlert_Status: ");
Serial.println(Alert_Status);
}

void GetSensorData(){


if(IR_Enabled == true){ // check if subsystem is allowed to read this sensor's data
// IR code 
if(_ValuecCount < _StartSkip){
  IRsensor.Smooth(IR_OUT); // triggers method but doesnt assign output to variable
 Serial.print("Skipping start value number:");
 Serial.println(_ValuecCount);
 _ValuecCount++;
}else{
IR_DATA = IRsensor.Smooth(IR_OUT); // triggers method and assign output to variable
}
if(IR_DATA < IR_Threshold){
// if this if statement activates that means that the sensor has been triggered  
IR_Status = 1;
}else{
IR_Status = 0;  
}
}

if(Pir_Enabled == true){ 
// pir code
Pir_Data = digitalRead(Pir_Out);
if(Pir_Data == 1){ // if PIR sensor actavated then set the alert status to active (true)
Pir_Status=1;
}else{
Pir_Status=0;  
}
}

if(Pir_Status || IR_Status == 1){ // check sensor data
Alert_Status = "ACTIVE";
}else{
Alert_Status = "OFF";
// no sensor actived no nothing
}
}  



void setup(){

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
  // debug



// sensor setup
IR_Enabled = true;
Pir_Enabled = true;
Alert_Status = "OFF";
// Initiate Mode
Mode = "OFF";
SecurityMode();
// sensor pins to INPUT
  pinMode (Pir_Out, INPUT); 
  pinMode (IR_OUT, INPUT); 





// --------------
    Serial.println("Initialising Main Program");
  Serial.println("defaulting OFF|RECEIVE State");


  delay(1000);

}
void loop(void) {

  if(Mode == "OFF"){
     Serial.println("\n Mode:OFF");
     RECEIVE();  
   }else{
  

 if(Alert_Status == "ACTIVE" ){
TRANSMIT("ALERT",Alert_Status);
while(Alert_Status == "ACTIVE" ){
  GetSensorData();
 SensorValueDebug();
 SensorAlertDebug();   
 }
JustTransmitted = true;
}
if(JustTransmitted == true){
 if(Alert_Status == "OFF"  ){
    TRANSMIT("ALERT",Alert_Status); 
    JustTransmitted = false;
  }
} 
  
  RECEIVE();  
 GetSensorData();
 SensorValueDebug();
 SensorAlertDebug();
}
} // end of loop
=======
     
/*
  Date 2020/8/20
  Title: ROMS 4 release v1
  REPO: https://github.com/OLLYDOTDEV/Project-Birdseye-DTX-2020
  Description: create a testing platform for sendeding remote local data from one
  this code is derived from other test codes 
*/

#include "RF24.h"
#include "printf.h"

//  |Config|

String Mode = "OFF";

RF24 radio(10, 9); // Set up nRF24L01 (makes OOP object) || RF24(_cepin _cspin )
// include needed libraries
#include <SPI.h>
 
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };   // radio address

// 1 byte can hold 1 character , there for the max amount of data that can be sent in one packect witht the NRF24 is 4bytes worth of character

bool TX = 1, RX = 0, Role = 0; // assign bool value to text representatives

// error values
byte Transmission_error = 0;
byte PacketSizeError = 0;

unsigned long startTime, stopTime;

bool Transmissiontime = false; // true means that the radio has been trying to tranmit for to long and failed
bool JustTransmitted = false;

//---------
bool UnsentData = false;
char Serialdata = "0";

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

// Sensor Variblers 

#include "Smooth.h"
   // count variable 
      int _ValuecCount = 0;           // count first amount of values while less then StartSkip
    int _StartSkip = 11;            // amount for ^^^ to skip
  // IR variables
  bool IR_Status = 0;
  int IR_DATA = 1000 ;
  int IR_Threshold = 900 ;
  int IR_OUT = A3; // connect ir sensor to arduino pin 2
  bool IR_Enabled;
  // PIR variables
  bool Pir_Status = 0;
  bool Pir_Data = 0;
  int Pir_Out = 4;
  bool Pir_Enabled;
  // general sensor variables 
 String  Alert_Status;
  int calibration_trigger = 1; // 1 = calibration mode on | 0 = calibration mode off
 AnalogSmooth IRsensor; // contruct an object with the custom smooth OOP class



// |---------|
// |Functions|
// |---------|

void TXF() {
  Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK *** "));
  radio.stopListening();                  // Stop listening
  delay(10);
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  Role = TX;
  delay(1000);
}
void RXF() {
  Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK *** "));
  delay(10);
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);
  Role = RX;
  radio.startListening();                  // Start listening
  delay(1000);
}

void RECEIVE(){
if(Role == TX){
    RXF();
   } 
   
 if(radio.available()) { // if there is information get prep for incomming otherwise 
    while(radio.available()){ // loop to read all the information in FIFO BUS
         radio.read(&Buff_Receive,sizeof(Buff_Receive));   
         received = true;
         Serial.println("receiving...\n");
        }
             
    String TempHeader(Buff_Receive.Header); // This Varible must be declare here so that the String constructor is called
    Wireless_Receive.Header = TempHeader; 

    String TempData(Buff_Receive.Data); // This Varible must be declare here so that the String constructor is called
    Wireless_Receive.Data = TempData;

        if(received == true){   
        Serial.print("Received Header: ");
        Serial.println(Wireless_Receive.Header);

        Serial.print("Received Data: ");
        Serial.println(Wireless_Receive.Data);

        if(Wireless_Receive.Header == "MODE"){
          Mode = Wireless_Receive.Data;
          SecurityMode();
           Serial.print("Received a Mode Packet");
          }else if( Wireless_Receive.Header == "PING"){
                    Serial.print("Received a Ping Packet\n");
      // no nothing as everthing for this is all ready done due to this is just to check if the radios are connected 
      }else{
              Serial.println("Invalid Packet") ;  
        }
        received = false;
       }
  }else{
       delay(100);
      Serial.println("\nNothing to Read in NRF24 Buffer\n");
       }   
}

void TRANSMIT(String header, String data ){ // returns `true` if transmission successfully  
  if(Role == RX){
    TXF();
   } 
   Serial.println("\n Transmitting\n");
Wireless_Send.Header = header;
Wireless_Send.Data = data;

 // Checking for errors the packet that is to be sent
 if(Wireless_Send.Data.length() <= sizeof(Buff_Send.Data) && Wireless_Send.Header.length() <= sizeof(Buff_Send.Header)){ // Stop overloading Char array with to large sized string
 
     for (unsigned int i = 0;i <= Wireless_Send.Header.length(); i++) {
    Buff_Send.Header[i] = Wireless_Send.Header[i]; // String to char Array  
   }
  
   for (unsigned int i = 0;i <= Wireless_Send.Data.length(); i++) {
    Buff_Send.Data[i] = Wireless_Send.Data[i]; // String to char Array  
   }

  for (unsigned int i=0; i<strlen(Buff_Send.Header); i++)
     Buff_Send.Header[i] = toupper(Buff_Send.Header[i]);

  for (unsigned int i=0; i<strlen(Buff_Send.Data); i++)
     Buff_Send.Data[i] = toupper(Buff_Send.Data[i]);
     
 }else{
      PacketSizeError = 1;
      Serial.println( "\n\n\n Warning failed to convert string to array: string to large, please make sure that the string is smaller then char array \n\n\n" );

      }
    
 if(sizeof(Buff_Send)> 32){ // add check for length of all varible in Datapack and make sure they are also under 32 
   PacketSizeError = 1;
   Serial.print("Buff Size: ") ;
    Serial.println(sizeof(Buff_Send)) ;
  Serial.println( "Failed to Send, Data Structure to Large\n") ;
  }

  
if(PacketSizeError == 0){
  
 Serial.println("\nPacket Integrity Verified \n\nTransmitting...") ; 
  // handles transmission errors
  if(Transmission_error > 9){ // if error count great then 10 reset value
    Transmission_error = 0; 
   }
  if(Transmission_error == 0){
    startTime = millis();
    Transmissiontime = false;
   }
    // Sends data to Radio and then give feedback
   if(!radio.writeFast(&Buff_Send, sizeof(Buff_Send))) { //Write to the FIFO buffers, also useds dynamic payload size
     Transmission_error++;                      //Keep count of failed payloads

      Serial.print(  "\nTransmission error number: ") ;
     Serial.println( Transmission_error ); 

     if( Transmission_error > 9 || Transmissiontime == true){ // keeps trying to send data for 5 seconds
      Serial.print( "\nChecking if other Radio is Transmitting ") ;
       RXF(); // change to 
        delay(10);
       if(radio.available()){
          Serial.println( "\nother radio transmitting waiting for available transmission slot" );
         RECEIVE();
        }
         
        TXF();
        receiving = false;
        return false; // didnt get ack responce 
       }
     }else{

          Serial.print( "\nTransmitted header: ");
           Serial.println( Wireless_Send.Header);
           Serial.print("\nTransmitted data: ");
           Serial.println(Wireless_Send.Data) ;

          // Resets values for next Transmission
          UnsentData = false;
          Transmissiontime = false;
          Transmission_error = 0;
          RXF();
          return true; // received ack responce 
          }
    
     if(Transmission_error == 0) { // checks if there is a error while transmission of data 
      RXF();
      }else{
           stopTime = millis();
           delay(100); 
           
           if( stopTime - startTime  > 5000){ // did time to send take longer then 5 seconds
             delay(100);
            Serial.println( "\nTime Taken: " );
             Serial.println( stopTime - startTime );
              Serial.println( "\nTransmition is taking too long") ;
             Transmissiontime = true;
              return false; // didnt get ack responce 
             }
            }
  
}else{ // Runs if Packet failed to make aending criteria
      PacketSizeError = 0;
      delay(1000);
     }
}



void Serialread(void) { // Serial override
  if(Serial.available()){
    Serialdata = toupper(Serial.read());
    if(Serialdata == 'T' && Role == RX){
      TXF();
    }else if(Serialdata == 'R' && Role == TX ){
            RXF();
           }
   }
}


void SecurityMode(){

if(Mode == "IR"){
    // enable only IR sensors
       IR_Enabled = true;
       Pir_Enabled = false; 
}else if(Mode == "PIR"){
     // enable only PIR sensors
       IR_Enabled = false;
       Pir_Enabled = true;
}else if(Mode == "ALL"){
     // enable all sensors
       IR_Enabled = true;
       Pir_Enabled = true;
}else if(Mode == "OFF"){
  // disable alert system do nothing 
      IR_Enabled = false;
      Pir_Enabled = false;
}else{
Serial.println("Invalid Mode");  
}
Serial.print("\n\n\nSet mode to:");
Serial.print(Mode);
Serial.print("\n\n\n");
delay(2000);
}



// sersor debug functions 
void SensorValueDebug(){ 
  // value debug
Serial.println("\n");
Serial.print("IR Data: ");
Serial.println(IR_DATA);
Serial.print("PIR Data: ");
Serial.println(Pir_Data);
}
void SensorAlertDebug(){
// alert debug
Serial.println("\n");
Serial.print("IR Status: ");
Serial.println(IR_Status);
Serial.print("PIR Status: ");
Serial.println(Pir_Status);
Serial.print("\nAlert_Status: ");
Serial.println(Alert_Status);
}

void GetSensorData(){


if(IR_Enabled == true){ // check if subsystem is allowed to read this sensor's data
// IR code 
if(_ValuecCount < _StartSkip){
  IRsensor.Smooth(IR_OUT); // triggers method but doesnt assign output to variable
 Serial.print("Skipping start value number:");
 Serial.println(_ValuecCount);
 _ValuecCount++;
}else{
//IR_DATA = IRsensor.Smooth(IR_OUT); // triggers method and assign output to variable
IR_DATA = analogRead(IR_OUT);
}
if(IR_DATA < IR_Threshold){
// if this if statement activates that means that the sensor has been triggered  
IR_Status = 1;
}else{
IR_Status = 0;  
}
}

if(Pir_Enabled == true){ 
// pir code
Pir_Data = digitalRead(Pir_Out);
if(Pir_Data == 1){ // if PIR sensor actavated then set the alert status to active (true)
Pir_Status=1;
}else{
Pir_Status=0;  
}
}

if(Pir_Status || IR_Status == 1){ // check sensor data
Alert_Status = "ACTIVE";
}else{
Alert_Status = "OFF";
// no sensor actived no nothing
}
}  



void setup(){

  //Setup and configure rf radio//

  Serial.begin(115200);
  delay(2000); // allow time for start up
  printf_begin();
  Serial.println("Initialising embedded software"); // Debug for when the start up function runs
  radio.begin();  // called function to setup the radio.

  radio.setChannel(125); // select sport portion of the 2.4 gigahertz Spectrum it is broadcasting on in this case it is selected above the frequency of 2.4Ghz Wi-Fi thus was will not received interference.
  radio.setPALevel(RF24_PA_MAX); // will be reaplaced with RF24_PA_MAX for larger range & penetration

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
  // debug
RECEIVE();  


// sensor setup
IR_Enabled = true;
Pir_Enabled = true;
Alert_Status = "OFF";
// Initiate Mode
Mode = "OFF";
SecurityMode();
// sensor pins to INPUT
  pinMode (Pir_Out, INPUT); 
  pinMode (IR_OUT, INPUT); 





// --------------
    Serial.println("Initialising Main Program");
  Serial.println("defaulting OFF|RECEIVE State");


  delay(1000);

}
void loop(void) {

  if(Mode == "OFF"){
     Serial.println("\n Mode:OFF");
     RECEIVE();  
   }else{
  

 if(Alert_Status == "ACTIVE" ){
TRANSMIT("ALERT",Alert_Status);
while(Alert_Status == "ACTIVE" ){
  delay(10);
  TRANSMIT("ALERT",Alert_Status);
  GetSensorData();
 SensorValueDebug();
 SensorAlertDebug();   if(Alert_Status == "OFF"){
    TRANSMIT("ALERT",Alert_Status); 
 }
 }
if(Alert_Status == "OFF"){
    TRANSMIT("ALERT",Alert_Status); 
 }

 }

  
  RECEIVE();  
 GetSensorData();
 SensorValueDebug();
 SensorAlertDebug();
}
} // end of loop
>>>>>>> 558a8c8... fixed OFF signal not being sent
