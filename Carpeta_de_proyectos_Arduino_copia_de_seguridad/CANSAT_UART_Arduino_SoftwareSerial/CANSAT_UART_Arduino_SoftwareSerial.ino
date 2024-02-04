/* Modificación usando librería Software Serial */

#include <SoftwareSerial.h>

SoftwareSerial miPuertoSerie(2, 3); // RX, TX --> crea el objeto de la clase Serial

int ledPin=13;
bool estado=LOW;

void setup() {
  miPuertoSerie.begin(9600); // inicializa el objeto de la clase serial
  // Serial.begin(9600); // en caso que use los pins 0 y 1  RX->0  TX->1
  pinMode(ledPin,OUTPUT);
}
void loop() {

  if (millis()>2000) { // en marcha 2 segundos después de encender
    miPuertoSerie.write(estado); // escribe el dato en el puerto serie
    // Serial.write(estado); // en caso que use los pins 0 y 1  RX->0  TX->1
    delay(1500);
    estado = !estado;
    digitalWrite(ledPin,estado);
    miPuertoSerie.flush(); // limpia el buffer 
    // Serial.flush(); // en caso que use los pins 0 y 1  RX->0  TX->1
  }
}
