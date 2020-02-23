#include <OneWire.h> //Se importan las librerías
#include <DallasTemperature.h>
#include <SPI.h>
#include <Ethernet.h>
 
// Configuración de direccion MAC e IP.
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xBF, 0x79 }; //mac address
IPAddress ip(192,168,1,35);
 
// Inicia la libreria Ethernet server con el puerto 80 (por defecto el puerto HTTP).
EthernetServer server(80);

// Parte de la temperatura
#define Pin 6 //Se declara el pin donde se conectará la DATA
OneWire ourWire(Pin); //Se establece el pin declarado como bus para la comunicación OneWire
DallasTemperature sensors(&ourWire); //Se instancia la librería DallasTemperature

float temperatura;

void setup() {
// Inicia el puerto serie.
Serial.begin(9600);
sensors.begin(); //Se inician los sensores
// Inicia la conexión Ethernet y el servidor.
Ethernet.begin(mac, ip);
server.begin();
Serial.print("IP local del servidor ");
Serial.println(Ethernet.localIP());
}
 
void loop() {

sensors.requestTemperatures(); //Prepara el sensor para la lectura

temperatura = sensors.getTempCByIndex(0);
Serial.print(temperatura); //Se lee e imprime la temperatura en grados Celsius
Serial.println(" ºC");
// Serial.print(sensors.getTempFByIndex(0)); //Se lee e imprime la temperatura en grados Fahrenheit
// Serial.println(" ºF"); 


EthernetClient client = server.available(); // Escucha a los clientes entrantes.
    if (client) { // Si un cliente se conecta al servidor:
    Serial.println("Nuevo cliente");
    boolean currentLineIsBlank = true; // Marcador para enviar la respuesta desde el servidor.
        while (client.connected()) { // Repite mientas existe clientes conectados:
            if (client.available()) {
            char c = client.read();
            Serial.write(c); // Imprime por el puerto serie la petición del cliente (caracter a caracter). Es mucha informacion
                if (c == '\n' && currentLineIsBlank) { // Se envia la respuesta a una petición de un cliente cuando a finalizado la petición:
                
                // Respuesta:
                client.println("HTTP/1.1 200 OK"); // Enviar un encabezado de respuesta HTTP estándar
                // client.println("Content-Type: text/html");
                client.println("Content-Type: application/json;charset=utf-8");
                client.println("Access-Control-Allow-Origin: *");  //Tells the browser it has accepted its request for data from a different domain (origin).
                client.println("Server: Arduino");           // The data is coming from an Arduino Web Server (this line can be omitted)
                client.println("Connection: close"); // Se cerrará la conexiós despues de enviar la respuesta.
                // client.println("Refresh: 5"); // Refrescar automáticamente la página después de 5 segundos.
                client.println();
                // imprimir tipo {"temperatura":"valor","tiempo":"valortiempo"}
                
                /* client.println("<!DOCTYPE HTML>"); // Tipo de documento.
                client.println("<html>"); // Etiqueta html inicio del documento. 
                client.print("Temperatura actual: ");
                client.print((String)temperatura); // Temperatura del sensor
                client.print(" oC ");
                client.println("<br />"); // Etiqueta html salto de linea. 
                client.println("</html>"); // Etiqueta html fin del documento. */
                
                client.print("["); 
                client.print("{\"temperatura\":\""+(String)temperatura+"\"");
                client.print(",");
                client.println("\"tiempo\":\""+(String)millis()+"\"}");
                client.print("]"); 
                break;
                }
                if (c == '\n') { // Si el caracter es un salto de linea:
                currentLineIsBlank = true; // La petición ha terminado, se respondera a dicha peticón en el sigueitne ciclo.
                } 
                else if (c != '\r') { // Si el caracter no es un retorno de carro (la petición no ha terminado).
                currentLineIsBlank = false; // Seguimos escuchando la petición.
                }
           }
       }
    
    delay(10); // Espera para dar tiempo al navegador a recibir los datos.
    client.stop(); // Cierra la conexión.
    Serial.println("Cliente desconectado");
    Serial.println();
    }
}
