#define LED_BUILTIN 2

void setup() {
  // El LED integrado está conectado al pin 2.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Encender el LED (Está conectado con una resistencia pull-up,
  // por eso se enciende si el pin está a nivel bajo)
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000); // Esperar un segundo
  // Apagar el LED
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000); // Esperar un segundo
}
