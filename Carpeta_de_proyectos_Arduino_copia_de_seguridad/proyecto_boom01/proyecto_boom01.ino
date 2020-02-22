
// Librerías necesarias para la pantalla
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Activación de la pantalla LCD con I2C
LiquidCrystal_I2C lcd(0x27,16,2);  // dirección de la pantalla LCD, número de columnas, número de filas

// Variables
unsigned long tiempo=0; // temporizador
int i=0; // contador proposito general
boolean juego=0; // estado del juego 0--> coloca solución 1--> jugando...

int solucionElegida=0;

// *********
// Funciones
// *********



// *********
// SetUP
// *********
void setup()
{
  lcd.init();                     // inicializa la pantalla LCD
  lcd.backlight();                // enciende la retroiluminación de la pantalla 
  lcd.setCursor(0,0);             // ponemos el cursor en la columna 3 de la fila 0
  lcd.print("Empezamos el juego..."); // escribimos
  lcd.clear(); // limpia la pantalla

  // pinModes
  pinMode(13,INPUT); // pin de inicio del juego
}

// *********
// Principal
// *********
void loop()
{

  tiempo = millis(); // tiempo transcurrido desde el inicio del programa
  
  if (digitalRead(13)) { juego=1; } // Simplemente pulsando el boton de inicio del juego, se activa

  if (juego) { // Comprueba el estado del juego
    lcd.setCursor(0,0);             
    lcd.print("Juego en marcha!"); 
    lcd.setCursor(0,1);
    lcd.print("               "); // Borra la linea
    lcd.setCursor(0,1);
    lcd.print("Solucion en " + (String) (solucionElegida+1));
     
  } else { // Condicional sobre la variable juego
    lcd.setCursor(0,0);             
    lcd.print("Elige una solucion...");  
    lcd.setCursor(0,1);
    lcd.print("Solucion n:");
    solucionElegida = map(analogRead(A0),0,1024,0,4);
    lcd.setCursor(13,1);
    lcd.print(solucionElegida+1);
  } // Fin del IF sobre la variable juego
  
} // Fin del Loop
