
// Luces coche Fantástico: 4 LEDS como salidas en los pines 4,5,6, y 7. Puerto D
// Interrupción en el pin2: Colocar un pulsador en este pin

unsigned tiempo = 0;
volatile int frecuencia = 100;

void setup() { 
  DDRD=B11111111; // puerto 4,5,6,7 como salida.
  /// Las demás también salidas pero no las voy a usar
  attachInterrupt(digitalPinToInterrupt(2), cambioFrecuencia, FALLING);
  Serial.begin(9600);
}

void loop() {
  tiempo = millis()/frecuencia; // A menos valor, más rápido
  PORTD = (1<< ((3*((tiempo/4)%2==1)-(tiempo%4)*((tiempo/4)%2==1)+(tiempo%4)*((tiempo/4)%2==0))+4) );
  Serial.println(frecuencia);
}

void cambioFrecuencia() {
  frecuencia+=10;
}
