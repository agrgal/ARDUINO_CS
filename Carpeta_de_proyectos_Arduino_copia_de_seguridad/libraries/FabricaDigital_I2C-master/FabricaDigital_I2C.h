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

#ifndef _FabricaDigital_I2C_H_
#define _FabricaDigital_I2C_H_

#include "Arduino.h"
#include "Wire.h"

class FabricaDigital_I2C {
public:
  uint8_t obtenerNumeroDeDispositivos();
  uint8_t obtenerDireccionDispositivo(uint8_t id);
};

#endif /* _FabricaDigital_I2C_H_ */
