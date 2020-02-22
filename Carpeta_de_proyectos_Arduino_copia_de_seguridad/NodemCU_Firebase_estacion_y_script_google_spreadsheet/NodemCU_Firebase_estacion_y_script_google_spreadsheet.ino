
/*
 * Created by Aurelio Gallardo Rodríguez 
 * Based on: K. Suwatchai (Mobizt)
 * 
 * Email: aurelio@seritium.es
 * 
 * ESTACION. Escritura. Envío de información a Firebase y Google Spreadsheet
 * 
 * Julio - 2019
 *
*/

//FirebaseESP8266.h must be included before ESP8266WiFi.h
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

#define FIREBASE_HOST "botonpanicoseritium.firebaseio.com"
#define FIREBASE_AUTH "Y0QVBot29hCVGZJQbpVUMr3IKngc7GacE2355bdy"
#define WIFI_SSID "JAZZTEL_shny"
#define WIFI_PASSWORD "3z7s5tvbtu4s"

const char* host = "script.google.com"; // Este es el host de los scripts de google.
const int httpsPort = 443;
// Huella digital del script de Google:
// D4:9E:40:F4:53:7A:04:93:38:F7:6B:4B:DC:70:02:A9:03:98:C2:DE
const char* fingerprint = "D4 9E 40 F4 53 7A 04 93 38 F7 6B 4B DC 70 02 A9 03 98 C2 DE";
// const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";

String googleSheetsID = "AKfycbz73SQeEiIrWJ3ZszPZnJmlrX45KwwSOPUI33-mr-yFw-D7kBMl"; // El que me da al implementar una aplicación web en el script.


// objeto comunicación con Google Spreadsheet
WiFiClientSecure cliente;

//Define FirebaseESP8266 data object
FirebaseData bd;

DynamicJsonDocument doc(1024);


String path = "/Estaciones"; // path a FireBase 
String estacion = "A"; // Estacion que voy a controlar

int i=1; // contador general

int activo=0;

#define LED 5 // D1(gpio5)
#define BUTTON 4 //D2(gpio4)

int estado=0;
int estadoAnterior=0;
int estadoLuz=0;

void setup()
{

  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);

  // conectando a la WIFI
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando a la WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Conectado con IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Conectando a la bd real Time Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(bd, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(bd, "unlimited");

  wFB(0); // Escribe un cero en la estación al arrancar
    
}

// Bucle principal
void loop() {

  estado=digitalRead(BUTTON); 
  if (estado!=estadoAnterior){
    // el estadoAnterior ahora es el estado
    estadoAnterior=estado;
    // si estado es alto, se pasa de bajo a alto
    if (estado) {
       estadoLuz=!estadoLuz; // El estado de la luz cambia
       // sendDataToGoogleSheets(estacion,estadoLuz); // envío a googleSheet esa información
       wFB(estadoLuz); // en vez de enviarlo a Google Spreadsheet, se lo envío a Real Time BD Firebase
       digitalWrite(LED,estadoLuz);
       Serial.println("Estado del LED: "+String(estadoLuz)); //Manda al monitor serie ese estado.
       delay(10);
       sendDataToGoogleSheets(estacion,estadoLuz); // envío a googleSheet esa información
    }
  }

  digitalWrite(LED,estadoLuz);
  // digitalWrite(LED,rFB()); // Escribe el estado en un LED, PERO leyéndolo de la BD.
}



// Función que escribe un dato en Firebase.
// Además, actualiza el dato si tiene la misma ruta, no genera un error.
void wFB(int dato) { //Write Data in FB

  if (Firebase.setInt(bd,path+"/"+estacion,dato)) {
      Serial.println("PASSED");
      Serial.println("PATH: " + bd.dataPath());
      Serial.println("TYPE: " + bd.dataType());
      Serial.println("ETag: " + bd.ETag());
      Serial.print("VALUE: ");
      if (bd.dataType() == "int")
        Serial.println(bd.intData());
      else if (bd.dataType() == "float")
        Serial.println(bd.floatData(), 5);
      else if (bd.dataType() == "double")
        printf("%.9lf\n", bd.doubleData());
      else if (bd.dataType() == "boolean")
        Serial.println(bd.boolData() == 1 ? "true" : "false");
      else if (bd.dataType() == "string")
        Serial.println(bd.stringData());
      else if (bd.dataType() == "json")
        Serial.println(bd.jsonData());
      Serial.println("------------------------------------");
      Serial.println();
  } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + bd.errorReason());
      Serial.println("------------------------------------");
      Serial.println();  
  }  
}

// Función de lectura
int rFB(){// lee el dato de la entrada correspondiente
 
  if (Firebase.getInt(bd, path+"/"+estacion)) {

    if (bd.dataType() == "int") {
      Serial.println("Dato leído: "+ (String) bd.intData());
      return bd.intData();        
    }

  } else {
    Serial.println(bd.errorReason());    
  }
}

/* Función de conexión. Importante la instrucción  cliente.setInsecure(); para conectar de forma anónima
 */

void sendDataToGoogleSheets(String nombreEstacion, int Activacion) {

  String cadena="";
  
  Serial.print("Conectando a: ");
  Serial.println(host);

  // cliente.setInsecure();
  
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

  String url = "/macros/s/" + googleSheetsID + "/exec?estacion=" + nombreEstacion + "&activo=" + (String) Activacion;
  
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

  String exito = obj[String("exito")];
  String comentario = obj[String("comentario")];   

  
  Serial.println("Respuesta extraída en variables deserializando la cadena en un objeto JSON");
  Serial.println("Ha tenido éxito: "+exito);
  Serial.println("Envía el comentario: "+comentario);
  Serial.println("==============================");
  Serial.println();

  delay(500); 
  
  cliente.stop(); //cierra la conexión   
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
