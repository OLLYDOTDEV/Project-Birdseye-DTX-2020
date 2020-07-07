int IRSensor = A3; // connect ir sensor to arduino pin 2

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


   Serial.println(analogRead (IRSensor));

}

  
