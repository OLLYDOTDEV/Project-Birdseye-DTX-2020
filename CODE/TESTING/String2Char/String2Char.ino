

typedef struct
{
   String Data = "Char test ";

}
DataDef;
DataDef DataPak;




typedef struct
{
   
char Data[32];

}
BuffDef;
BuffDef BuffPak;





void setup() {
   Serial.begin(115200);
}
void loop() {
  for (byte i = 0; i <= 9; i++) {
    BuffPak.Data[i] = DataPak.Data[i]; // String to char Array
    Serial.println(BuffPak.Data[0]); // Print n'th key of array (Arrays start at 0)
  
  }
 delay(200); 
}
