/**
 * Buscador_I2C
 * Busca dispositivos I2C.
 *
 * Este ejemplo es de dominio público.
 *
 * Modificado el 15 de febrero de 2016
 * por Pablo Bacho para Fábrica Digital
 */

#include <Wire.h>
#include <FabricaDigital_I2C.h>

FabricaDigital_I2C i2c;

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  uint8_t dispositivos, direccion;
  
  dispositivos = i2c.obtenerNumeroDeDispositivos();
  
  if(dispositivos > 0) {
    Serial.print("Dispositivos en las direcciones: ");
    for(uint8_t i = 0; i < dispositivos; i++) {
      direccion = i2c.obtenerDireccionDispositivo(i);
      Serial.print("0x");
      if(direccion < 0x10) {
        Serial.print("0");
      }
      Serial.print(direccion, HEX);
      Serial.print(" ");
    }
    Serial.println("");
  } else {
    Serial.println("No se han encontrado dispositivos I2C");
  }
  
  delay(1000);
}