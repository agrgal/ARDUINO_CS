// Control de un motor PASO A PASO.
// Por Aurelio Gallardo Rodríguez.

// Establece las constantes a utilizar 
 int A=9,C=8; //Pines del motor paso a paso: primer bobinado 
 int B=7,D=6; //Pines del motor paso a paso: segundo bobinado
 
 int microSegundosPaso=1500000; //Tiempo en microsegundos entre paso y paso

// *********
// Funciones
// *********
void secuencia(int cual) {
      digitalWrite(A,((cual==1) or (cual==4))); // El A se activa en la secuencia 1 y 4
      digitalWrite(B,((cual==1) or (cual==2))); // El B se activa en la secuencia 1 y 2
      digitalWrite(C,((cual==2) or (cual==3))); // El C se activa en la secuencia 1 y 3
      digitalWrite(D,((cual==3) or (cual==4))); // El D se activa en la secuencia 1 y 4 
      delayMicroseconds(microSegundosPaso); 
}

// Función de parada motor 
void paro(){
    secuencia(0); // Sería LOW en todos
}

// Función izquierdas
void izquierda(){
  secuencia(1); // ver esquema NORMAL
  secuencia(2); 
  secuencia(3);  
  secuencia(4); 
}

// Función derechas
void derecha(){
  secuencia(4); // ver esquema NORMAL
  secuencia(3); 
  secuencia(2);  
  secuencia(1); 
}

// Función que establece el sentido y los pasos del motor 
void pasos(int pasos, char sentido){
      for (int i = 0; i < pasos; i++) {                             // n pasos a la izquierda    
        if (sentido=='d') {derecha();}
        if (sentido=='i') {izquierda();}
      }
 }

// ***********
// SETUP
// ***********
void setup() {
 pinMode(A, OUTPUT); //Configura A como salida 
 pinMode(B, OUTPUT); //Configura B como salida
 pinMode(C, OUTPUT); //Configura C como salida
 pinMode(D, OUTPUT); //Configura D como salida
 paro();
 }
 

// ***********
// LOOP
// ***********
void loop() {
 pasos(10,'d'); // 10 pasos a la derecha
 delay (500);   // Una paradita
 pasos(10,'i'); // 15 pasos a la izquierda
 delay (500);   // Una paradita
 } 
