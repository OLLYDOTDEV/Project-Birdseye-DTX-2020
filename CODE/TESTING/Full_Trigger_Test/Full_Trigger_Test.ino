/*
V1
DEV:OLIVER
REPO:https://github.com/OLLYDOTDEV/Project-Birdseye-DTX-2020
INFO: 
> test code for PIR and ir sensor
> test to settting a triggered state alert.

*/


// IR variables
bool IR_status = 0;
int IRDATA = 1000 ;
int IR_threshold = 600 ;
int IR_OUT = A3; // connect ir sensor to arduino pin 2
// PIR variables
bool pir_status = 0;
int pir_out = 5;
// general variables 
bool Status = 0;
int calibration_trigger = 1; // 1 = calibration mode on | 0 = calibration mode off
void setup() 
{


Serial.begin(9600);

// sensor pins to INPUT
  pinMode (pir_out, INPUT); 
  pinMode (IR_OUT, INPUT); 
}

void loop()
{


// IR code 

IRDATA = analogRead (IR_OUT);
if(IRDATA > IR_threshold){
// IR sensor not triggered no nothing
IR_status = 0;
Status = 0;
}else if(IRDATA < IR_threshold){
// if this if statement activates that means that the sensor has been triggered  
IR_status = 1;
Status = 1;
// then send out alert to relay
}

// pir code
pir_status = digitalRead(pir_out);
if(pir_status == 1){ // if PIR sensor actavated then set the alert status to active (true)
Status = 1;
pir_status=0;
} else{
Status = 0;
}
// alert system
if(Status == 1 ){
// Send alert
Serial.println("Alarm status: " + Status);
}else{
Serial.println("Alarm status: " + Status);  
}









}
