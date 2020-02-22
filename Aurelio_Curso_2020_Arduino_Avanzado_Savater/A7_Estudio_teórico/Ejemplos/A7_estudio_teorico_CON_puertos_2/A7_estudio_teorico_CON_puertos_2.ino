void setup() { 
  DDRD=B01111111; // puerto 7 como entrada. 
  DDRC=B00100000; // puerto 5 (A5) como salida.
}

void loop() {
  // La lectura del puerto D en el valor 7
  // por si acaso con AND 1
  // La traslado al quinto lugar del PORTC
  PORTC=((PIND>>7 & 1)<<5);
}
