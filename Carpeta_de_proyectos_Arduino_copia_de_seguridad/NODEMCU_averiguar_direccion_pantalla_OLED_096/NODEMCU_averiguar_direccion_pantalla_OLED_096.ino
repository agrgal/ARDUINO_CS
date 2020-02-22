#include <Wire.h>

void setup()
{
  Wire.begin();

  Serial.begin(9600);
  while (!Serial);             // Espera al monitor serie
  Serial.println("\nEscaner I2C");
}

void loop()
{
  byte error, address;
  int nDevices;

  Serial.println("Escaneando...");

  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // El escaner i2c utiliza el valor devuelto por la instrucción 
    // Write.endTransmisstion para reconocer la dirección a la que 
    // está conectado cada dispositivo.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("Dispositivo I2C encontrado en la direccion 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Error desconocido en la direccion 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("Ningun dispositivo I2C encontrado\n");
  else
    Serial.println("Hecho\n");

  delay(5000);           // espera 5 segundos para el siguiente escaneo
}
