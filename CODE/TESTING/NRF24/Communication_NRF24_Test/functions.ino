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

    wireless_send[0] = 1 ;
    wireless_send[1] = 2 ; 
    wireless_send[2] = 3 ; 
    wireless_send[3] = 4 ;
Serial.println("Transmitting..."); 
if(error > 9){ // if error count great then 10 reset value
    error = 0; 
    }
if(error == 0){
   startTime = millis();
  }
    if (!radio.write(&wireless_send, sizeof(wireless_send))) { //Write to the FIFO buffers, also useds dynamic payload size
      error++;                      //Keep count of failed payloads

      Serial.print("Transmission error number: ");
       Serial.println(error); 

       if( error > 9 || Transmissiontime == true){ // keeps trying to send data for 5 seconds

        Serial.println("Checking if other Radio is Transmitting ");
         RXF(); // change to 
         
         if(radio.available()){
         Serial.println("other radio transmitting waiting for available transmission slot");
         RECEIVE();
         }
         
         TXF();
         receiving = false;
       }
     }else{
     Serial.println("Transmission Successful\n");
     UnsentData == false;
     Transmissiontime = false;
     error = 0;
     }
    
    if (error == 0) { // checks if there is a error while transmission of data
      
      RXF();
    }else{
      stopTime = millis();
      if(stopTime - startTime  > 5000)
      delay(100);
      Serial.print("Time Taken: ");
      Serial.println(stopTime - startTime );
  
      Serial.println("Transmition is taking too long");
      Transmissiontime = true;
     }
}

void RECEIVE(){
    byte wireless_receive[4];  // store data that has been received
    if (radio.available()) { // if there is information get prep for incomming otherwise 
      while (radio.available()) { // loop to read all the information in FIFO BUS
        radio.read(&wireless_receive, sizeof(wireless_receive));   
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
