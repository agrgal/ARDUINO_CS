/*
  Blink
*/

// ledPin refers to ESP32 GPIO 1
const int ledPin = 12; // No tiene todos los pines. pruebo en puerto IO2
const int Button = 2; // No tiene todos los pines. pruebo en puerto IO12
bool estado = 0; // estado del sistema
bool estadoAnt = 0; // estado anterior del sistema
bool pulsado = 0; // botón pulsado

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin ledPin as an output.
  pinMode(ledPin, OUTPUT);
  pinMode(Button, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  pulsado = digitalRead(Button);
  estado = (!estadoAnt)*pulsado + estado*(!pulsado);
  if (estado!=estadoAnt) {
    delay(200);
  }
  digitalWrite(ledPin,estado);
  estadoAnt=estado; 
}
