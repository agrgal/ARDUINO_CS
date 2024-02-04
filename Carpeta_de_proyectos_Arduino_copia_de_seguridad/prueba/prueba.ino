// A) definición de variables
unsigned long tiempo = 0; // tiempo como entero largo sin signo, 4 bytes o 32 bits.
boolean dato = 0;  // Defino variable dato como binaria a 0

void setup() {
  DDRC = B00000100; // defino como salida el C2 con el registro.
  PORTC = B00000000; // Inicializo el puerto 2 a 0
}

void loop() {
  tiempo = millis(); // Obtiene la cuenta del tiempo en ms y se la asigna a la variable tiempo
  dato = 1 & (tiempo >> 9); 
     // Desplaza los bits de la variable tiempo 9 posiciones a la derecha
     // El bit 9º pasa a ocupar la posición 0.
     // Ese bit cambia de 0 a 1 cada 2^9 milisegundos, o cada 0.512 segundos. 
     // Al hacer la operación AND con ese desplazamiento, obtengo como valor binario puro dicho bit
     // dato es pues 0 si el bit 9º es cero y 1 si es 1.
  PORTC = 0 | (dato << 2); // Desplazamiento de un BIT dos posiciones 
     // El puerto C será igual a desplazar el dato dos posiciones a la izquierda. Por lo tanto
     // valdrá o B0000000 o B00000100
}
