
/*
 * Created by Aurelio Gallardo Rodríguez 
 * Based on: K. Suwatchai (Mobizt)
 * 
 * Email: aurelio@seritium.es
 * 
 * CENTRAL. Lectura
 * 
 * Julio - 2019
 *
*/


//FirebaseESP8266.h must be included before ESP8266WiFi.h
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "botonpanicoseritium.firebaseio.com"
#define FIREBASE_AUTH "Y0QVBot29hCVGZJQbpVUMr3IKngc7GacE2355bdy"
#define WIFI_SSID "JAZZTEL_shny"
#define WIFI_PASSWORD "3z7s5tvbtu4s"

//Define FirebaseESP8266 data object
FirebaseData bd;

String path = "/Estaciones"; // path a FireBase 
char *estaciones[]= {"A","B","C","D","E"}; // Estaciones que voy a controlar

int i=1; // contador general

int activo=0; // Alarma 

#define LED 5 // D1(gpio5)

void setup()
{

  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  
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
    
}

// Bucle principal
void loop() {

  activo=0; // reinicializo la variable
  
  for(i=0;i<=4;i++) {
    delay(1);
    activo=activo+rFB("/"+(String) estaciones[i]);    
  }
  digitalWrite(LED,(activo>=1)); // activo el LED si es mayor o igual a 1.
  

}



// Función de lectura
int rFB(String estacion){// lee el dato de la entrada correspondiente

  int valor=0;
 
  if (Firebase.getInt(bd, path+estacion)) {
    if (bd.dataType() == "int") {
      Serial.println("Dato leído: "+ path+estacion +" --> " + (String) bd.intData());
      valor = bd.intData(); // retorna el valor       
    }
  } else {
    // Si no existe el dato de la estación, salta el error
    // Pero el error se muestra en pantalla (bd.errorReason()) Y REINICIALIZA LA UNIDAD, lo cual no quiero.
    Serial.println("Estoy dando un error... ojo");
    // Serial.println(bd.errorReason());   
  }

  return valor;
  
}
