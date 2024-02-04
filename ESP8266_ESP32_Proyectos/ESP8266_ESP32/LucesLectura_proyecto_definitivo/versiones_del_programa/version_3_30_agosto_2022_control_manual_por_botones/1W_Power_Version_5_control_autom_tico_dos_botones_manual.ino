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
const char* ssid = "JAZZTEL_dsfs";
const char* password = "cvxcvzxcvxcv";
String newHostname = "milampara"; // modo local. llamo a milampara.local/ pero no puedo en el móvil ANDROID

IPAddress ip(192,168,1,20); // parámetros de conexión   
IPAddress gateway(192,168,1,1);   
IPAddress subnet(255,255,255,0); 

// definimos salidas y entradas
#define pilotoWiFi D2 // piloto que indica que el sistema está conectado a la red
uint8_t misLED[]={D5,D8}; // definición de los LEDs
uint8_t misBotones[]={D3,D1}; // botones de entrada manual

// variable para usar con las funciones asíncronas. Parámetro de lectura de get
const char* PARAM_INPUT_1 = "miSlider1"; // Funciona con "name" en el input

// Crear objeto AsyncWebServer en el puerto 80
AsyncWebServer server(80);

// Control iluminación 
String miLamp = "DERECHA"; // La lámpara que estoy usando. Inicialmente la DERECHA
int cualES = 0; // 0 --> lámpara derecha y 1 a izquierda
int entrada[] = {0,0}; // valor intensidad inicial de derecha e izquierda
// int entradaAnt[] = {0,0}; // entrada antigua inicial de derecha e izquierda
// Variables de estado de los botones manuales
int estadoBoton[] = {0,0};        // estado actual
int estadoBotonAnt[] = {0,0};     // estado previo
int factor[] = {1,1}; // factor para subir o bajar la intensidad
int pulsacion = 0; // inicialmente la pulsacion es 0
                   // Esta variable evita una activacion espontánea de los pulsadores PULLUP al arrancar
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
  pinMode(misBotones[0], INPUT_PULLUP); // botón manual derecho
  pinMode(misBotones[1], INPUT_PULLUP); // botón manual izquierdo
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

  // Ruta a intercambiar lámpara derecha a izquierda en modo online
  server.on("/cual", HTTP_GET, [](AsyncWebServerRequest *request){
    if (miLamp=="DERECHA") {miLamp="IZQUIERDA";} else {miLamp="DERECHA";}
    if (miLamp=="DERECHA") { cualES=0;} else if (miLamp=="IZQUIERDA") { cualES=1;} 
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Apaga todas las lámparas
  server.on("/apagar", HTTP_GET, [](AsyncWebServerRequest *request){
    for (int i=0;i<=1;i++) {      
        entrada[i]=0;
        factor[i]=1; // Al apagar estan preparadas para aumentar
        analogWrite(misLED[i],map(entrada[i],0,maxPasoIntensidad,0,4096)); 
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Enciende todas las lámparas
  server.on("/encender", HTTP_GET, [](AsyncWebServerRequest *request){
    for (int i=0;i<=1;i++) {      
        entrada[i]=10;
        factor[i]=-1; // Al encender estan preparadas para disminuir
        analogWrite(misLED[i],map(entrada[i],0,maxPasoIntensidad,0,4096)); 
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Media luz todas las lámparas
  server.on("/media", HTTP_GET, [](AsyncWebServerRequest *request){
    for (int i=0;i<=1;i++) {      
        entrada[i]=5; // dejo igual los factores
        analogWrite(misLED[i],map(entrada[i],0,maxPasoIntensidad,0,4096)); 
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Disminuir luz todas las lámparas
  server.on("/disminuir", HTTP_GET, [](AsyncWebServerRequest *request){
    for (int i=0;i<=1;i++) {      
        entrada[i]=(entrada[i]-1)*(entrada[i]>0);
        factor[i]=1; // estamos disminuyendo
        analogWrite(misLED[i],map(entrada[i],0,maxPasoIntensidad,0,4096)); 
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Aumentar luz todas las lámparas
  server.on("/aumentar", HTTP_GET, [](AsyncWebServerRequest *request){
    for (int i=0;i<=1;i++) {      
        entrada[i]=(entrada[i]+1)*(entrada[i]<10)+10*(entrada[i]==10);
        factor[i]=1; // estamos aumentando
        analogWrite(misLED[i],map(entrada[i],0,maxPasoIntensidad,0,4096)); 
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Intercambiar
  server.on("/intercambiar", HTTP_GET, [](AsyncWebServerRequest *request){
    int ix = entrada[1]; //dejo igual los factores
    entrada[1] = entrada[0];
    entrada[0] = ix;    
    for (int i=0;i<=1;i++) {      
        analogWrite(misLED[i],map(entrada[i],0,maxPasoIntensidad,0,4096)); 
    }
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

    tiempo = millis();

    if (pulsarBoton(0)) { Serial.println ("Lámpara derecha. Luminosidad: " + (String) entrada[0]);}
    if (pulsarBoton(1)) { Serial.println ("Lámpara izquierda. Luminosidad: " + (String) entrada[1]);}
    
    
} // fin del loop

// =========
// FUNCIONES
// =========

// ****************************************
// Función que detecta si se pulsa un botón
// ****************************************
boolean pulsarBoton(int cual) {

  boolean devuelve = false; // valor por defecto  

  // leer la entrada del botón
  estadoBoton[cual] = digitalRead(misBotones[cual]);

  // compare the buttonState to its previous state
  if (estadoBoton[cual] != estadoBotonAnt[cual] && pulsacion>1) {
    // if the state has changed, increment the counter
    if (estadoBoton[cual]  == HIGH) {
      devuelve = true;
      // Pone un valor a la entrada de forma manual      
      cualES = cual;
      if (cualES==0) { miLamp="DERECHA"; } else {miLamp="IZQUIERDA";}
      factor[cualES] = (entrada[cualES]==0) - (entrada[cualES]==10)+factor[cualES]*(entrada[cualES]>0 && (entrada[cualES]<10));         
      entrada[cualES]=entrada[cualES]+factor[cualES]; // De 0 a 10, 10 salta a cero      
      analogWrite(misLED[cualES],map(entrada[cualES],0,maxPasoIntensidad,0,4096));
      while(millis() < tiempo+tmpVariacionLuz){
        // espere [tmpVariacionLuz] milisegundos
      }      

    }
  }
  // guarda el estado previo del botón
  estadoBotonAnt[cual]  = estadoBoton[cual] ;
  pulsacion = pulsacion+1;
  return devuelve;  
}

// *****************************************************
// funcion que reemplaza  STATE por su valor en el HTML
// *****************************************************
String processor(const String& var){ 

  String facString[] = {"&lt;--","--&gt;"}; // cadenas si, manualmente disminuye o no la luminosidad

  if (var == "NIVEL") {
     Serial.println("Mi entrada vale "+ (String) entrada[cualES]);
     return (String) entrada[cualES];
  }
  
  if (var == "CUALES") {
     Serial.println("Mi lámpara es "+ (String) miLamp);
     return (String) miLamp;    
  }

  if (var == "FACTOR") {
     Serial.println("Mi factor vale "+ (String) factor[cualES]);
     return (String) facString[(factor[cualES]==1)]; // si es factor=-1, lo convierte a cero y disminuye. Si no es 1 y aumenta.
  }
 
  } // Fin de la función
  
