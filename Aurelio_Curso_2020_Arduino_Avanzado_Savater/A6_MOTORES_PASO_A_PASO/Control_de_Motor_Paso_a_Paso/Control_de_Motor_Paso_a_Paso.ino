/*
  Control de un Motor Paso a Paso
  Este codigo controla un motor paso a paso de un reproductor de CD. 
  
  Emplea las salidas digitales 6, 7, 8 y 9 en formato Digital
 
  Este ejemplo es de dominio publico.
 */
 
// Establece las constantes a utilizar 
 int devanado1_A=6,devanado1_B=7,devanado2_C=8,devanado2_D=9;       //Pines del motor paso a paso
 int microSegundosPaso=1500000;                                        //Tiempo en microsegundos entre paso y paso

void setup() {
 pinMode(devanado1_A, OUTPUT);                                      //Configura devanado1_A como salida 
 pinMode(devanado1_B, OUTPUT);                                      //Configura devanado1_B como salida
 pinMode(devanado2_C, OUTPUT);                                      //Configura devanado2_C como salida
 pinMode(devanado2_D, OUTPUT);                                      //Configura devanado2_D como salida
 paro();
 }
 
// Función de parada motor 
void paro(){
 digitalWrite(devanado1_A, LOW);                                    // Borna A del devanado 1 a 0 voltios
 digitalWrite(devanado1_B, LOW);                                    // Borna B del devanado 1 a 0 voltios
 digitalWrite(devanado2_C, LOW);                                    // Borna C del devanado 2 a 0 voltios
 digitalWrite(devanado2_D, LOW);                                    // Borna D del devanado 2 a 0 voltios
 }

// Función giro a la izquierda del motor 
void izquierda(){
 //paso 1
 digitalWrite(devanado1_A, LOW);                                    // Borna A del devanado 1 a 0 voltios
 digitalWrite(devanado1_B, HIGH);                                   // Borna B del devanado 1 a 5 voltios
 digitalWrite(devanado2_C, HIGH);                                   // Borna C del devanado 2 a 5 voltios
 digitalWrite(devanado2_D, LOW);                                    // Borna D del devanado 2 a 0 voltios
 delayMicroseconds(microSegundosPaso);                              // Espera un tiempo en microsegundos 
 //paso 2
 digitalWrite(devanado2_C, LOW);                                    // Borna C del devanado 2 a 0 voltios
 digitalWrite(devanado2_D, HIGH);                                   // Borna D del devanado 2 a 5 voltios
 delayMicroseconds(microSegundosPaso);                              // Espera un tiempo en microsegundos 
 //paso 3
 digitalWrite(devanado1_A, HIGH);                                   // Borna A del devanado 1 a 5 voltios
 digitalWrite(devanado1_B, LOW);                                    // Borna B del devanado 1 a 0 voltios
 delayMicroseconds(microSegundosPaso);                              // Espera un tiempo en microsegundos 
 //paso 4
 digitalWrite(devanado2_C, HIGH);                                   // Borna C del devanado 2 a 5 voltios
 digitalWrite(devanado2_D, LOW);                                    // Borna D del devanado 2 a 0 voltios
 delayMicroseconds(microSegundosPaso);                              // Espera un tiempo en microsegundos 
 }

// Función giro a la derecha del motor 
void derecha(){
 //paso 1
 digitalWrite(devanado1_A, HIGH);                                   // Borna A del devanado 1 a 5 voltios
 digitalWrite(devanado1_B, LOW);                                    // Borna B del devanado 1 a 0 voltios
 digitalWrite(devanado2_C, HIGH);                                   // Borna C del devanado 2 a 5 voltios
 digitalWrite(devanado2_D, LOW);                                    // Borna D del devanado 2 a 0 voltios
 delayMicroseconds(microSegundosPaso);                              // Espera un tiempo en microsegundos 
 //paso 2
 digitalWrite(devanado2_C, LOW);                                    // Borna C del devanado 2 a 0 voltios
 digitalWrite(devanado2_D, HIGH);                                   // Borna D del devanado 2 a 5 voltios
 delayMicroseconds(microSegundosPaso);                              // Espera un tiempo en microsegundos 
 //paso 3
 digitalWrite(devanado1_A, LOW);                                    // Borna A del devanado 1 a 0 voltios
 digitalWrite(devanado1_B, HIGH);                                   // Borna B del devanado 1 a 5 voltios
 delayMicroseconds(microSegundosPaso);                              // Espera un tiempo en microsegundos 
 //paso 4
 digitalWrite(devanado2_C, HIGH);                                   // Borna C del devanado 2 a 5 voltios
 digitalWrite(devanado2_D, LOW);                                    // Borna D del devanado 2 a 0 voltios
 delayMicroseconds(microSegundosPaso);                              // Espera un tiempo en microsegundos 
 }

// Función que establece el sentido y los pasos del motor 
void pasos(int pasos, char sentido){
    switch (sentido) {                                              // sentido de giro
    case 'd':    
      for (int i = 0; i < pasos; i++) {                             // n pasos a la derecha
        derecha();
      }
      break;
    case 'i':    
      for (int i = 0; i < pasos; i++) {                             // n pasos a la izquierda    
        izquierda();
      }
      break;
    }
 }

void loop() {
 pasos(50,'d');                                                     // 50 pasos a la derecha
 delay (1500);
 pasos(150,'i');                                                    // 150 pasos a la izquierda
 delay (1500);
 } 
