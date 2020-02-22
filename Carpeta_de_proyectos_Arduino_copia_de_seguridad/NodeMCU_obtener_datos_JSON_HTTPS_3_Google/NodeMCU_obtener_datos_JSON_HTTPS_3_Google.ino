/*
  Envio de datos para Google Sheets
  Hardware Utilizado: Nodemcu v1.0, DHT11
  Autor: Yhan Christian Souza Silva - data: 27/07/2018
  Referências: https://youtu.be/fS0GeaOkNRw
  Modificación: Aurelio Gallardo. 06/07/2019
*/

// -- Bibliotecas auxiliares --

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
// #include "DHT.h"

// -- Hardware --

// #define DHTPIN D2
// #define DHTTYPE DHT11

// DHT dht(DHTPIN, DHTTYPE);


// -- Variables y constantes --
const char* ssid = "JAZZTEL_shny"; // Rellena con el nombre de tu red WiFi
const char* password = "3z7s5tvbtu4s"; // Rellena con la contraseña de tu red WiFi

const char* host = "script.google.com"; // Este es el host de los scripts de google.
const int httpsPort = 443;

// Huella digital del script de Google:
// D4:9E:40:F4:53:7A:04:93:38:F7:6B:4B:DC:70:02:A9:03:98:C2:DE
const char* fingerprint = "D4 9E 40 F4 53 7A 04 93 38 F7 6B 4B DC 70 02 A9 03 98 C2 DE";

// const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";

String googleSheetsID = "AKfycbzxJZ06IyF1XR8qjloDE0FE7_OBLpRdDJaSngisNotRSZS-gUBY"; // El que me da al implementar una aplicación web en el script.
// https://script.google.com/macros/s/AKfycbzxJZ06IyF1XR8qjloDE0FE7_OBLpRdDJaSngisNotRSZS-gUBY/exec?TEMPERATURA=20&HUMEDAD=60

WiFiClientSecure cliente;

long previusMillis;
const long interval = 5000; //cada 5 segundos
int temperature, humidity;
float lastH, lastT;

// -- Setup -- 

void setup() {
  // dht.begin();
  Serial.begin(115200);
  connectToWiFi();
}

// -- LOOP: lectura del sensor y envío de datos según el intervalo--

void loop() {
  if (millis() - previusMillis >= interval) {
    readSensor();
    previusMillis = millis();
  }
}

// -- Funciones auxiliares --

// -- Conectando a la red Wifi. Muestra la IP recibida --

void connectToWiFi() {
  Serial.println("Conectando a rede: ");
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

/* Lectura del sensor de temperatura y humedad
 */

/* void readSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.print("Temperatura: ");
    Serial.print(lastH);
    Serial.print(" Umidade: ");
    Serial.println(lastT);
    temperature = (int) lastT;
    humidity = (int) lastH;
    sendDataToGoogleSheets(lastT, lastH);
  } else {
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.print(" Umidade: ");
    Serial.println(h);
    temperature = (int) t;
    humidity = (int) h;
    sendDataToGoogleSheets(t, h);
    lastH = h;
    lastT = t;
  }
} */

void readSensor() {
  sendDataToGoogleSheets(35,47);
}

/* Función de conexión. Importante la instrucción  cliente.setInsecure(); para conectar de forma anónima
 */

void sendDataToGoogleSheets(int temp, int hum) {
  
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
    Serial.println("Comprobar certificado");
  }

  String stringTemp = String(temp, DEC);
  String stringHum = String(hum, DEC);
  String url = "/macros/s/" + googleSheetsID + "/exec?TEMPERATURA=" + stringTemp + "&HUMEDAD=" + stringHum;
  
  Serial.print("Petición  URL ");
  Serial.println(url);

  cliente.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("Request enviada");
  while (cliente.connected()) {
    String line = cliente.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Cabeceras Recibidas");
      Serial.println(line);
      break;
    }
  }
  
  String line = cliente.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("Éxito");
  } else {
    Serial.println("El envío falló!");
  }
  
  Serial.println("Respuesta:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("¡Cerrando la conexión!");
}
