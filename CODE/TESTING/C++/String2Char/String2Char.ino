// based off https://www.geeksforgeeks.org/convert-string-char-array-cpp/ && https://forum.arduino.cc/index.php?topic=550642.0
// 
// Make By OLLYDOTDEV

typedef struct
{
   String Data = "Char test";

}
DataDef;
DataDef DataPak;




typedef struct
{
   
char Data[32];

}
BuffDef;

BuffDef BuffPak;


#define key 5


void setup() {
   Serial.begin(115200);
}
void loop() {



  
  for (byte i = 0;i <= DataPak.Data.length(); i++) {
    BuffPak.Data[i] = DataPak.Data[i]; // String to char Array

 
  }

  
 delay(200); 
 Serial.print("String Length: ");
 Serial.println(DataPak.Data.length());
 
     Serial.print("printing ");
     Serial.print(key);
    Serial.print("'th key of array: ");
    Serial.println(BuffPak.Data[key]); // Print n'th key of array (Arrays start at 0)

}
