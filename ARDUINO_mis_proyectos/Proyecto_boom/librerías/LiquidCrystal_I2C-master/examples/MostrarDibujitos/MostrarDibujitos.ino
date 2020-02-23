
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27,16,2);  // dirección de la pantalla LCD, número de columnas, número de filas

byte coco[8] = {                  
  0b00000,
  0b01110,
  0b11111,
  0b10101,   
  0b11111, 
  0b11111,   
  0b11111,   
  0b10101   
};  // variable array que incluiye el caracter personalizado  

void setup() {    

  lcd.init();          // inicializa la pantalla LCD
  lcd.backlight();     // retroiluminación de la pantalla  
  lcd.createChar(0, coco);  //guardamos a coco en la posición de memoria 0
}  

void loop() {   
  lcd.setCursor(3,0);  //colocamos el cursor en la columna 3 de la fila 0
  lcd.write(byte(0));  //mostramos a coco
}


