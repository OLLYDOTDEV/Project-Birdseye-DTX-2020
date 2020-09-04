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
   
char Data[32];

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



 while (radio.available()) {
      radio.read(&BuffPak, sizeof(BuffPak));
      received = true;

 }


  

    
    String TempBuffString(BuffPak.Data); // This Varible must be declare here
      DataPak.Data = TempBuffString;

 
//     DataPak.Data.remove(DataPak.Data.length()-1,1); not all way needed || remove last character from string


        
      Serial.print("Packet Size: ");
        Serial.println(sizeof(DataPak));
 //   Serial.print("Packet Length: ");
  //  Serial.println(DataPak.Data.length());  


        Serial.print("Data Received: ");  
   Serial.println(BuffPak.Data);
  Serial.println("\n\n");



if(received == true){



  memset(BuffPak.Data, 0, 32*sizeof(*BuffPak.Data)); // clear char array || // https://stackoverflow.com/questions/9146395/reset-c-int-array-to-zero-the-fastest-way 

Serial.println("Clearing Array\n");  
received = false;
delay(500);
}
}
