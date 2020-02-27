
#include <IRremote.h> //Biblioteca control infrarrojo
// IRremote by shirriff

int RECV_PIN = 11; // Pin de recepción de 

IRrecv receptor(RECV_PIN); // inicializo objeto de recepción

decode_results resultados; //objeto resultados 

String dato = "";
int red = 0;
int blue = 0;
int green = 0;
int pinR = 6;
int pinB = 5;
int pinG = 9;

// ************
// SETUP
// ************
void setup()
{
  Serial.begin(9600); // inicializa monitor serie
  Serial.println("[I N I C I O]");
  receptor.enableIRIn(); 
  // Inicializa el receptor para escuchar datos
  receptor.blink13(true); //permite que parpadee 
  // pin 13 al recibir datos.
  pinMode(pinR,OUTPUT);
  pinMode(pinB,OUTPUT);
  pinMode(pinG,OUTPUT);
}

// *********
// LOOP
// *********
void loop() {
  if (receptor.decode(&resultados)) {
    
    if (resultados.decode_type == NEC && resultados.bits==32) {
      // reconoce un dato
      dato = String(resultados.value, HEX); // convierte a hex
      Serial.println(dato);
      if (dato=="f720df") {red=255; blue=0; green=0;}
      if (dato=="f7a05f") {red=0; blue=0; green=255;}
      if (dato=="f7609f") {red=0; blue=255; green=0;}   
      if (dato=="f710ef") {red=(red+10)*(red<255); } 
      if (dato=="f750af") {blue=(blue+10)*(blue<255);} 
      if (dato=="f7906f") {green=(green+10)*(green<255);} 
      encender(red,green,blue);
    }
     
  
    receptor.resume(); // Preparado para recibir el siguiente valor
  } // findel if de recepción
}

// Funcion 

void encender(int cRojo,int cVerde,int cAzul) {
  Serial.println("R: "+(String) cRojo+ " / G: "+ (String) cVerde + " / B: "+ (String) cAzul);
  analogWrite(pinR,cRojo);
  analogWrite(pinB,cAzul);
  analogWrite(pinG,cVerde);
}
