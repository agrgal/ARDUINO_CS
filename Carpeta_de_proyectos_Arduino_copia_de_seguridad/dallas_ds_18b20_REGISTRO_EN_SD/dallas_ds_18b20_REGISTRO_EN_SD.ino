#include <OneWire.h> //Se importan las librerías
#include <DallasTemperature.h>

// bibliotecas necesarias con la tarjeta
#include <SPI.h>
#include <SD.h>

#define Pin 2 //Se declara el pin donde se conectará la DATA

// La placa ethernet NECESITA comunicarse con este PIN. No usuarlo en el experimento. Tb creo que no usar el 11. LEER
const int chipset = 4;

// VAriables temporales
long tiempo = 0; // cuenta de tiempo...
int cada = 2000; // cada 1000ms o un segundo. Mayor de 500ms

OneWire ourWire(Pin); //Se establece el pin declarado como bus para la comunicación OneWire

DallasTemperature sensors(&ourWire); //Se instancia la librería DallasTemperature

File mifichero; // variable declarada como de tipo fichero.

String dato =""; // cadena vacía ocn los datos
String nomFichero = "temp.csv"; // tiene que ser del tipo 8.3

void setup() {

pinMode(8,OUTPUT);

delay(1000); // Espero un segundo
Serial.begin(9600);
sensors.begin(); //Se inician los sensores

// Inicializo tarjeta
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipset)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  // Si el fichero existe, lo borra
  // Así lo puedo usar indefinidamente con el mismo nombre
  if (SD.exists(nomFichero)) {
    Serial.println("El fichero "+nomFichero+" ya existe. Lo borro.");
    SD.remove(nomFichero);
  }

}


void loop() {

  // cuenta del tiempo
  tiempo = millis();

  // lectura del puerto analógico A0
 sensors.requestTemperatures(); //Prepara el sensor para la lectura
 float sensorValue = sensors.getTempCByIndex(0);
  
   // Cadena con los datos...
 dato = String(tiempo)+"; "+String(sensorValue);
 dato.replace('.', ','); // por si necesito la coma decimal y no el punto...
  
  // print out the value you read:
  Serial.println(dato);
  
  // Escribir valor en la tarjeta
  escribir(mifichero,dato);

  delay(cada); // retraso hasta la próxima lectura
  // digitalWrite(8,LOW); // aseguro que el LED esté apagado
  
} // fin del loop



// ***************************
// Funciones
// ***************************

// F1) Función que escribe un dato en la tarjeta
int escribir(File fic, String valor) {
  
  // Abre el fichero en modo de escritura
  fic = SD.open(nomFichero, FILE_WRITE);

  // if the file is available, write to it:
  if (fic) {
    fic.println(valor);
    fic.close();
    // Encendido del LED
    digitalWrite(8,HIGH);
    delay(200);
    digitalWrite(8,LOW);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening temp.csv");
  }

}
