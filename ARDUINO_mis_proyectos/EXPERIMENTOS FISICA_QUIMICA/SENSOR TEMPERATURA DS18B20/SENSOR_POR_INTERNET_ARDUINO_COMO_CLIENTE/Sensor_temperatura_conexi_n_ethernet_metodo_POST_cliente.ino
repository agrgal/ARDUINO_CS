#include <OneWire.h> //Se importan las librerías
#include <DallasTemperature.h>
#include <SPI.h>
#include <Ethernet.h>
 
// Configuración de direccion MAC e IP.
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xBF, 0x79 }; //mac address
 
// Inicia la libreria Ethernet como cliente
EthernetClient client;

// Parte de la temperatura
#define Pin 6 //Se declara el pin donde se conectará la DATA
OneWire ourWire(Pin); //Se establece el pin declarado como bus para la comunicación OneWire
DallasTemperature sensors(&ourWire); //Se instancia la librería DallasTemperature

float temperatura;

String data;

void setup() {
// Inicia el puerto serie.
Serial.begin(9600);
sensors.begin(); //Se inician los sensores
// Inicia la conexión Ethernet y el servidor.
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP"); 
  }
Serial.print("IP local");
Serial.println(Ethernet.localIP());
}
 
void loop() {

sensors.requestTemperatures(); //Prepara el sensor para la lectura

temperatura = sensors.getTempCByIndex(0);
Serial.print(temperatura); //Se lee e imprime la temperatura en grados Celsius
Serial.println(" ºC");
// Serial.print(sensors.getTempFByIndex(0)); //Se lee e imprime la temperatura en grados Fahrenheit
// Serial.println(" ºF"); 

data = "temperatura="+(String)temperatura + "&tiempo=" +String(millis()); // Separados por un &

  if (client.connect("192.168.1.137",80)) { // REPLACE WITH YOUR SERVER ADDRESS
    client.println("POST /temperatura/add.php HTTP/1.1"); // Ruta al fichero
    client.println("Host: 192.168.1.137"); // SERVER ADDRESS HERE TOO
    client.println("Content-Type: application/x-www-form-urlencoded"); 
    client.print("Content-Length: "); 
    client.println(data.length()); 
    client.println(); 
    client.print(data); 
  } 

  if (client.connected()) { 
    client.stop();  // DISCONNECT FROM THE SERVER
    Serial.println("Cliente desconectado");
    Serial.println();
  }
    
    delay(3000); // Espera para dar tiempo al navegador a recibir los datos.

}
