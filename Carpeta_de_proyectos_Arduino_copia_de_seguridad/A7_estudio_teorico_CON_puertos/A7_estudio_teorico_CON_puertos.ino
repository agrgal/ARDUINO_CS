boolean pulsado = 0;

void setup() { 
  DDRD=B01111111; // puerto 7 como entrada. 
  DDRC=B00100000; // puerto 5 (A5) como salida.

}

void loop() {
  pulsado = PIND & (1<<7);
  PORTC=(pulsado<<5);
}
