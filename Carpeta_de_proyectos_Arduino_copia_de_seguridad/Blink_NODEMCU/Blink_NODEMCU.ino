
#define LED D0  // Lo conectamos a D0 o al GPIO16
// Mapeo de pines --> https://programarfacil.com/wp-content/uploads/2017/06/pines-nodemcu.png

void setup() {
  // El LED integrado está conectado al pin 2.
  pinMode(LED, OUTPUT);
}

void loop() {
  // Encender el LED (Está conectado con una resistencia pull-up,
  // por eso se enciende si el pin está a nivel bajo)
  digitalWrite(LED, LOW);
  delay(500); // Esperar un segundo
  // Apagar el LED
  digitalWrite(LED, HIGH);
  delay(500); // Esperar un segundo
}
