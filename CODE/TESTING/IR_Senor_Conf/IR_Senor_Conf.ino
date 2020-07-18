/*
 *  CODE FOR PROJECT Birdseye
 *  made by: OLLYDOTDEV
 *  https://github.com/OLLYDOTDEV/Project-Birdseye-DTX-2020
 * 
 * 
 * 
 * 
 * 
 * 
 
 *  
 */




int IRSensor = A3; // connect ir sensor to arduino pin 3
int LED = 13;
int IRDATA = 0 ;

int calibration_trigger = 1; // 1 = calibration mode on | 0 = calibration mode off
int IR_threshold = 600 ;
void setup() 
{


  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }

  pinMode (IRSensor, INPUT); // sensor pin INPUT
  pinMode (LED, OUTPUT);
  digitalWrite(LED,LOW);
}

void loop(){


if(calibration_trigger != 1){


  
IRDATA = analogRead (IRSensor);

Serial.println(IRDATA);  
Serial.print(","); 
if(IRDATA > IR_threshold){
  
Serial.println(1020);
digitalWrite(LED,LOW) ;
}
else if(IRDATA < IR_threshold){
// if this if statement activates that means that the sensor has been triggered  
Serial.println(0);
digitalWrite(LED,HIGH);

// then send out alert to relay
}

  
delay(3);
}else{

// THIS IS JUST FOR calibration
Serial.print("calibration value: ");
IRDATA = analogRead (IRSensor);  
if(IRDATA > IR_threshold){
  
Serial.println(1020);
digitalWrite(LED,LOW) ;
}
else if(IRDATA < IR_threshold){

Serial.println(0);
digitalWrite(LED,HIGH);
}

}
}
  
