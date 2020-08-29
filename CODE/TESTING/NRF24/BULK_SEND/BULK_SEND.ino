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
  for (int i = 0; i < 4; i++) {
    data[i] = i + 1;                //Load the buffer with random data
  }
    radio.printDetails();   
  radio.powerUp();                         //Power up the radio
  
}

void loop() {
  // put your main code here, to run repeatedly:


for (int i = 0; i < 4; i++) {
    
  Serial.print("Sending : ");
  Serial.println(data[i]);
  }
radio.writeFast(&data, 4);
  radio.txStandBy(1000);

 Serial.println("\n\n\n");
}
