// https://programarfacil.com/blog/arduino-blog/ds18b20-sensor-temperatura-arduino/
// https://www.instructables.com/id/SD-Card-Module-With-ESP8266/

// Importante: con NodeMCU , la tarjeta funciona a 5V. Conectarla a Vin siestá conectada a un ordenador.

#include <SD.h> // tarjetas SD
// #include <SPI.h>
#include <OneWire.h> //Se importan las librerías
#include <DallasTemperature.h>

#define Pin D2
//Se declara el pin donde se conectará la DATA

OneWire ourWire(Pin); //Se establece el pin declarado como bus para la comunicación OneWire
DallasTemperature sensors(&ourWire); //Se instancia la librería DallasTemperature

#define miPIN  D8
// Por el que se comunica con la tarjeta

File miFichero; 

String dato =""; // cadena vacía ocn los datos
String nomFichero = "temperaturas.csv"; // tiene que ser del tipo 8.3

long tiempo=0;
int cada = 5000 ;// tiempo en milisegundos
float sensorValue =0.0; // valor del sensor 

int contar=0;

void setup() {  
  pinMode(2, OUTPUT);     // Initialize the LED_BUILTIN pin 
  delay(1000);
  // Serial.begin(9600);
  sensors.begin(); //Se inician los sensores

}



void loop() {

  if (contar==0) {
    if (!SD.begin(miPIN)) { // Pin por el que se comunica con la tarjeta
        // Serial.println("La tarjeta está fallando");
        // don't do anything more:
        return;
      }
    
    // Serial.println("tarjeta inicializada.");
    contar =1;
  
    // Si el fichero existe, lo borra
    // Así lo puedo usar indefinidamente con el mismo nombre
    if (SD.exists(nomFichero)) {
      // Serial.println("El fichero "+nomFichero+" ya existe. Lo borro.");
      SD.remove(nomFichero);
    }
    
  }
  
  tiempo = millis();
  
  sensors.requestTemperatures(); //Prepara el sensor para la lectura
  
  // Del primer sensor, índice cero. Se pueden conectar más. 
  sensorValue = sensors.getTempCByIndex(0);
  // Serial.print(sensorValue); //Se lee e imprime la temperatura en grados Celsius
  // Serial.println(" grados Centigrados");
  // Serial.print(sensors.getTempFByIndex(0)); //Se lee e imprime la temperatura en grados Fahrenheit
  // Serial.println(" grados Fahrenheit"); 

  dato = String(tiempo)+"; "+String(sensorValue);
  dato.replace('.', ','); // por si necesito la coma decimal y no el punto...
  
  // print out the value you read:
  // Serial.println(dato+ " ºC");
  
  // Escribir valor en la tarjeta
  escribir(miFichero,dato);
  digitalWrite(2, HIGH); 
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
    digitalWrite(2, LOW); // Se activa por lógica negativa
    delay(200);
  }
  // if the file isn't open, pop up an error:
  else {
    // Serial.println("error opening temp.csv");
  }

}
