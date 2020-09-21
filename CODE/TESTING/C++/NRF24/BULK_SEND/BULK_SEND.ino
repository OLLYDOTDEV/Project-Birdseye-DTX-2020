/* Test NRF24 By just sending Data at the SPI Connection
 *   
 *  
 */


#include <SPI.h>
#include "RF24.h"
#include "printf.h"
RF24 radio(10,9);

const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };


typedef struct // only for strings
{ 
   String Data = "12341234123412341234123412341234";
   // String Data = "TESTGAMETESTGAMETESTGAMETESTGAME12345TESTGAMETESTGAMETESTGAMETESTGAME12345";
   // String Data = "TESTGAME";
}
DataDef;
DataDef DataPak;

  


typedef struct   // max of 32 Char objects with in this struct as the NRF24 radio and only send 32 bytes
{
   
char Data[32];

}
BuffDef;
BuffDef BuffPak;


byte Error = 0;

void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
  delay(2000);
  printf_begin();
  radio.begin(); 
  radio.setChannel(125);
  radio.setPALevel(RF24_PA_LOW);           // If you want to save power use "RF24_PA_MIN" but keep in mind that reduces the module's range
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(false);                     // Ensure autoACK is disabled
  radio.setRetries(15, 15);    
  radio.setCRCLength(RF24_CRC_8);  // Cyclic redundancy check used for error-detecting
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);


 
  Serial.println(F("\\BULK INFO Spam\\"));
  
  
    radio.printDetails();   
    radio.powerUp();                         //Power up the radio
  
}

void loop() {
  // put your main code here, to run repeatedly:

  if(DataPak.Data.length() <= sizeof(BuffPak.Data)){ // Stop overloading Char array with to large sized string
  for (byte i = 0;i <= DataPak.Data.length(); i++) {
    BuffPak.Data[i] = DataPak.Data[i]; // String to char Array  
  }
  }else{
    Error = 1;
     Serial.println("\n\n\n");
      Serial.println("Warning failed to convert string to array: String to large, please make sure that the String is smaller then char array ");
       Serial.println("\n\n\n");
    }

  if(sizeof(BuffPak)> 32){ // add check for length of all varible in Datapak and make sure they are also under 32 
  Error = 1;
   Serial.print("Buff Size: ");
  Serial.println(sizeof(BuffPak));
 Serial.println("Failed to Send, Data Structure to Large");
  }
  
  if(Error == 0){
   Serial.print("Buff Size: ");
  Serial.println(sizeof(BuffPak.Data));
  
  Serial.print("Sending: ");
  Serial.println(BuffPak.Data);
  
radio.writeFast(&BuffPak, sizeof(BuffPak));
  radio.txStandBy(100);

 Serial.println("\n\n\n");
 delay(500);
}else{
Error = 0;
delay(1000);
}
}
