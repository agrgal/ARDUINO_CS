//Sample using LiquidCrystal library
#include <LiquidCrystal.h>

/*******************************************************

This program will test the LCD panel and the buttons
Mark Bramwell, July 2010

********************************************************/

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int valor = 0;
int tiempo = 0;
String valorTexto = "";
void setup()
{
 lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
 lcd.print("Pulsa los botones"); // print a simple message
}

void loop() {
  // put your main code here, to run repeatedly:
  valor = analogRead(0);
  tiempo = int(millis() / 1000);
  valorTexto=String(valor);
  valorTexto.concat("   ");
  lcd.setCursor(0,1);
  lcd.print(valorTexto);
  lcd.setCursor(9,1);
  lcd.print(tiempo);
  delay(100);
}
