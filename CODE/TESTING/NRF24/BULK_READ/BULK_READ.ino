/* Test NRF24 by 
 *   
 *  
 */


#include <SPI.h>
#include "RF24.h"
#include "printf.h"
RF24 radio(10,9);


byte data[4];       // buffer
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };
int radioread = 0;
bool received = false;
void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
 delay(2000);
  printf_begin();
  radio.begin(); 
  radio.setChannel(1);
  radio.setPALevel(RF24_PA_LOW);           // If you want to save power use "RF24_PA_MIN" but keep in mind that reduces the module's range
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(false);                    
  radio.setRetries(15, 15);    

  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);

  radio.startListening();                  // Start listening
  radio.printDetails();                    // Dump the configuration of the rf unit for debugging

  radio.powerUp();    
}

void loop() {




  
delay(2000);


 while (radio.available()) {
      radio.read(&data, 4);
      received = true;
  }

  for (int i = 0; i < 4; i++) {
   radioread = data[i];                //Load the buffer with random data
   Serial.print("Data Received: ");  
   Serial.println(radioread);
  }
  Serial.println("\n\n");
  for (int i = 0; i < 4; i++) { // clears data
    data[i] = 0;
  }
Serial.println("Clearing Array");  
received = false;
delay(1000);
 



}
