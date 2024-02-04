/* 
 *  Ejemplo de envío de datos por telemetría
 *  Dispositivo: NodeMCU con valores lógicos a 3.3V
 *  Fecha: 8/1/2022
 */

 // Include the RFM69 and SPI libraries:

#include <RFM69.h>
#include <SPI.h>

// Addresses for this node. CHANGE THESE FOR EACH NODE!

#define NETWORKID     100   // Must be the same for all nodes --> esta es la red.
#define MYNODEID      10   // My node ID --> este es el nodo de mi dispositivo
#define TONODEID      20   // Destination node ID --> este es el nodo del otro dispositivo

// RFM69 frequency, uncomment the frequency of your module:

//#define FREQUENCY   RF69_433MHZ
// #define FREQUENCY  RF69_915MHZ
#define FREQUENCY     RF69_868MHZ
#define FREQUENCY_E   868001000UL // frecuencia exacta

// AES encryption (or not):

#define ENCRYPT       true // Set to "true" to use encryption
#define ENCRYPTKEY    "123456789" // Use the same 16-byte key on all nodes --> clave secreta

// Use ACKnowledge when sending messages (or not):

#define USEACK        false // Request ACKs or not

// Packet sent/received indicator LED (optional):

// #define LED           9 // LED positive pin
// #define GND           8 // LED ground pin

// Según la web https://iot.uy/conectar-la-radio-rfm69-hcw-al-esp8266/ 
#define SPI_CS              D8 //NSS o Cable Select al GPIO15 o D8
#define IRQ_PIN             10 // D4, GPIO09/SDD2 (9) (no conectar) - POSIBLES: GPIO10/SDD3 (10),, D1, 
#define IRQ_NUM             10 // Mismo que IRQ_PIN

#define IS_RFM69HCW true // Si tu radio es RFM69HCW entonces va "true"
#define POWER_LEVEL 31 // Valor máximo de potencia

// Create a library object for our RFM69HCW module:

RFM69 radio = RFM69(SPI_CS, IRQ_PIN, IS_RFM69HCW, IRQ_NUM);

// Datos que deben ser registrados
// Cadena del GPS --> $GPRMC,225446,A,4916.45,N,12311.12,W,000.5,054.7,191194,020.3,E*68
String cadenaGPS = "GPRMC,225446,A,4916.45,N,12311.12,W,000.5,054.7,191194,020.3,E*68"; // cadena GPS
const unsigned int codigoSeritium = 11700767; // Código del centro
float temperatura = -25.5; // Ejemplo de temperatura con signo (2 dígitos + 1 decimal)
unsigned int presion = 108444; // Ejemplo de presión atmosférica. Entero positivo sin decimales.
float altura = 1238.5; // Ejemplo de altura. Positivo sin signo (4 dígitos + 1 decimal) 
float ace[3] = {+39.2,-39.2,+39.2}; // valores entre -4g y 4g, en m/s2. Con signo (2 dígitos + 1 decimal)
float ang[3] = {-90.0,+90.0,-45.5}; // valores entre -90 y 90, en grados. Con signo (2 dígitos + 1 decimal)
bool foto = false; // Si el programa ha tomado o no fotografías.


// ======================
// SETUP
// ======================

void setup() {
  
  delay(2000);

  Serial.begin(9600);
  Serial.print("Node ");
  Serial.print(MYNODEID,DEC);
  Serial.println(" ready");

    // Initialize the RFM69HCW:
  // radio.setCS(10);  //uncomment this if using Pro Micro
   
  radio.initialize(FREQUENCY, MYNODEID, NETWORKID);
  radio.setHighPower(); // Always use this for RFM69HCW

  // afinar frecuencia
  radio.setFrequency(FREQUENCY_E); 
  // Saber más en: https://www.aprendiendoarduino.com/tag/banda-ism/
  // rango de 863 a 870 MHz

  // Turn on encryption if desired:

  if (ENCRYPT) {
    radio.encrypt(ENCRYPTKEY); }

  Serial.println("Terminado SETUP...");

}

// ======================
// LOOP
// ======================

void loop() {
  
  delay(800); // cada 500 milisegundos
  enviarDatos(cadenaParaEnviar(1)); // primero llama a la funcion cadenaParaEnviar con valor 1
        // que formatea los datos de temperatura, presión, altura, aceleración, ángulos.
        // Los concatena en una cadena que empieza por @ y termina por #. Tamaño máximo 61 caracteres. 
        // Al principio incluye un 0, primer dato, y separado por ? el código del centro IES Seritium, 
        // Posteriormente, separado por ?, el resto de los datos. Acaba con #        
  delay(800); // cada 500 milisegundos
  enviarDatos(cadenaParaEnviar(2)); // idem caso anterior pero con valor 2.
        // envía la latitud, la longitud y si se ha sacado o no foto.
    
} // Fin del LOOP

// ======================
// Funciones
// ======================

// ********************************
// Enviando cadena
// ********************************
void enviarDatos(String datos) {

  // Envío de datos
  static char sendbuffer[62];
  static int sendlength = 0;
  
  sendlength = datos.length();
  datos.toCharArray(sendbuffer,sendlength);
  Serial.println(datos);
  

  if (USEACK)
  {
  if (radio.sendWithRetry(TONODEID, sendbuffer, sendlength))
    Serial.println("ACK received!");
  else
    Serial.println("no ACK received");
  }
  
  // If you don't need acknowledgements, just use send():
  
  else // don't use ACK
  {
  radio.send(TONODEID, sendbuffer, sendlength);
  Serial.println("Enviado...");
  }

  sendlength=0; 
}

// ********************************
// cadena ya formateada para enviar
// ********************************
String cadenaParaEnviar (int cual) {
  char tmp[62]; // variable de 62 bytes, del 0 al 61
  switch (cual) {
  
    // en este caso formateo los datos de temperatura, presión, altura, aceleración, ángulos.
    case 1: 
    sprintf(tmp,"@%1d?%8d?%1d%3d?%6d?%5d?%1d%3d?%1d%3d?%1d%3d?%1d%3d?%1d%3d?%1d%3d?#",cual,codigoSeritium
               ,(temperatura>=0),(int) (abs(temperatura*10))
               ,presion,(int) (altura*10)
               ,(ace[0]>=0), (int) abs(ace[0]*10),(ace[1]>=0), (int) abs(ace[1]*10),(ace[2]>=0), (int) abs(ace[2]*10)
               ,(ang[0]>=0), (int) abs(ang[0]*10),(ang[1]>=0), (int) abs(ang[1]*10),(ang[2]>=0), (int) abs(ang[2]*10) ); 
    break; 
 
    // en este caso envío los datos de latitud, longitud y si se ha sacado una foto
    case 2:
    sprintf(tmp,"@%1d?%8d?%6d%1s?%7d%1s?%1d?#",cual,codigoSeritium 
                 ,(int) ( 100.0 * parte(cadenaGPS,',',3).toFloat() ), parte(cadenaGPS,',',4)
                 ,(int) ( 100.0 * parte(cadenaGPS,',',5).toFloat() ), parte(cadenaGPS,',',6)
                 ,(int) foto);
    // Serial.println(parte(cadenaGPS,',',0)); // importante usar simple quotas ' ' ' ' 
    break; 
    
    // por defecto 
    default:
    break;
  }  
  return tmp; // devuelve el conjunto de caracteres
}

// ********************************
// encuentra parte de una cadena
// ********************************
String parte(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
