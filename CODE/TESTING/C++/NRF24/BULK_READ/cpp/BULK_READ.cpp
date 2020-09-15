/* NRF24 Test by OLLYDOTDEV
 *   
 *  
 */


#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>


using namespace std;

RF24 radio(22,0);



const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };
int radioread = 0;
bool received = false;




typedef struct
{
   string Data;

}
DataDef;
DataDef DataPak;




typedef struct
{
   
char Data[32];

}
BuffDef;
BuffDef BuffPak;



void setup(void) {
  delay(2000);
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

void loop(void) {
 while (radio.available()) {
      radio.read(&BuffPak, sizeof(BuffPak));
      received = true;
 }
    string TempBuffstring(BuffPak.Data); // This Varible must be declare here
      DataPak.Data = TempBuffstring;

 
//     DataPak.Data.remove(DataPak.Data.length()-1,1); not all way needed || remove last character from string


        
      cout << "Packet Size: ";
      cout << sizeof(BuffPak.Data);
      cout << "\n";

      cout << "Data Received: ";  
      cout << BuffPak.Data ;
      cout << "\n\n";



if(received == true){



  memset(BuffPak.Data, 0, 32*sizeof(*BuffPak.Data)); // clear char array || // https://stackoverflow.com/questions/9146395/reset-c-int-array-to-zero-the-fastest-way 

cout << "Clearing Array\n";  
received = false;
delay(500);
}
}


int main(){
        setup();
        while(1)
                loop();
        return 0;
}