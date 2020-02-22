
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // dirección de la pantalla LCD, número de columnas, número de filas

void setup()
{
  lcd.init();                     // inicializa la pantalla LCD
  lcd.backlight();                // enciende la retroiluminación de la pantalla 
  lcd.setCursor(3,0);             // ponemos el cursor en la columna 3 de la fila 0
  lcd.print("Hola maker");        // escribimos en la pantalla Hola maker
  lcd.setCursor(0,1);             // ponemos el cursor en la columna 0 de la fila 1
  lcd.print("Fabrica Digital");   // escribimos Fabrica Digital
}


void loop()
{
}
