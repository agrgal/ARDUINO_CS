/*
  restart by button
*/

// ledPin refers to ESP32 GPIO 1
const int ledPin = 12; // No tiene todos los pines. pruebo en puerto IO12
const int Button = 2; // No tiene todos los pines. pruebo en puerto IO2
long tiempo = 0; // tiempo en milisegundos
long rapidez= 0; // rapidez del parpadeo
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
  tiempo = millis();
  rapidez= (100*(1+int(tiempo/4000)))*(rapidez<40000)+rapidez*(rapidez>=40000);
  
  pulsado = digitalRead(Button);
  estado = (!estadoAnt)*pulsado + estado*(!pulsado);
  if (estado!=estadoAnt) {
    ESP.restart(); // reinicia el dispositivo. 
  }
  
  digitalWrite(ledPin,(tiempo/rapidez)%2);
  estadoAnt=estado; 
}
