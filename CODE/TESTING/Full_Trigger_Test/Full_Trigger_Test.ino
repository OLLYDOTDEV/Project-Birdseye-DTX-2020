/*
V1
DEV:OLIVER
REPO:https://github.com/OLLYDOTDEV/Project-Birdseye-DTX-2020
INFO: 
> test code for PIR and ir sensor
> test to settting a triggered state alert.

*/
#include "Smooth.h"

// IR variables
bool IR_Status = 0;
int IR_DATA = 0 ;
int IR_Threshold = 600 ;
int IR_OUT = A3; // connect ir sensor to arduino pin 2
// PIR variables
bool Pir_Status = 0;
bool Pir_Data = 0;
int Pir_Out = 4;
// general variables 
bool Alert_Status = 0;
int calibration_trigger = 1; // 1 = calibration mode on | 0 = calibration mode off





AnalogSmooth IRsensor; // contruct an object with the smooth class
void setup() 
{
Serial.begin(9600);
 IRsensor.Smooth_setup(); // set all array values to 0 [see Smooth.h]
// sensor pins to INPUT
  pinMode (Pir_Out, INPUT); 
  pinMode (IR_OUT, INPUT); 

Serial.println("Boot sequence complete initialising main program \n\n\n\n\n\n\n\n\n\n\n\n");
delay(1000);
}

void loop()
{



Serial.println("\n\n\n ________________________");



// IR code 
IR_DATA = IRsensor.Smooth(IR_OUT);

if(IR_DATA < IR_Threshold){
// if this if statement activates that means that the sensor has been triggered  
IR_Status = 1;
}else{
IR_Status = 0;  
}

// pir code
Pir_Data = digitalRead(Pir_Out);
if(Pir_Data == 1){ // if PIR sensor actavated then set the alert status to active (true)
Pir_Status=1;
}else{
Pir_Status=0;  
}


if(Pir_Status || IR_Status == 1){
Alert_Status = 1;
// then send out alert to relay 
// sensors have been triggered
}else{
Alert_Status = 0;
// no sensor actived no nothing
}


// value debug
Serial.println("\n");
Serial.print("IR Data: ");
Serial.println(IR_DATA);
Serial.print("PIR Data: ");
Serial.println(Pir_Data);
// alert debug
Serial.println("\n");
Serial.print("IR Status: ");
Serial.println(IR_Status);
Serial.print("PIR Status: ");
Serial.println(Pir_Status);
Serial.print("\nAlert_Status: ");
Serial.println(Alert_Status);







delay(600);


}
