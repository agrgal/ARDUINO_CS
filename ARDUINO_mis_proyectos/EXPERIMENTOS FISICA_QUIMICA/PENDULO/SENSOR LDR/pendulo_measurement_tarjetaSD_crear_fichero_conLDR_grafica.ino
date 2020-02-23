#include <SPI.h>
#include <SD.h>

#define led 8

long tiempo;

File miFichero;

const int chipSelect = 4;

void setup() {
  // define el pin led como salida y el ir como entrada DIGITAL
  pinMode(led, OUTPUT);
  // Serial
  Serial.begin(9600); // Pone en marcha la lectura en serie.

  // ver si la tarjeta puede 
  if (!SD.begin(chipSelect)) {
    Serial.println("No tengo tarjeta!!");
    // don't do anything more:
    return;
  }
  Serial.println("Tarjeta inicializada");
  
}

void loop() {

  // cuenta del tiempo;
  tiempo = micros();

  // Inicializa la cadena
  String dataEscribir = "";
  
        
  dataEscribir+=String(tiempo);
  dataEscribir+=String(";");
  dataEscribir+=String(analogRead(A0));
  
  // abro el fichero
  miFichero = SD.open("LDRgraph.csv", FILE_WRITE); // Importante: nombre con 8 caracteres, tres de extensión.
  // escribo 
  miFichero.println(dataEscribir);
  // Cierro el fichero
  miFichero.close();

  Serial.println(dataEscribir);

  delay(100); // cada 100 milisegundos           
 
}
