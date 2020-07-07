/**
Test counting up a array and printing it


**/
String test4= "test343";
byte count = 0;
String test[] = {"hi","I","am","testing","\n\r\n\r\n\r"}; 
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
Serial.print(test[count]+"\n");

count++;

if(count >= 5){ // reset count key

 count = 0;
 }
delay(200) ;
}
