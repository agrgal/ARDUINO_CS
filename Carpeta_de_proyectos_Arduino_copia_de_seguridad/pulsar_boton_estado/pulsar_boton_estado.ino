/*
  State change detection (edge detection)

  Often, you don't need to know the state of a digital input all the time, but
  you just need to know when the input changes from one state to another.
  For example, you want to know when a button goes from OFF to ON. This is called
  state change detection, or edge detection.

  This example shows how to detect when a button or button changes from off to on
  and on to off.

  The circuit:
  - pushbutton attached to pin 2 from +5V
  - 10 kilohm resistor attached to pin 2 from ground
  - LED attached from pin 13 to ground (or use the built-in LED on most
    Arduino boards)

  created  27 Sep 2005
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/ButtonStateChange
*/

// this constant won't change:
#define led D2       // the pin that the LED is attached to
uint8_t misBotones[]={D3,D1};

long tiempo;

// Variables will change:
int estadoBoton[] = {0,0};         // current state of the button
int estadoBotonAnt[] = {0,0};     // previous state of the button

void setup() {
  // initialize the button pin as a input:
  pinMode(misBotones[0], INPUT_PULLUP);
  pinMode(misBotones[1], INPUT_PULLUP);
  // initialize the LED as an output:
  pinMode(led, OUTPUT);
  // Serial
  Serial.begin(9600);
}


void loop() {
  tiempo = millis();
  Serial.println( "Botón Uno: " + (String) pulsarBoton(0) + " -- Botón Dos: " + (String) pulsarBoton(1));
} // Fin del loop

// ****************************************
// Función que detecta si se pulsa un botón
// ****************************************
boolean pulsarBoton(int cual) {

  boolean devuelve = false; // valor por defecto
  
  // leer la entrada del botón
  estadoBoton[cual] = digitalRead(misBotones[cual]);

  // compare the buttonState to its previous state
  if (estadoBoton[cual] != estadoBotonAnt[cual]) {
    // if the state has changed, increment the counter
    if (estadoBoton[cual]  == HIGH) {
      devuelve = true;
      // Hacer algo 
      digitalWrite(led, LOW);
      delay(100);
      digitalWrite(led, HIGH);
      delay(1000);
      digitalWrite(led, LOW);
      delay(100);
      // Pulsado
    }
    // Delay a little bit to avoid bouncing
    while(millis() < tiempo+50){
        // espere [periodo] milisegundos
    }
  }
  // save the current state as the last state, for next time through the loop
  estadoBotonAnt[cual]  = estadoBoton[cual] ;
  return devuelve;  
}
