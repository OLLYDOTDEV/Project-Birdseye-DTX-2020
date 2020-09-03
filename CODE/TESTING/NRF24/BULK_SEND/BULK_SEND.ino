/* Test NRF24 By just sending Data at the SPI Connection
 *   
 *  
 */


#include <SPI.h>
#include "RF24.h"
#include "printf.h"
RF24 radio(10,9);

byte data[4];       // buffer
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };


typedef struct
{
   String Data = "Char test ";

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
typedef struct
{
   char Buff[32];

}
RadioBuffDef;
RadioBuffDef RadioBuffPak;
  
  
  for (byte i = 0;i <= DataPak.Data.length(); i++) {
    BuffPak.Data[i] = DataPak.Data[i]; // String to char Array  
  }

  
  Serial.print("Sending: ");
  Serial.println(BuffPak.Data);
  
radio.writeFast(&BuffPak, sizeof(BuffPak));
  radio.txStandBy(500);

 Serial.println("\n\n\n");
}
