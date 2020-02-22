#include <SD.h>
#include <SPI.h> 

#define css D4
 
File logFile;
int contar = 0;

void setup()
{
  Serial.begin(115200);

}
 

void loop()
{

  if (contar==0) {
    Serial.print(F("Iniciando SD ..."));
    if (!SD.begin(css))
    {
      Serial.println(F("Error al iniciar"));
      return;
    }
    Serial.println(F("Iniciado correctamente"));
    contar=1;
  }
  
  // Abrir archivo y escribir valor
  logFile = SD.open("datalog.txt", FILE_WRITE);
  
  if (logFile) { 
        int value = 111;
        logFile.print("Time(ms)=");
        logFile.print(millis());
        logFile.print(", value=");
        logFile.println(value);
        Serial.println("ESTÁ FUNCIONADO");
        logFile.close();  
  } 
  else {
    Serial.println("Error al abrir el archivo");
  }
  delay(500);
  
}
