#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>  // Incluimos la biblioteca WiFiClientSecure.h
// #include <ArduinoJson.h>

// https://circuits4you.com/2019/01/11/nodemcu-esp8266-arduino-json-parsing-example/
// https://circuits4you.com/2019/01/10/esp8266-nodemcu-https-secured-get-request/ --> https

const char* ssid = "JAZZTEL_shny"; // Rellena con el nombre de tu red WiFi
const char* password = "3z7s5tvbtu4s"; // Rellena con la contraseña de tu red WiFi

const char *host = "jsonplaceholder.typicode.com"; // incluimos el host
const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80

int n = 1;

// https://jsonplaceholder.typicode.com/comments?postId=7
// Huella--> 0E:81:AA:54:2C:1A:AC:BA:15:A8:92:AD:62:32:59:1B:B2:E8:0D:9E

const char fingerprint[] PROGMEM = "0E 81 AA 54 2C 1A AC BA 15 A8 92 AD 62 32 59 1B B2 E8 0D 9E";

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Conectando...");
  }
}

void loop() 
{
  
  if (WiFi.status() == WL_CONNECTED)   
  {


        WiFiClientSecure httpsClient;    //Declare object of class WiFiClient
       
        Serial.println(host);
       
        Serial.printf("Usando la huella %s'\n", fingerprint);
        httpsClient.setFingerprint(fingerprint);
        httpsClient.setTimeout(15000); // 15 Seconds
        delay(1000);
        
        Serial.print("HTTPS Conectando...");
        int r=0; //retry counter
        while((!httpsClient.connect(host, httpsPort)) && (r < 30)){
            delay(100);
            Serial.print("#");
            r++;
        }
        
        if(r==30) {
          Serial.println("Fallo de conexión");
        }
        else {
          Serial.println("¡¡Conectado a la web!!");
          String Link,line;

          //GET Data
          Link = "/comments?postId=" + String(n);
           
          Serial.print("Preguntamos por la URL: ");
          Serial.println(host+Link);
           
          httpsClient.print(String("GET ") + Link + " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" +               
                       "Connection: close\r\n\r\n");
  
          Serial.println("petición enviada");
  
  
          // Recibiendo las cabeceras                  
          while (httpsClient.connected()) {
            line = httpsClient.readStringUntil('\n');
            if (line == "\r") {
              Serial.println("cabeceras recibidas");
              break;
            }
          }
  
          // Recibiendo la información
          Serial.println("Respuesta:");
          Serial.println("==========");
          
          while(httpsClient.available()){        
             line = httpsClient.readStringUntil('\n');  //Read Line by Line
             Serial.println(line); //Print response
          }
          Serial.println("==========");
          Serial.println("Cerrando conexión");  
        }        
    
  }
  
  delay(10000);

  n = (n+1)*(n<7)+1*(n>=7);
}
