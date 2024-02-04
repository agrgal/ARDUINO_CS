/* Información.  
 * proyecto basado en el ejemplo de Rui Santos.
 * https://randomnerdtutorials.com/esp8266-web-server-spiffs-nodemcu/
 * 
 * https://randomnerdtutorials.com/esp32-esp8266-input-data-html-form/
 * 
 */

// Bibliotecas
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h> // para la comunicación del ESP8266
#include <ESP8266mDNS.h> // librería mDNS para poder llamarlo por nombre local
#include <FS.h> // para los ficheros

// #include "conectarWifi.h" // carga conexión de ficheros

// Constantes
const char* ssid = "JAZZTEL_dsfs";
const char* password = "cvxcvzxcvxcv";
String newHostname = "milampara";

IPAddress ip(192,168,1,20);     
IPAddress gateway(192,168,1,1);   
IPAddress subnet(255,255,255,0); 


#define miLed D5 
String nivelLUZ;
String estadoLED;

const char* PARAM_INPUT_1 = "miSlider1"; // Funciona con "name" en el input

String estadoBoton = "online"; // averigua el boton en estado online o manual

// Crear objeto AsyncWebServer en el puerto 80
AsyncWebServer server(80);

long tiempo = 0;

// Control manual 
int entrada = 0;
int entradaAnt = 0;
int maxPasoIntensidad = 10; // Niveles de luz posibles.
                            // 10 parece ser un número óptimo. Si ponemos más pueden darse parpadeos.
int tmpVariacionLuz = 100; //tiempo en el que la luz permanece, en ms.

// Control página web


void setup() {
  
  pinMode(miLed, OUTPUT);  
  analogWriteRange(4096); // Rango de valores a la salida PWM
  Serial.begin(9600);


  // Initialize SPIFFS: conecta con el sistema de archivos
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);  // Modo estático
  WiFi.config(ip,gateway,subnet);
  WiFi.hostname(newHostname.c_str()); // establece el hostname
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
    //Get Current Hostname
    Serial.printf("New hostname: %s\n", WiFi.hostname().c_str());
  }
  
  // Iniciar mDNS a direccion esp8266.local
   if (!MDNS.begin(newHostname.c_str()) )
   {             
     Serial.println("Error iniciando mDNS");
   }
   Serial.println("mDNS iniciado con nombre "+newHostname);

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  /* Route to set GPIO to HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(miLed, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to set GPIO to LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(miLed, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  }); */


  // Route to set GPIO to HIGH
  server.on("/man1", HTTP_GET, [](AsyncWebServerRequest *request){
    entrada=0;
    analogWrite(miLed,map(entrada,0,maxPasoIntensidad,0,4096));
    estadoBoton = "manual"; 
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to set GPIO to LOW
  server.on("/online1", HTTP_GET, [](AsyncWebServerRequest *request){
    estadoBoton = "online"; 
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Botón de refrescar
  server.on("/refrescar", HTTP_GET, [](AsyncWebServerRequest *request){
     request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {

    
    String inputMessage; 
    String inputParam;
    
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      entrada = inputMessage.toInt();
      analogWrite(miLed,map(entrada,0,maxPasoIntensidad,0,4096)); 
      request->send(SPIFFS, "/index.html", String(), false, processor);          
          
    } else {
      inputMessage = "No message sent";      
      inputParam = "none";
      entrada = 0;
      Serial.println("Errores");
    }

    // Serial.println(inputMessage);
    // Serial.println(inputParam);
    
  });


  // Start server

  server.onNotFound(notFound);
  
  server.begin();
  
}


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// the loop function runs over and over again forever
void loop() {

    MDNS.update(); // MUY IMPORTANTE para que pueda cogerlo por nombre del host. y en el LOOP
    // Instalo service browser en Android

    if (estadoBoton=="manual") {
       tiempo = millis()/1000; 
      // A) Control Manual
      // a1) valor de la entrada
      entrada = analogRead(A0);
      entrada = map(entrada,15,1024,0,maxPasoIntensidad);
      if (entrada!=entradaAnt) {
        suavizado(entrada,entradaAnt); 
        }
         Serial.println(String (tiempo) +" -- "+ String (entrada)+ " Vol: "+ String (entrada*3.3/100) );
      // a2) valor entrada antigua
        entradaAnt = entrada;       
    }
    
    
} // fin del loop

// =========
// FUNCIONES
// =========

// **********************************************
// funcion que suaviza los cambios bruscos de luz
// **********************************************
void suavizado(int a, int b) {
   if (a>b) {
      for (int i = b; i<=a; i++) {
          analogWrite(miLed,map(i,0,maxPasoIntensidad,0,4096));
          delay(tmpVariacionLuz);
      }
   } else if (b>a) {
      for (int i = b; i>=a; i--) {
          analogWrite(miLed,map(i,0,maxPasoIntensidad,0,4096));
          delay(tmpVariacionLuz);
      }    
   }   
}

// *****************************************************
// funcion que reemplaza  STATE por su valor en el HTML
// *****************************************************
String processor(const String& var){
  
  /* if(var == "STATE"){
    if(digitalRead(miLed)){
      estadoLED = "ON";
    }
    else{
      estadoLED = "OFF";
    }
    Serial.print(estadoLED);
    return estadoLED;
  } */

  if (var == "NIVEL") {
     Serial.println("Mi entrada vale "+ (String) entrada);
     return (String) entrada;
  }

  if (var == "MODO") {
      return (String) estadoBoton;    
  }

  if (var == "ACTUAR" and estadoBoton=="manual") { return (String) "disabled";} else { return (String) "";}
  
  } // Fin de la función
  
