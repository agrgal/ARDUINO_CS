/*
Ejemplo de comunicación ESP-NOW por Dani No www.esploradores.com
***ESTE SKETCH CORRESPONDE AL PAR ESCLAVO***

El sketch permite la recepción de una comunicación vía ESP-NOW de dos datos enviados
por el par maestro.

- En la variable -potenciometro- se envía el dato de la lectura de la entrada
analógica del circuito maestro -valores entre 0 y 1023-, que se utilizarán
para regular la intensidad del LED conectado en el circuito esclavo.
- En la variable -tiempo- se envía el dato del milisegundo en el que el circuito maestro
realiza la comunicación con el circuito esclavo.

¡¡¡IMPORTANTE!!!

Para hacer funcionar la comunicación ESP-NOW si se tiene instalado en Arduino como gestor
de tarjetas el: esp8266 by ESP8266 Community versión 2.3.0 (Se puede comprobar buscándolo en:
Herramientas->Placa:"NodeMCU 1.0(ESP-12E Module)"->Gestor de tarjetas..., es necesario editar
el fichero: ~/Library/Arduino15/packages/esp8266/hardware/esp8266/2.1.0/platform.txt,
buscar "compiler.c.elf.libs", y añadir al final de la línea "-lespnow".
*/

#include <ESP8266WiFi.h>
extern "C" {
#include <espnow.h>
}

//***ESTRUCTURA DE LOS DATOS TRANSMITIDOS MAESTRO/ESCLAVO***//
//Se de establecer IGUAL en el par maestro
struct ESTRUCTURA_DATOS {
uint16_t potenciometro = 0;
uint32_t tiempo = 0;
};

//***PIN de conexión del LED a regular con el potenciometro del ESP MAESTRO***//
int PinLED = 5; //Pin D1

void setup() {

//***INICIALIZACIÓN DEL PUERTO SERIE***//
Serial.begin(115200); Serial.println();

//***INICIALIZACIÓN DEL PROTOCOLO ESP-NOW***//
if (esp_now_init()!=0) {
Serial.println("Protocolo ESP-NOW no inicializado...");
ESP.restart();
delay(1);
}

//***DATOS DE LAS MAC (Access Point y Station) del ESP***//
Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());

//***DECLARACIÓN DEL PAPEL DEL DISPOSITIVO ESP EN LA COMUNICACIÓN***//
//0=OCIOSO, 1=MAESTRO, 2=ESCLAVO y 3=MAESTRO+ESCLAVO
esp_now_set_self_role(2);

//***DECLARACIÓN del PinLED como SALIDA***//
pinMode(PinLED, OUTPUT);
}

void loop() {

//***RECEPCIÓN DE LA COMUNICACIÓN ESP-NOW***//
esp_now_register_recv_cb([](uint8_t *mac, uint8_t *data, uint8_t len) {

char MACmaestro[6];
sprintf(MACmaestro, "%02X:%02X:%02X:%02X:%02X:%02X",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
Serial.print("Recepcion desde ESP MAC: "); Serial.print(MACmaestro);

ESTRUCTURA_DATOS ED;
memcpy(&ED, data, sizeof(ED));

Serial.print(". Dato potenciometro: "); Serial.print(ED.potenciometro);
Serial.print(". Dato tiempo: "); Serial.println(ED.tiempo);

analogWrite(PinLED,ED.potenciometro);
});

}
