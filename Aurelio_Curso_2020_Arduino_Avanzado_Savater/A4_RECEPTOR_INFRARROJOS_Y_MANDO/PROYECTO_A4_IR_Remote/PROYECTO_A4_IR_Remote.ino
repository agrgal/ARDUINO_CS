
#include <IRremote.h> //Biblioteca control infrarrojo
// IRremote by shirriff

int RECV_PIN = 11; // Pin de recepción de 

IRrecv receptor(RECV_PIN); // inicializo objeto de recepción

decode_results resultados; //objeto resultados 

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
}

// *********
// LOOP
// *********
void loop() {
  if (receptor.decode(&resultados)) {
    // Si consigue decodificar un resultado..
    // https://www.pjrc.com/teensy/td_libs_IRremote.html
    
    if (resultados.decode_type == NEC) {
      Serial.print("NEC: ");
    } else if (resultados.decode_type == SONY) {
      Serial.print("SONY: ");
    } else if (resultados.decode_type == RC5) {
      Serial.print("RC5: ");
    } else if (resultados.decode_type == RC6) {
      Serial.print("RC6: ");
    } else if (resultados.decode_type == UNKNOWN) {
      Serial.print("UNKNOWN: ");
    } // imprime el tipo de codificación recibida
    
    
    Serial.println(resultados.value, HEX);
    Serial.println("bits: "+ (String) resultados.bits);
    // Más información con las órdenes 
    // results.rawbuf (un array de los pulsos IR) 
    // results.rawlen (número de items almacenados en el array)
    
    receptor.resume(); // Preparado para recibir el siguiente valor
  }
}
