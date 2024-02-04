/* 
 *  Ejemplo de envío de datos por telemetría
 *  Dispositivo: NodeMCU con valores lógicos a 3.3V
 *  Fecha: 8/1/2022
 */

 // Include the RFM69 and SPI libraries:

#include <RFM69.h>
#include <SPI.h>
#include <Adafruit_BMP085.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include <SoftwareSerial.h> 

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
#define SPI_CS              10 //NSS o Cable Select al GPIO15 o D8
#define IRQ_PIN             2 // D4, GPIO09/SDD2 (9) (no conectar) - POSIBLES: GPIO10/SDD3 (10),, D1, 
#define IRQ_NUM             2 // Mismo que IRQ_PIN

#define IS_RFM69HCW true // Si tu radio es RFM69HCW entonces va "true"
#define POWER_LEVEL 31 // Valor máximo de potencia

// Create a library object for our RFM69HCW module:

RFM69 radio = RFM69(SPI_CS, IRQ_PIN, IS_RFM69HCW, IRQ_NUM);
MPU6050 sensor; // Iniciamos el sensor MPU6050.
Adafruit_BMP085 bmp;  

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

// -=- DATOS DEL GIRSOCOPIO -=-
short int ax, ay, az;
short int gx, gy, gz;  // Valores sin procesar del acelarómetro y giroscopio.
long p0;  // Valor de la presión inicial. Se toma al encender el CANSAT.
long tiempo_prev;
float dt; 
// Variables que calculan el tiempo (Cálculos aceleromtro/giroscopio)
float ang_x, ang_y, ang_z;
float ang_x_prev, ang_y_prev, ang_z_prev;  // Variables para calcular los ángulos relativos de rotación.

//-=- Variables para el LED
unsigned long t = 0;   

// ======================
// SETUP
// ======================

void setup() {
  
  delay(2000);

  Serial.begin(9600);
  Serial.print("Node ");
  Serial.print(MYNODEID,DEC);
  Serial.println(" ready");

  // Biblioteca WIRE
  Wire.begin();    

  sensor.initialize();
  if(sensor.testConnection()){                                                                            //Iniciamos sensores + posibles mensajes de error.
    Serial.println("Sensor MPU6050 iniciado correctamente");
  }
  else{
    Serial.println("Error al iniciar el sensor MPU6050");
  }
  
  if(!bmp.begin()){
    Serial.print("Error al iniciar el sensor BMP180");
  }
  else{
    Serial.println("Sensor BMP180 iniciado correctamente");
  }
  
  p0 = bmp.readPressure();
  Serial.print("Presión inicial = ");
  Serial.println(p0);
  Serial.println("------------------------------------------"); 

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

  leerGY87();
  
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

void leerGY87(){
  
  sensor.getAcceleration(&ax, &ay, &az);
  sensor.getRotation(&gx, &gy, &gz);                                                                        //Obtenemos las lecturas de aceleración y girsocopio. 
  dt = (millis()-tiempo_prev)/1000.0;
  tiempo_prev = millis();                                                                                   //Calculamos el tiempo pasado entre cada medición. 
  float ang_x_accel = atan(ax/sqrt(pow(ay,2) + pow(az,2)))*(180.0/3.14);
  float ang_y_accel = atan(ay/sqrt(pow(az,2) + pow(ax,2)))*(180.0/3.14);
  float ang_z_accel = atan(az/sqrt(pow(ax,2) + pow(ay,2)))*(180.0/3.14);                                    //Calculamos los angulos de inclinación en los ejes usando el acelerómetro.
  ang_x = 0.98*(ang_x_prev+(gx/131)*dt) + 0.02*ang_x_accel;
  ang_y = 0.98*(ang_y_prev+(gy/131)*dt) + 0.02*ang_y_accel;
  ang_z = 0.98*(ang_z_prev+(gy/131)*dt) + 0.02*ang_z_accel;                                                 //Calculamos la rotación usando tanto el girsocopio como el acelerómetro para eliminar errores.
  ang_x_prev = ang_x;
  ang_y_prev = ang_y;
  ang_z_prev = ang_z;
  ace[2] = az * (9.81/16384.0);
  ace[1] = ay * (9.81/16384.0);
  ace[0] = ax * (9.81/16384.0);                                                                             //Calculamos la aceleración en m/s2 en los tres ejes 
  ang[0] = ang_x_accel;  
  ang[1] = ang_y_accel;
  ang[2] = ang_z_accel; 
  altura = bmp.readAltitude(p0);
  presion = bmp.readPressure();
  //float m_az_ax = sqrt(pow(sqrt((pow(ax_ms2, 2)) + (pow(az_ms2, 2))), 2) + (pow(ay_ms2, 2)));             //Calculamos el módulo de los 3 vectores, para así obtener la aceleración que experimenta.--> NO SE USA POR AHORA.
  Serial.print("Rotación en X: ");
  Serial.print(ang[0]); 
  Serial.print("º\tRotación en Y: ");
  Serial.print(ang[1]);
  Serial.print("º \tAceleración de caída(m/s2): ");
  Serial.print(ace[2]);  
  Serial.print("\tTemperatura = ");
  temperatura = bmp.readTemperature();
  Serial.print(temperatura);
  Serial.print("ºC\tPresión = ");
  Serial.print(presion);
  Serial.print("\tAltitud respecto al suelo = ");
  Serial.print(altura);
  Serial.println("m");                                                                                      //Mostramos por puerto serie los datos obtenidos. 
}
