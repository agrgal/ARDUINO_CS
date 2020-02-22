byte antes = 0;

void setup() { 
  DDRD=B01111111; // puerto 7 como entrada. 
  DDRC=B00100000; // puerto 5 (A5) como salida.
}

void loop() {
  PORTC=( ( (PINC>>5) xor ((PIND>>7)>(antes>>7)) ) <<5);
  antes = PIND & B10000000;
  delay(50);
}
