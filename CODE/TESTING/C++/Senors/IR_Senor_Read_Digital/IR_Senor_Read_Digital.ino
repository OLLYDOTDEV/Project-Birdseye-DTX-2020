int IRSensor = 3; // connect ir sensor to arduino pin 3

void setup() 
{


  Serial.begin(2000000);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }

  pinMode (IRSensor, INPUT); // sensor pin INPUT
}

void loop()
{


   Serial.println(digitalRead(IRSensor));
delay(100);
}

  
