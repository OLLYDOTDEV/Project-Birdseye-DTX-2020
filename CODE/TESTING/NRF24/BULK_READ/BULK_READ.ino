/* Test NRF24 by 
 *   
 *  
 */


#include <SPI.h>
#include "RF24.h"
#include "printf.h"
RF24 radio(10,9);



const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };
int radioread = 0;
bool received = false;




typedef struct
{
   String Data;

}
DataDef;
DataDef DataPak;




typedef struct
{
   
char Data[31];

}
BuffDef;
BuffDef BuffPak;



void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
 delay(2000);
  printf_begin();
  radio.begin(); 
  
  radio.setChannel(125);
  radio.setPALevel(RF24_PA_LOW);           // If you want to save power use "RF24_PA_MIN" but keep in mind that reduces the module's range
  
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(false);                    
  
  radio.setRetries(15, 15);    
  radio.setCRCLength(RF24_CRC_8);  // Cyclic redundancy check used for error-detecting
  
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);

  radio.startListening();                  // Start listening
  radio.printDetails();                    // Dump the configuration of the rf unit for debugging

  radio.powerUp();    
}

void loop() {


delay(2000);


 while (radio.available()) {
      radio.read(&BuffPak, sizeof(BuffPak));
      received = true;

 }

    String TempBuffString(BuffPak.Data); // This Varible must be 
      DataPak.Data = TempBuffString;

        Serial.print("Data Received: ");  
   Serial.println(DataPak.Data);
  Serial.println("\n\n");



if(received == true){

  for (byte i = 0;i <= 31; i++) {
    BuffPak.Data[i] = '0'; // String to char Array

 
  }

Serial.println("Clearing Array");  
received = false;
delay(500);
}
}
