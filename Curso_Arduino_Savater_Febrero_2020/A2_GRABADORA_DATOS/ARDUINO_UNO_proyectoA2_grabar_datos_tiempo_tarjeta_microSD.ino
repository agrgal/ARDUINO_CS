// Para saber más
// https://www.instructables.com/id/SD-Card-Module-With-ESP8266/

#include <SD.h> // Importar librería de tarjetas SD

int cs_PIN = 8; // PIN CS el que se comunica con la tarjeta grabadora microSD

File miFichero; // Declaro objeto de tipo fichero

String dato =""; // cadena vacía con los datos
String nomFichero = "tiempos0.csv"; // tiene que ser del tipo 8.3
// Es decir nombre de 8 caracteres ASCII + "punto" + extensión 3 caracteres ASCII

unsigned long tiempo=0; // variable general de tipo largo. Hasta 2^32-1 valor
int x = 1000 ; // tiempo en milisegundos, mayor que 250ms

int valor = 0; // recogemos los valores del sensor

// **********************
// SETUP
// **********************

void setup() {

   Serial.begin(9600); // Inicializo la comunicación serie
   pinMode(6,OUTPUT); // Inicializo el pin 6 como salida para el led
  
   if (!SD.begin(cs_PIN)) { // Si no es capaz de inicializar la tarjeta...
      Serial.println("La tarjeta está fallando");
      return;       // Resetea la tarjeta
     } else  {
      Serial.println("Tarjeta inicializada.");  
      if (SD.exists(nomFichero)) { // Si el fichero existe...
        Serial.println("El fichero "+nomFichero+" ya existe. Lo borro.");
        SD.remove(nomFichero); // lo borra
        // Así lo puedo usar indefinidamente con el mismo nombre
      }      
    } // fin del if
  
}

// **********************
// LOOP
// **********************

void loop() {
  
  tiempo = millis(); // tiempo en milisegundos transcurridos desde que
  // se inicializa Arduino 
  if (tiempo%x==0) { // Cada 'x' milisegundos
     valor = analogRead(A0); // lectura del sensor
     dato = String(tiempo)+";"+String(valor); // transforma a cadena el dato del tiempo y del valor
     Serial.println("Tiempo transcurrido y valor : "+dato); // imprime
     // en el monitor serie el dato de tiempo y el valor
     escribir(miFichero,dato); // Llamada a la función que escribe
     // el dato en la tarjeta
  }
}


// ********************************************
// Funciones
// ********************************************

// F1) Función que escribe un dato en la tarjeta
int escribir(File fic, String valor) {
  
  // Abre el fichero en modo de escritura
  fic = SD.open(nomFichero, FILE_WRITE);

  // Si el fichero está disponible, escribe un dato.
  if (fic) { // devuelve TRUE si ha podido abrirlo
    fic.println(valor); // escribe el valor en la tarjeta
    fic.close(); // cierra el fichero
    // Encendido del LED
    digitalWrite(6,HIGH);
    delay(200);
    digitalWrite(6,LOW);
  }
  // ¿No se abre el fichero? Error
  else {
    Serial.println("Error abriendo fichero "+nomFichero);
  }

}
