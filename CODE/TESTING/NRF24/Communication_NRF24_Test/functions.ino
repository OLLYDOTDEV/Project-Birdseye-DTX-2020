// .ino file are appended to each other alphabetically
// forsake of readability ability this is being split into a separate file for just functions 


// |---------|
// |Functions|
// |---------|


// 

// Role change
void TRANSMIT(){
 // Wireless_Send.Data = "12341234123412341234123412341234"; // Set Max length temp data for testing
 Wireless_Send.Data = "Test Data " ;// set temp data for testing
 Serial.println("\nChecking Packet Integrity"); 
 // Checking for errors the packet that is to be sent
 if(Wireless_Send.Data.length() <= sizeof(Buff_Send.Data)){ // Stop overloading Char array with to large sized string
  
   for (byte i = 0;i <= Wireless_Send.Data.length(); i++) {
    Buff_Send.Data[i] = Wireless_Send.Data[i]; // String to char Array  
  }
  
 }else{
      PacketSizeError = 1;
      Serial.println("\n\n\n");
      Serial.println("Warning failed to convert string to array: String to large, please make sure that the String is smaller then char array ");
      Serial.println("\n\n\n");
      }
    
 if(sizeof(Buff_Send)> 32){ // add check for length of all varible in Datapak and make sure they are also under 32 
   PacketSizeError = 1;
   Serial.print("Buff Size: ");
   Serial.println(sizeof(Buff_Send));
   Serial.println("Failed to Send, Data Structure to Large");
  }

  
if(PacketSizeError == 0){
  
  Serial.println("\nPacket Integrity Verified \n\nTransmitting..."); 
  // handles transmission errors
  if(Transmission_error > 9){ // if error count great then 10 reset value
    Transmission_error = 0; 
   }
  if(Transmission_error == 0){
    startTime = millis();
    Transmissiontime == false;
   }
    // Sends data to Radio and then give feedback
   if(!radio.write(&Buff_Send, sizeof(Buff_Send))) { //Write to the FIFO buffers, also useds dynamic payload size
     Transmission_error++;                      //Keep count of failed payloads

     Serial.print("Transmission error number: ");
     Serial.println(Transmission_error); 

     if( Transmission_error > 9 || Transmissiontime == true){ // keeps trying to send data for 5 seconds
       Serial.println("\nChecking if other Radio is Transmitting ");
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
          Serial.print("Packet Size: ");
          Serial.println(sizeof(Wireless_Send));
          Serial.print("Packet Length: ");
          Serial.println(Wireless_Send.Data.length());  
          Serial.print("transmitted: ");
          Serial.println(Wireless_Send.Data);
          Serial.print("\n");
          // Resets values for next Transmission
          UnsentData == false;
          Transmissiontime = false;
          Transmission_error = 0;
          }
    
     if(Transmission_error == 0) { // checks if there is a error while transmission of data 
      RXF();
      }else{
           stopTime = millis();
           delay(100); 
           
           if( stopTime - startTime  > 5000){ // did time to send take longer then 5 seconds
             delay(100);
             Serial.print("Time Taken: ");
             Serial.println(stopTime - startTime );
             Serial.println("Transmition is taking too long");
             Transmissiontime = true;
             }
            }
  
}else{ // Runs if Packet failed to make aending criteria
      PacketSizeError = 0;
      delay(1000);
     }
}
void RECEIVE(){

 if(radio.available()) { // if there is information get prep for incomming otherwise 
    while(radio.available()){ // loop to read all the information in FIFO BUS
         radio.read(&Buff_Receive,sizeof(Buff_Receive));   
         received = true;
         Serial.println("receiving...\n");
        }
             
    String TempBuffString(Buff_Receive.Data); // This Varible must be declare here so that the String constructor is called
    Wireless_Receive.Data = TempBuffString;

    Serial.print("Packet Size: ");
    Serial.println(sizeof(Wireless_Receive));
    Serial.print("Packet Length: ");
    Serial.println(Wireless_Receive.Data.length());  
     
      if(received == true){       
        Serial.print("received: ");
        Serial.println(Wireless_Receive.Data);
        received = false;
       }
  }else{
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
  if(Serial.available()){
    Serialdata = toupper(Serial.read());
    if(Serialdata == 'T' && Role == RX){
      TXF();
    }else if(Serialdata == 'R' && Role == TX ){
            RXF();
           }
   }
}
