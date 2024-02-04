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
#define MYNODEID      20   // My node ID --> este es el nodo de mi dispositivo
#define TONODEID      10   // Destination node ID --> este es el nodo del otro dispositivo

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
#define IRQ_PIN             D1 // DIO0 conectado a GPIO4
#define IRQ_NUM             D1 //IRQ igual al pin

#define IS_RFM69HCW true // Si tu radio es RFM69HCW entonces va "true"
#define POWER_LEVEL 31 // Valor máximo de potencia

// Create a library object for our RFM69HCW module:

RFM69 radio = RFM69(SPI_CS, IRQ_PIN, IS_RFM69HCW, IRQ_NUM);

// Datos que deben ser registrados
// GPS --> 
String latitud ="";
String longitud="";
// Valores
unsigned int codigoSeritium = 0; // Código del centro
float temperatura = 0.0; // Ejemplo de temperatura con signo (2 dígitos + 1 decimal)
unsigned int presion = 0; // Ejemplo de presión atmosférica. Entero positivo sin decimales.
float altura = 0.0; // Ejemplo de altura. Positivo sin signo (4 dígitos + 1 decimal) 
float ace[3] = {0.0, 0.0, 0.0}; // valores entre -4g y 4g, en m/s2. Con signo (2 dígitos + 1 decimal)
float ang[3] = {0.0, 0.0, 0.0}; // valores entre -90 y 90, en grados. Con signo (2 dígitos + 1 decimal)
bool foto = false; // Si el programa ha tomado o no fotografías.
// potencia recibida
int potencia = 0;

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
  
  delay(100); // tiene que ser más rápido que el envío de datos... Si se envían cada 500 ms, por ejemplo, 
              // aquí ponemos la mitad o así... 
  String cadena = recibirDatos(); 
  if (cadena.length()>2) {analizaCadena(cadena); } // lama a la función que empieza a leer los datos. 
    
} // Fin del LOOP

// ======================
// Funciones
// ======================

// ********************************
// Recibiendo cadena
// ********************************
String recibirDatos() {

  char temp[63]; // un byte más de lo necesario
  // String tmp;
    
  if (radio.receiveDone()) // Got one!
  {
    // Print out the information:

    // Serial.println( (int) radio.DATALEN);
    // Serial.println(radio.DATA);  
    
    // The actual message is contained in the DATA array,
    // and is DATALEN bytes in size:

    for (byte i = 0; i < radio.DATALEN; i++) {
      char n = (char)radio.DATA[i];
      if (n=='#') {
        temp[i]=NULL ; break;        
      } else if (n=='@') {
        temp[i]='?';
      } else {
        temp[i]= n;
      }
      // Serial.print((char)radio.DATA[i]);
      // Serial.print(temp[i]);
      // tmp.concat(temp[i]); 
    }

    // int bufferLength = sizeof(temp);  
    // Serial.println("Tamaño del buffer: " + (String) bufferLength);
    
    // Serial.print("received from node ");
    // Serial.print(radio.SENDERID, DEC);
    // Serial.print(", message [");  
    // for (int j=0; j<bufferLength; j++) {
      // Serial.print(temp[j]);
    // }

    // RSSI is the "Receive Signal Strength Indicator",
    // smaller numbers mean higher power.

    // Serial.print("], RSSI ");
    // Serial.println(radio.RSSI);
    potencia = (int) radio.RSSI; 

    // Send an ACK if requested.
    // (You don't need this code if you're not using ACKs.)

    if (radio.ACKRequested())
    {
      radio.sendACK();
      Serial.println("ACK sent");
    }

  }  

  return String(temp);
  // return tmp;
}

// ********************************
// análisis de cadena
// ********************************

void analizaCadena (String trama) {

  String d; // dato que recibe 
  String comodin; // cadena comodin
  d = parte(trama,'?',1); // debe recibir o un 1 o un 2
  codigoSeritium = parte(trama,'?',2).toInt();
  
  switch (d.toInt()) {

    // recibo la primera cadena
    case 1:
        Serial.println("Potencia: " + (String) potencia + " // Cadena 1 --> " + trama);
        Serial.println("Recibido para el centro: "+ (String) codigoSeritium);
        // Temperatura
        temperatura = recValorFloat (parte(trama,'?',3), 10, true); // cadena a recuperar, divisor, si tiene signo
        // Presión
        comodin = parte(trama,'?',4);
        presion = comodin.toInt();
        // Altura
        altura = recValorFloat (parte(trama,'?',5), 10, false);
        // Aceleracion
        comodin ="Aceleraciones: ";
        for (int i=0;i<3;i++) {
          ace[i]=recValorFloat (parte(trama,'?',i+6), 10, true); // valores 6, 7 y 8
          comodin = comodin + "A"+char(120+i)+" = "+ace[i]+" m/s2 ~ ";
        }
        comodin = comodin.substring(0,comodin.length()-2);
        // Ángulos
        comodin = comodin + "\nÁngulos: ";
        for (int i=0;i<3;i++) {
          ang[i]=recValorFloat (parte(trama,'?',i+9), 10, true); // valores 9, 10 y 11
          comodin = comodin + "ANG"+char(120+i)+" = "+ang[i]+" º ~ ";
        }
        comodin = comodin.substring(0,comodin.length()-2);
        // Muestra los datos en el monitor serie
        Serial.println("T = "+(String) temperatura +" º C // "+
                       "P = "+(String) presion +" Pa // "+
                       "h = "+(String) altura +" m // ");
        Serial.println(comodin);
    break;
    // ************************************************
    
    // recibo la segunda cadena
    case 2:
        Serial.println("Potencia: " + (String) potencia + " // Cadena 2 --> " + trama);
        Serial.println("Recibido para el centro: "+ (String) codigoSeritium);
        // latitud
        comodin = parte(trama,'?',3);
        latitud = comodin.substring(0,2)+"º "+(String) recValorFloat(comodin.substring(2,6), 100, false) 
                  +"' "+ cambioLetra(comodin.substring(6,7));
        // longitud
        comodin = parte(trama,'?',4);
        longitud = comodin.substring(0,3)+"º "+(String) recValorFloat(comodin.substring(3,7), 100, false) 
                  +"' "+ cambioLetra(comodin.substring(7,8));
        // Fotografía
        comodin = parte(trama,'?',5);
        foto = (comodin=="1");
        comodin = (foto) ? "Sí" : "No";
        Serial.println("LAT: "+latitud+ " // LON: "+longitud);
        Serial.println("¿Se ha tomado una foto?: "+ comodin);
        
    break; 
    // *******************************************
    
    default:
        // Serial.println("No he encontrado una cadena: " + trama);
        return; // sale de la función
    break;
    
  }

  // imprime línea de caracteres
  int iguales = 91;
  while (--iguales) { Serial.print("="); } 
  Serial.print("\n");

}

// ********************************
// Recupera valores float
// ********************************
float recValorFloat (String num, int divisor, bool signo) {
  float valor = 0.0;
  int longitud = num.length();
  if (signo) {
    valor = num.substring(1,longitud).toFloat()/divisor;
    valor = (num.substring(0,1)=="0") ? valor = -1*valor : valor;
  } else {
    valor = num.toFloat()/divisor;
  }
  return valor;
}

// ********************************
// cambio letra en longitud
// ********************************
String cambioLetra (String letra) {
  return (letra=="W") ? "Oeste" : (letra=="E") ? "Este" : (letra=="N") ? "Norte" : (letra=="S") ? "Sur" : "" ;
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
