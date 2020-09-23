int pir_out_pin = 4;
int pir_status = 0;
void setup() {
Serial.begin(9600);

pinMode(pir_out_pin, INPUT);

}

void loop() {

pir_status = digitalRead(pir_out_pin);

Serial.println(pir_status);
  }
