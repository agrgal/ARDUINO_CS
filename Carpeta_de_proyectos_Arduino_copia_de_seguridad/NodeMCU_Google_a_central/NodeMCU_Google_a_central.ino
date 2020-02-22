
/*
  Envio de datos para Google Sheets
  Hardware Utilizado: Nodemcu v1.0,
  Autor:Aurelio Gallardo Rodríguez
*/

// -- Bibliotecas auxiliares --

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

// -- Hardware --


// -- Variables y constantes --
const char* ssid = "JAZZTEL_shny"; // Rellena con el nombre de tu red WiFi
const char* password = "3z7s5tvbtu4s"; // Rellena con la contraseña de tu red WiFi

const char* host = "script.google.com"; // Este es el host de los scripts de google.
const int httpsPort = 443;

// Huella digital del script de Google:
// D4:9E:40:F4:53:7A:04:93:38:F7:6B:4B:DC:70:02:A9:03:98:C2:DE
const char* fingerprint = "D4 9E 40 F4 53 7A 04 93 38 F7 6B 4B DC 70 02 A9 03 98 C2 DE";
// const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";

String googleSheetsID = "AKfycbz73SQeEiIrWJ3ZszPZnJmlrX45KwwSOPUI33-mr-yFw-D7kBMl"; // El que me da al implementar una aplicación web en el script.
// Script EscribeDatos --> Código.gs --> De la hoja "DatosIntermedios"
// https://script.google.com/macros/s/AKfycbz73SQeEiIrWJ3ZszPZnJmlrX45KwwSOPUI33-mr-yFw-D7kBMl/exec


WiFiClientSecure cliente;
DynamicJsonDocument doc(1024);

int alarma = 0; // detecta si se ha activado la alarma

int tiempoActivacionMinimo = 3000; // Tiempo en ms de activación mínimo

#define LED 5 // D1(gpio5)

// -- Setup -- 

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  connectToWiFi();
}

// -- LOOP: lectura del sensor y envío de datos según el intervalo--

void loop() {

  sendDataToGoogleSheets("CENTRAL");  // se conecta a Google Spreadsheet

  if (alarma>=1) {
    digitalWrite(LED,true); // simplemente cuando cambie el valor de "alarma" se ilumina
    delay(tiempoActivacionMinimo); // dura encendida un tiempo
  } else {
    digitalWrite(LED,false);    
  }

  delay(2000); // Entre conexión y conexión 1 segundo. 
  
}

// -- Funciones auxiliares --

// -- Conectando a la red Wifi. Muestra la IP recibida --

void connectToWiFi() {
  Serial.println("Conectando a la red: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  WiFi.mode(WIFI_STA);
  Serial.println("");
  Serial.println("Conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  delay(1000);
}


/* Función de conexión. Importante la instrucción  cliente.setInsecure(); para conectar de forma anónima
 */

void sendDataToGoogleSheets(String nombreEstacion) {

  String cadena="";
  
  Serial.print("Conectando a: ");
  Serial.println(host);

  cliente.setInsecure();
  
  if (!cliente.connect(host, httpsPort)) {
    Serial.println("Falla la conexión a Google Sheets -->" + String(host) + ": "+ String(httpsPort) );
    return;
  }
  
  if (cliente.verify(fingerprint, host)) {
    Serial.println("Certificado OK");
  }
  else {
    Serial.println("Debes comprobar certificado");
  }

  String url = "/macros/s/" + googleSheetsID + "/exec?estacion=" + nombreEstacion;
  
  Serial.print("Petición  URL ");
  Serial.println(url);

  cliente.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  // Es necesario leer las cabeceras   
  Serial.println("Request enviada");
  while (cliente.connected()) {
    String line = cliente.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Cabeceras Recibidas");
      Serial.println(line);
      break;
    }
  }

  // lee el contenido de la respuesta y lo almacena en la variable cadena
  while (cliente.available()) {
      char c = cliente.read();      
      cadena = cadena + (String) c;
  }  
  Serial.println("Respuesta directa del servidor");
  Serial.println(cadena); 
  Serial.println("==============================");
  Serial.println();
  
  // PROBLEMA: la respuesta, por motivos de seguridad, no es jamás el texto, o el html, sino que está codificada.
  // He resuelto este problema encerrando la respuesta (JSON) entre dos palabras, EMPEZAR y TERMINAR.
  // Una vez localizada la información, la extraigo con la función midString.
  // Los caracteres, en codificación unicode, los transformo en sus caracteres con varias líneas de código...
  // Y voalá... ¡obtiene la respuesta en formato JSON de texto!
  String respuesta =midString(cadena,"EMPEZAR","TERMINAR");
  char comillas = 34;
  respuesta.replace("x7b","{");
  respuesta.replace("x7d","}");
  respuesta.replace("x22",String(comillas));
  respuesta.replace("\\",""); // doble slash para que lo reconozca ??
  // Serial.println(respuesta);

  Serial.println("Respuesta filtrada desde el servidor"); 
  Serial.println(respuesta);
  Serial.println("==============================");
  Serial.println();

  // Usar bibliteca JSON
  deserializeJson(doc,respuesta);
  JsonObject obj = doc.as<JsonObject>();

  alarma = (int) obj[String("alarma")]; // extraigo la variable alarma y la pongo en la variable del mismo valor.
    
  Serial.println("Respuesta extraída en variables deserializando la cadena en un objeto JSON");
  Serial.println("Estado de la alarma: "+(String) alarma);
  Serial.println("==============================");
  Serial.println(); 
  
  cliente.stop(); //cierra la conexión

  // ===========================
  /* Obteniendo la respuesta  */
  // ===========================

 
  
  /*
  String line = cliente.readStringUntil('\n');
  Serial.println(line);
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("Éxito");
  } else {
    Serial.println("El envío falló!");
  }
  
  Serial.println("Respuesta:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("¡Cerrando la conexión!"); */
  
}


/* función texto intermedio */
String midString(String str, String start, String finish){
  int locStart = str.indexOf(start);
  if (locStart==-1) return "";
  locStart += start.length();
  int locFinish = str.indexOf(finish, locStart);
  if (locFinish==-1) return "";
  return str.substring(locStart, locFinish);
}
