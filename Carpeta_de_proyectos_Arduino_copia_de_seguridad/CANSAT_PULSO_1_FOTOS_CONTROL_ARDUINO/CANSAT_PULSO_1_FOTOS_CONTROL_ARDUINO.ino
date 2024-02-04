long tiempo = 0; // variable tiempo
long cada = 4000; // cada 3 segundos
long ancho = 800; // ancho del pulso 50 milisegundos
int pulso = 0;

int ledPin = 8;

void setup() {
  pinMode(ledPin,OUTPUT); // pìn 8
  pinMode(13,OUTPUT); // pin 13 output
}

void loop() {

  tiempo = millis();
  pulso = int(tiempo/cada);

  digitalWrite(ledPin,!((tiempo-pulso*cada)<ancho)); // negado, pulso con lógica negativa
  digitalWrite(13,!((tiempo-pulso*cada)<ancho)); // negado, pulso con lógica negativa  

}
