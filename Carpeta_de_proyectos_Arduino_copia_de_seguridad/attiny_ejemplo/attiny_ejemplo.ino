#include <Arduino.h>

#define PERIODO 1000
unsigned char salida = 0;
unsigned estado_salida = 0;
unsigned long tiempo_anterior = 0;

void setup() {
  // put your setup code here, to run once:
  DDRB = DDRB | (1<<PB2);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long tiempo = millis();

  if (tiempo - tiempo_anterior>PERIODO) {

      if (estado_salida == 0 ) {
        estado_salida = 1;
        salida = salida | (1<<PB2);
      } else {
        estado_salida = 0;
        salida = salida & ~(1<<PB2);
      }
        tiempo_anterior = tiempo;
  }



  PORTB = salida;
  
}
