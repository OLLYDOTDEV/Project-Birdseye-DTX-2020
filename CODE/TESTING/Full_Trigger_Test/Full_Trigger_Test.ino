/*
V1
DEV:OLIVER
REPO:https://github.com/OLLYDOTDEV/Project-Birdseye-DTX-2020
INFO: 
> test code for PIR and ir sensor
> test to settting a triggered state alert.

*/

int IR_OUT_Sensor = A3; // connect ir sensor to arduino pin 2
// PIR variables
bool pir_status = 0;
int pir_out_pin = 5;
// general variables 
bool Status = 0;
void setup() 
{


Serial.begin(9600);

// sensor pins to INPUT
  pinMode (pir_out_pin, INPUT); 
  pinMode (pir_out_pin, INPUT); 
}

void loop()
{
  // pir stuff

pir_status = digitalRead(pir_out_pin);
if(pir_status == 1){ // if PIR sensor actavated then set the alert status to active (true)
Status = 1;
pir_status=0
} else{
Status = 0;
}



Serial.println(Status);
}
