
// Bibliotecas necesarias
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// La biblioteca SoftwareSerial se usa cuando se necesita instanciar una segunda
// comunicación serie. En este caso lo haremos con nuestro DFPlayerMini
// La comunicación serie "normal" la mantendremos con el PC
// y así veremos información en la pantalla.
SoftwareSerial miSegundaSerial(6, 7); // RX, TX
// Instancio objeto de la clase DFRobotDFPlayerMini
DFRobotDFPlayerMini miMP3;

// *********
// Variables
// *********
int volumen = 10; // valor de volumen, de 0 a 30
unsigned long tiempo=0; // tiempo en ms
int pista=0; // nº de pista a reproducir
int estado = LOW; // estados pulsación de botón
int estadoAnterior = LOW; 

// *********
// SETUP
// *********
void setup()
{
  miSegundaSerial.begin(9600); // a 9600
  Serial.begin(115200); // a 115200
  
  Serial.println();
  Serial.println(F("Demostración DFRobot DFPlayer Mini")); // Serial.println 
  // con función macro F() https://heli.xbot.es/?p=519
  Serial.println(F("Inicializar DFPlayer Mini puede llevar varios segundos"));
  
  if (!miMP3.begin(miSegundaSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Incapaz de arrancar"));
    Serial.println(F("1.Comprueba las conexiones"));
    Serial.println(F("2. Comprueba la tarjeta"));
    while(true){
      delay(0); // Código compatible con la vigilancia ESP8266
    }
  }
  Serial.println(F("DFPlayer Mini Activado"));
  
}

// **************************************************
// *********
// Principal
// *********
void loop()
{

  tiempo = millis(); // tiempo transcurrido desde el inicio del programa

  estado = digitalRead(4); // lee el pin 4

  // Si detecta una subida por flanco en el botón
  if (estado==HIGH && estadoAnterior==LOW) { 
    pista = (pista+1)*(pista<4)+1*(pista>=4); // de la 1 a la 4
    Serial.println("Pista "+(String) pista);
    tocaMusica(pista,10000); // Función que toca la pista 
    // y el programa "no hace nada" en el tiempo estipulado
  }

  if (miMP3.available()) {
    // Cadena que devuelve DFPlayerMini. Podemos 
    // detectar estados y errores.
    printDetail(miMP3.readType(), miMP3.read()); 
  }

  // Cambio de volumen con el potenciómetro
  volumen = map(analogRead(A0),0,1023,1,30);
  miMP3.volume(volumen);
  Serial.println("Volumen "+ (String) volumen);  
 
  estadoAnterior = estado; // refresco del estado
    
} // Fin del Loop
// **************************************************

// *********
// Funciones
// *********
// toca una de las cuatro pistas almacenadas
void tocaMusica(int pista, int tiempo) {
  miMP3.play(pista);  // sd:/mp3/0001.mp3
  // miMP3.next(); // Otra posible orden para tocar la pista "siguiente"
  waitMilliseconds(tiempo); // Espera el tiempo estipulado
  // comentar esta función para observar otro modo de reproducción
}

// Función que espera un tiempo
void waitMilliseconds(uint16_t msWait)
{
  uint32_t start = millis();  
  while ((millis() - start) < msWait)
  {
    delay(1); // El programa no hace nada 
    // mientras dura el sonido activado
  }
}

// Función detallada en la página del proyecto
// Detecta estados y errores
void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

}
