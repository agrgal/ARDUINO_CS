/*
  Blink
*/

// ledPin refers to ESP32 GPIO 1
const int ledPin = 2; // No tiene todos los pines. pruebo en puerto IO2
const int Button = 12; // No tiene todos los pines. pruebo en puerto IO12

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin ledPin as an output.
  pinMode(ledPin, OUTPUT);
  pinMode(Button, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(ledPin, digitalRead(Button) );   // turn the LED on (HIGH is the voltage level)
  // wait for a second
}
