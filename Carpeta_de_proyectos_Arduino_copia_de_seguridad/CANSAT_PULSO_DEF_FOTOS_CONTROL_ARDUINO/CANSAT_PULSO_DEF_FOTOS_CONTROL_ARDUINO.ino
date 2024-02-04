#include <SoftwareSerial.h>

SoftwareSerial miPuertoSerie(2, 3); // RX, TX --> crea el objeto de la clase Serial

long tiempo = 0; // variable tiempo
long cada = 5000; // cada 3 segundos
long ancho = 800; // ancho del pulso 50 milisegundos
int pulso = 0;

void setup() {
  miPuertoSerie.begin(9600); // inicializa el objeto de la clase serial
  // Serial.begin(9600); // en caso que use los pins 0 y 1  RX->0  TX->1
  pinMode(13,OUTPUT); // pin 13 output
}

void loop() {

  tiempo = millis();
  pulso = int(tiempo/cada);

  if (tiempo>2000) { // en marcha 2 segundos después de encender
    miPuertoSerie.write( ((tiempo-pulso*cada)<ancho) ); // escribe el dato 0 o 1 en puerto en serie
                                                         // estado alto cada 800 ms (ancho)
                                                         // envía un 1 cada 5 segundos (cada)
    // Serial.write(estado); // en caso que use los pins 0 y 1  RX->0  TX->1
    delay(100);
    miPuertoSerie.flush(); // limpia el buffer 
    // Serial.flush(); // en caso que use los pins 0 y 1  RX->0  TX->1
  }

  // digitalWrite(ledPin,!((tiempo-pulso*cada)<ancho)); // negado, pulso con lógica negativa
  digitalWrite(13,((tiempo-pulso*cada)<ancho)); // lógica positiva
}
