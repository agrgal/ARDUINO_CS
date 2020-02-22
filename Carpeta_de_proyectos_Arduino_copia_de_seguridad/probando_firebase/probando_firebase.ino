
/*
 * Created by K. Suwatchai (Mobizt)
 * 
 * Email: k_suwatchai@hotmail.com
 * 
 * Github: https://github.com/mobizt
 * 
 * Copyright (c) 2019 mobizt
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
String estacion = "/A"; // Estacion que voy a controlar

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
       Serial.println("Estado del LED: "+String(estadoLuz)); //Manda al monitor serie ese estado.
       delay(1);
    }
  }

  digitalWrite(LED,estadoLuz);
  // digitalWrite(LED,rFB()); // Escribe el estado en un LED, PERO leyéndolo de la BD.
}



// Función que escribe un dato en Firebase.
// Además, actualiza el dato si tiene la misma ruta, no genera un error.
void wFB(int dato) { //Write Data in FB

  if (Firebase.setInt(bd,path+estacion,dato)) {
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
 
  if (Firebase.getInt(bd, path+estacion)) {

    if (bd.dataType() == "int") {
      Serial.println("Dato leído: "+ (String) bd.intData());
      return bd.intData();        
    }

  } else {
    Serial.println(bd.errorReason());    
  }
}
