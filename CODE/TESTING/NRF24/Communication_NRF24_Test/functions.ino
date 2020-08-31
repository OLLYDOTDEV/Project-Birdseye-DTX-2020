// .ino file are appended to each other alphabetically
// forsake of readability ability this is being split into a separate file for just functions 


// |---------|
// |Functions|
// |---------|


// 

// Role change
void TRANSMIT(){
   Serial.println("");
    
    byte wireless_send[4];  // store data to be transmitted
    error = 0; // reset if there has been a error
    

    wireless_send[0] = 1 ;
    wireless_send[1] = 2 ; 
    wireless_send[2] = 3 ; 
    wireless_send[3] = 4 ;
    
 Serial.println("Transmitting...");
    if (!radio.write(&wireless_send, 4)) { //Write to the FIFO buffers, also useds dynamic payload size
      error = 1 ;                      //Keep count of failed payloads
      Serial.println("Transmission error");
       if(0 == radio.txStandBy(10000) ){ // keeps trying to send data for 5 seconds
        Serial.println("Unable to Transmit ");
        Serial.println("Checking to other Radio is Transmitting ");
         RXF(); // change to 
         
         while(receiving == true){
         Serial.println("other radio transmitting waiting for available transmission slot");
         RECEIVE();
         }
         
         TXF();
         receiving = false;
       }
     }else{
     Serial.println("Transmission Successful\n");
     UnsentData == false;
     }
    
    if (error == 0) { // checks if there is a error while transmission of data
      RXF();
    }else{
     }
}

void RECEIVE(){
    byte wireless_receive[4];  // store data that has been received
    if (radio.available()) { // if there is information get prep for incomming otherwise 
      while (radio.available()) { // loop to read all the information in FIFO BUS
        radio.read(&wireless_receive, 4);   
        received = true;
        for (int i = 0; i < 4; i++) {
          radioread = wireless_receive[i];                //Load the buffer with random data
          Serial.print("Data Received: ");
          Serial.println(radioread);
        }
        Serial.println("\n\n");
        for (int i = 0; i < 4; i++) { // clears data
          wireless_receive[i] = 0;
        }

      }
      if (received == true) {
        Serial.println("Clearing Array\n\n\n");
        received = false;
       

      }
  } else {
  delay(100);
  Serial.println("\nNothing to Read in NRF24 Buffer\n");

  }  
}
void TXF() {
  Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
  radio.stopListening();                  // Stop listening
  delay(10);
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  Role = TX;
  delay(1000);
}
void RXF() {
  Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
  delay(10);
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);
  Role = RX;
  radio.startListening();                  // Start listening
  delay(1000);
}

void Serialread(void) { // Serial override
  if ( Serial.available()) {
    Serialdata = toupper(Serial.read());
    if (Serialdata == 'T' && Role == RX) {
      TXF();

    } else if (Serialdata == 'R' && Role == TX ) {
      RXF();
    }
  }
}
