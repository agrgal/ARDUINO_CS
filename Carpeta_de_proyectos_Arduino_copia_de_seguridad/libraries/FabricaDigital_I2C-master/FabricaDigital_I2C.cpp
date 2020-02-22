/**
 * Librería de utilidades I2C para Arduino
 * Copyright 2016 - Fábrica Digital (fabricadigital.org)
 *
 * Publicado bajo licencia CC-BY-SA 4.0
 * Creative Commons: Reconocimiento - Compartir Igual 4.0 Internacional
 * http://creativecommons.org/licenses/by-sa/4.0/deed.es_ES
 *
 * Modificado el 15 de febrero de 2016
 * por Pablo Bacho para Fábrica Digital
 */

#include "FabricaDigital_I2C.h"

uint8_t FabricaDigital_I2C::obtenerNumeroDeDispositivos() {
  uint8_t nDispositivos = 0;

  for(uint8_t direccion = 1; direccion < 127; direccion++) {
    Wire.beginTransmission(direccion);
    if(Wire.endTransmission() == 0) {
      nDispositivos++;
    }
  }

  return nDispositivos;
}

uint8_t FabricaDigital_I2C::obtenerDireccionDispositivo(uint8_t id) {
  uint8_t nDispositivos = 0;

  for(uint8_t direccion = 1; direccion < 127; direccion++) {
    Wire.beginTransmission(direccion);
    if(Wire.endTransmission() == 0) {
      if(id == 0) { // Cuando llega a 0 es el dispositivo que buscamos
        return direccion;
      }
      id--; // Usa id de contador
    }
  }

  return 0;
}
