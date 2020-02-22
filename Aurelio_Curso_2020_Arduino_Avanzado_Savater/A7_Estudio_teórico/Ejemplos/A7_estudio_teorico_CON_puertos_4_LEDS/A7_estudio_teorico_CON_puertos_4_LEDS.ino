// Luces coche Fantástico
// 4 LEDS como salidas en los pines
// 4,5,6, y 7. Puerto D

unsigned tiempo = 0;

void setup() { 
  DDRD=B11111111; // puerto 4,5,6,7 como salida.
  /// Las demás también salidas pero no las voy a usar
}

void loop() {
  tiempo = millis()/100; // A menos valor, más rápido
  // Control de puertos y tiempo avanzado.
  // tiempo%4 --> señal periódica 0 1 2 3 -->
  // Fórmula ciclo positivo --> x
  // Fórmula ciclo negativo --> 3-x
  // Ciclo positivo en tiempo/4 resto entre 2 igual a cero
  // Ciclo negativo en tiempo/4 resto entre 2 igual a uno
  // El último suma 4, porque empieza en el pin 4...
  PORTD = (1<< ((3*((tiempo/4)%2==1)-(tiempo%4)*((tiempo/4)%2==1)+(tiempo%4)*((tiempo/4)%2==0))+4) );
}
