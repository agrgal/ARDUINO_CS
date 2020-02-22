

/*
 Ejemplo de uso de display LCD 16x2.  Usando la librería LiquidCrystal
 library compatible con el driver de Hitachi HD44780 driver 
*/

// incluimos la libreria LiquidCrystal
#include <LiquidCrystal.h>

// inicializamos la librería con los numeros pins del interfaz
// cada LCD puede llevar sus propios numeros
LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);


void setup() {
  // establecemos el numero de columnas y filas del display 
  lcd.begin(16, 2);
  // enviamos el mensaje a mostrar en el display
  lcd.print("Hola");
}

void loop() {
  // enviamos la posicion del cursor al display
  // (nota: la linea 1 es la segunda fila, empieza a contar en 0
  lcd.setCursor(0, 1);
  // mostramos el numero de segundos desde el inicio del programa
  lcd.print(millis()/100);
}
