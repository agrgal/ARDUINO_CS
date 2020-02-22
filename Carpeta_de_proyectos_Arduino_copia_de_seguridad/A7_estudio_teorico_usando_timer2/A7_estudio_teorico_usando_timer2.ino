

// Usando timer2. El timer2 es de 8bits 
// usa por defecto divisiones del reloj 16MHz.
// La primera que usa es la división 1/2 --> 8MHz.
// A partir de ahí, 1MHz, 250Hz, etc. verlo en la web
// https://hetpro-store.com/TUTORIALES/arduino-timer/

volatile int cuenta=0;
volatile boolean ESTADO = 0;

void setup() {
  pinMode(6,OUTPUT); // poner un led en el pin 6
  // A) Desactivo el uso de interrupciones. Registro SREG
  // 0 en el bit 7.
  SREG = (SREG & 0b01111111);  
  // B) Limpio el registro contador del timer2 
  TCNT2 = 0; 
  // C) Habilitar la generación de una señal por desbordamiento
  // en el paso de 255 a 0. Bandera 0 del registro TIMSK2
  TIMSK2 =TIMSK2|0b00000001; 
  // D) Registro TCCR2B. Sus tres bits menos significativos
  // CS20, CS21 y CS22 me dan la frecuencia del timer2. Por ejemplo
  // si tengo 111 --> 7812.5Hz o un período de 1.28e-4 seg = 0.128ms
  // cuando acabe la cuenta de 255 han pasado 32.6ms 
  TCCR2B = 0b00000111; // o sea, funciona a 7812.5Hz
  // E) Habilito las interrupciones globales
   SREG = (SREG & 0b01111111) | 0b10000000; //Habilitar interrupciones 
}


void loop() {
  // nada  
}

// Después del Loop
ISR(TIMER2_OVF_vect){
    cuenta++; // contando a 30 permite
    // que el período sea ~ un segundo
    // 30 * 32.6ms = 978ms
    if(cuenta > 29) {
      digitalWrite(6,ESTADO);
      ESTADO = !ESTADO;
      cuenta=0;
    }
}
