int IRSensor = A3; // connect ir sensor to arduino pin 3
int LED = 13;
int IRDATA = 0 ;
void setup() 
{


  Serial.begin(2000000);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }

  pinMode (IRSensor, INPUT); // sensor pin INPUT
  pinMode (LED, OUTPUT);
  digitalWrite(LED,LOW);
}

void loop()
{
IRDATA = analogRead (IRSensor);

Serial.println(IRDATA);  
Serial.print(","); 
if(IRDATA > 600){
  
Serial.println(1020);
digitalWrite(LED,LOW) ;
}
else if(IRDATA < 600){
// if this if statement activates that means that the sensor has been triggered  
Serial.println(0);
digitalWrite(LED,HIGH);
}

  
delay(3);
}

  
