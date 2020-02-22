// https://programarfacil.com/blog/arduino-blog/ds18b20-sensor-temperatura-arduino/

#include <OneWire.h> //Se importan las librerías
#include <DallasTemperature.h>

#define Pin D3 //Se declara el pin donde se conectará la DATA

OneWire ourWire(Pin); //Se establece el pin declarado como bus para la comunicación OneWire
DallasTemperature sensors(&ourWire); //Se instancia la librería DallasTemperature

void setup() {
delay(1000);
Serial.begin(9600);
sensors.begin(); //Se inician los sensores
}

void loop() {
sensors.requestTemperatures(); //Prepara el sensor para la lectura

// Del primer sensor, índice cero. Se pueden conectar más. 
Serial.print(sensors.getTempCByIndex(0)); //Se lee e imprime la temperatura en grados Celsius
Serial.println(" grados Centigrados");
// Serial.print(sensors.getTempFByIndex(0)); //Se lee e imprime la temperatura en grados Fahrenheit
// Serial.println(" grados Fahrenheit"); 

delay(300); //Se provoca un lapso de x segundos antes de la próxima lectura

}
