#include <SoftwareSerial.h>

SoftwareSerial miPuertoSerie(2, 3); // RX, TX --> crea el objeto de la clase Serial

long tiempo = 0; // variable tiempo
long cada = 5000; // cada 3 segundos
long ancho = 800; // ancho del pulso 50 milisegundos
int pulso = 0;

unsigned int altura;

void setup() {
  miPuertoSerie.begin(9600); // inicializa el objeto de la clase serial
  // Serial.begin(9600); // en caso que use los pins 0 y 1  RX->0  TX->1
  pinMode(13,OUTPUT); // pin 13 output

  randomSeed(analogRead(0));
  altura = (int) random(200,500);
}

void loop() {

  tiempo = millis();
  pulso = int(tiempo/cada);  

  if (tiempo>2000 && ((tiempo-pulso*cada)<ancho)==1 ) { // en marcha 2 segundos después de encender y si se cumple la condicion
    miPuertoSerie.print(altura); // escribe el dato de altura
    // miPuertoSerie.write(highByte(altura));
    // miPuertoSerie.write(lowByte(altura)); // dividido en dos bytes
    Serial.write(altura); // en caso que use los pins 0 y 1  RX->0  TX->1
    // Serial.print(highByte(altura));
    // Serial.print(lowByte(altura)); // dividido en dos bytes
    Serial.println();
    digitalWrite(13,HIGH);
    delay(ancho+1);
    digitalWrite(13,LOW);
    miPuertoSerie.flush(); // limpia el buffer 
    // Serial.flush(); // en caso que use los pins 0 y 1  RX->0  TX->1
    altura++;
  }

  // digitalWrite(ledPin,!((tiempo-pulso*cada)<ancho)); // negado, pulso con lógica negativa
  // digitalWrite(13,((tiempo-pulso*cada)<ancho)); // lógica positiva
}
