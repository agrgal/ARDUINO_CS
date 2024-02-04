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

// Constantes de conexión WiFi
const char* ssid = "JAZZTEL_shny";
const char* password = "3z7s5tvbtu4s";
String newHostname = "milampara"; // modo local. llamo a milampara.local/ pero no puedo en el móvil ANDROID

IPAddress ip(192,168,1,20); // parámetros de conexión   
IPAddress gateway(192,168,1,1);   
IPAddress subnet(255,255,255,0); 

// definimos salidas y entradas
#define pilotoWiFi D2 // piloto que indica que el sistema está conectado a la red
#define modoManualCual D0 // en modo manual, indica a qué lámpara afecta el cambio del potenciómetro
uint8_t misLED[]={D5,D8}; // definición de los LEDs

// variable para usar con las funciones asíncronas. Parámetro de lectura de get
const char* PARAM_INPUT_1 = "miSlider1"; // Funciona con "name" en el input

// Crear objeto AsyncWebServer en el puerto 80
AsyncWebServer server(80);

// Control iluminación 
String estadoBoton = "manual"; // averigua el boton en estado online o manual. Parto de una situación "manual" 
String miLamp = "DERECHA"; // La lámpara que estoy usando. Inicialmente la DERECHA
int cualES = 0; // 0 --> lámpara derecha y 1 a izquierda
int entrada[] = {0,0}; // valor intensidad inicial de derecha e izquierda
int entradaAnt[] = {0,0}; // entrada antigua inicial de derecha e izquierda

// Otros datos 
int tmpVariacionLuz = 200; //tiempo en el que la luz permanece, en ms.
int maxPasoIntensidad = 10; // Niveles de luz posibles.
                            // 10 parece ser un número óptimo. Si ponemos más pueden darse parpadeos.
long tiempo = 0; // control del tiempo


// ******************
// FUNCIÓN SETUP
// ******************
void setup() {
  
  pinMode(misLED[0], OUTPUT); // lámpara derecha
  pinMode(misLED[1], OUTPUT); // lámpara izquierda
  pinMode(modoManualCual, INPUT); // detecta la entrada izquierda derecha en modo manual
  pinMode(pilotoWiFi, OUTPUT); // Piloto que indica si estamos conectados a WiFi
  
  digitalWrite(pilotoWiFi,LOW); // En principio apagado
   
  analogWriteRange(4096); // Rango de valores a la salida PWM
  Serial.begin(9600); // Monitor serie con 9600 baudios


  // ******************
  // Initialize SPIFFS: conecta con el sistema de archivos
  // ******************
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  
  // ******************
  // Conectando con Wi-Fi
  // ******************
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

  digitalWrite(pilotoWiFi,HIGH); // Encendido cuando wifi se activa
  
  // ******************
  // Iniciar mDNS a direccion milampara.local 
  // ******************
   if (!MDNS.begin(newHostname.c_str()) )
   {             
     Serial.println("Error iniciando mDNS");
   }
   Serial.println("mDNS iniciado con nombre "+newHostname);

  // ******************
  // Print ESP32 Local IP Address
  // ******************
  Serial.println(WiFi.localIP());

  // ******************
  // Ruta por la raíz del servidor
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Ruta del fichero de estilo style.css
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Ruta que establece el modo manual
  server.on("/man1", HTTP_GET, [](AsyncWebServerRequest *request){
    entrada[cualES]=0;
    analogWrite(misLED[cualES],map(entrada[cualES],0,maxPasoIntensidad,0,4096));
    estadoBoton = "manual"; 
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Ruta que establece el modo online
  server.on("/online1", HTTP_GET, [](AsyncWebServerRequest *request){
    estadoBoton = "online"; 
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Ruta a intercambiar lámpara derecha a izquierda en modo online
  server.on("/cual", HTTP_GET, [](AsyncWebServerRequest *request){
    if (miLamp=="DERECHA") {miLamp="IZQUIERDA";} else {miLamp="DERECHA";}
    if (miLamp=="DERECHA") { cualES=0;} else if (miLamp=="IZQUIERDA") { cualES=1;} 
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Botón de refrescar
  server.on("/refrescar", HTTP_GET, [](AsyncWebServerRequest *request){
     request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Obtiene los parámetros get pulsados en la página
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    
    String inputMessage; 
    String inputParam;
    
    // Obtiene el valor del primer parámetro
    if (request->hasParam(PARAM_INPUT_1)) {
      
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      entrada[cualES] = inputMessage.toInt();
      analogWrite(misLED[cualES],map(entrada[cualES],0,maxPasoIntensidad,0,4096)); 
      request->send(SPIFFS, "/index.html", String(), false, processor);          
          
    } else { // Si no obtiene nada
      inputMessage = "No message sent";      
      inputParam = "none";
      entrada[cualES] = 0;
      Serial.println("Errores");
    }

    // Serial.println(inputMessage);
    // Serial.println(inputParam);
    
  });


  // *************
  // Start server
  // *************
  server.onNotFound(notFound); // si no encuentra el servidor  
  server.begin();
  
}


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// ******************
// FUNCIÓN LOOP
// ******************
void loop() {

    MDNS.update(); // MUY IMPORTANTE para que pueda cogerlo por nombre del host. y en el LOOP
    // Instalo service browser en Android

    // Control Manual
    if (estadoBoton=="manual") {      
      tiempo = millis(); 
      // a1) Elección manual de la lámpara, izquierda o derecha
      cualES = digitalRead(modoManualCual);
      if (cualES==0) { miLamp="DERECHA"; } else {miLamp="IZQUIERDA";} 
      Serial.println("Modo manual: lámpara " + miLamp + " ["+ (String) cualES +"]");
      // a2) valor de la entrada
      entrada[cualES]= analogRead(A0);
      entrada[cualES] = map(entrada[cualES],15,1024,0,maxPasoIntensidad);
      if (entrada[cualES]!=entradaAnt[cualES]) {
        suavizado(entrada[cualES],entradaAnt[cualES]); 
      }   
      Serial.println(String (tiempo) +" -- "+ String (entrada[cualES])+ " Vol: "+ String (entrada[cualES]*3.3/100) );
      // a3) Refrescar el valor de entrada antigua
      entradaAnt[cualES] = entrada[cualES];       
    } // fin de reconocimiento manual 
    else {
      Serial.println("Modo online: lámpara " + miLamp + " ["+ (String) cualES +"]");
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
          analogWrite(misLED[cualES],map(i,0,maxPasoIntensidad,0,4096));
           while(millis() < tiempo+tmpVariacionLuz){
            // espere [periodo] milisegundos
           }
      }
   } else if (b>a) {
      for (int i = b; i>=a; i--) {
          analogWrite(misLED[cualES],map(i,0,maxPasoIntensidad,0,4096));
           while(millis() < tiempo+tmpVariacionLuz){
            // espere [periodo] milisegundos
           }
      }    
   }   
}

// *****************************************************
// funcion que reemplaza  STATE por su valor en el HTML
// *****************************************************
String processor(const String& var){  

  if (var == "NIVEL") {
     Serial.println("Mi entrada vale "+ (String) entrada[cualES]);
     return (String) entrada[cualES];
  }

  if (var == "MODO") {
      return (String) estadoBoton;    
  }  
  
  if (var == "CUALES") {
     Serial.println("Mi lámpara es "+ (String) miLamp);
     return (String) miLamp;    
  }

  if (var == "ACTUAR" and estadoBoton=="manual") { return (String) "disabled";}
  
  if (var == "ACTUAR" and estadoBoton=="online") { return (String) "";}
  
  } // Fin de la función
  
