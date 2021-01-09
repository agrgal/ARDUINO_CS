/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp8266-nodemcu-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  Modificado por Aurelio Gallardo.
*/

#include <ESP8266WiFi.h>
#include <espnow.h> // Biblioteca ESPNOW

// Esta es la dirección del ue va A RECIBIR  los datos
uint8_t broadcastAddress[] = {0xA4, 0xCF, 0x12, 0xDF, 0x5A, 0x6B};

// Ejemplo de estructura de enviar datos
// debe coincidir la estructura del receptor
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} struct_message;

// Crear un mensaje estructurado llamado myData
struct_message myData;

// Función callback cuando datos se envían
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Status del último paquete enviado: ");
  if (sendStatus == 0){
    Serial.println("Entregado con éxito");
  }
  else{
    Serial.println("Entrega fallida");
  }
}
 
void setup() {
  // Inicializa monitor serie
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER); // como controlador
  esp_now_register_send_cb(OnDataSent); // define la función de callback
  
  // Registra pareja
  // The esp_now_add_peer accepts the following arguments, in this order:
  // mac address, role, wi-fi channel, key, and key length.
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
  // Set values to send
  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = random(1,20);
  myData.c = 1.2;
  myData.d = "Hello";
  myData.e = false;

  // Envía el mensaje via ESP-NOW
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  delay(500);
}
