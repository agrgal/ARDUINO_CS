// https://programarfacil.com/blog/arduino-blog/ds18b20-sensor-temperatura-arduino/
// https://www.instructables.com/id/SD-Card-Module-With-ESP8266/

#include <SD.h> // tarjetas SD
// #include <SPI.h> 
#include <OneWire.h> //Se importan las librerías
#include <DallasTemperature.h>

int Pin =  2;
//Se declara el pin donde se conectará la lectura de datos del sensor
int cs_PIN = 9;
// Por el que se comunica con la tarjeta grabadora microSD

OneWire ourWire(Pin); //Se establece el pin declarado como bus para la comunicación OneWire
DallasTemperature sensors(&ourWire); //Se instancia la librería DallasTemperature

File miFichero; 

String dato =""; // cadena vacía con los datos
String nomFichero = "tempAUNO.csv"; // tiene que ser del tipo 8.3

long tiempo=0;
int cada = 500 ;// tiempo en milisegundos
float sensorValue =0.0; // valor del sensor 

void setup() {

   Serial.begin(9600);
  
   if (!SD.begin(cs_PIN)) { // Pin por el que se comunica con la tarjeta
      Serial.println("La tarjeta está fallando");
      // don't do anything more:
      return;
    }
  
  Serial.println("Tarjeta inicializada.");

  // Si el fichero existe, lo borra
  // Así lo puedo usar indefinidamente con el mismo nombre
  if (SD.exists(nomFichero)) {
    Serial.println("El fichero "+nomFichero+" ya existe. Lo borro.");
    SD.remove(nomFichero);
  }

  pinMode(6,OUTPUT);
  
  delay(1000);
  sensors.begin(); //Se inician los sensores

}



void loop() {
  
  tiempo = millis();
  
  sensors.requestTemperatures(); //Prepara el sensor para la lectura
  
  // Del primer sensor, índice cero. Se pueden conectar más. 
  sensorValue = sensors.getTempCByIndex(0);
  // Serial.print(sensorValue); //Se lee e imprime la temperatura en grados Celsius
  // Serial.println(" grados Centigrados");
  // Serial.print(sensors.getTempFByIndex(0)); //Se lee e imprime la temperatura en grados Fahrenheit
  // Serial.println(" grados Fahrenheit"); 

  dato = String(tiempo)+"; "+String(sensorValue)+" º C";
  dato.replace('.', ','); // por si necesito la coma decimal y no el punto...
  
  // print out the value you read:
  Serial.println(String(sensorValue));
  
  // Escribir valor en la tarjeta
  escribir(miFichero,dato);

  delay(cada); //Se provoca un lapso de 1 segundo antes de la próxima lectura

}


// ********************************************
// funciones
// ********************************************

// F1) Función que escribe un dato en la tarjeta
int escribir(File fic, String valor) {
  
  // Abre el fichero en modo de escritura
  fic = SD.open(nomFichero, FILE_WRITE);

  // if the file is available, write to it:
  if (fic) {
    fic.println(valor);
    fic.close();
    // Encendido del LED
    digitalWrite(6,HIGH);
    delay(200);
    digitalWrite(6,LOW);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("Error abriendo fichero "+nomFichero);
  }

}
