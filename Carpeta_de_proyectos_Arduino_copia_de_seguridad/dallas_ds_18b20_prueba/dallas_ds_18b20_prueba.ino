// ***********************
// Bibliotecas y variables
// ***********************
#include <OneWire.h> //Se importan las librerías
#include <DallasTemperature.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

double temperatura = 0.0;
long contador = 0;
int Pin = 2; //Se declara el pin donde se conectará la DATA

OneWire ourWire(Pin); //Se establece el pin declarado como bus para la comunicación OneWire
DallasTemperature sensors(&ourWire); //Se instancia la librería DallasTemperature

LiquidCrystal_I2C lcd(0x27,16,2);  // establece la dirección del LCD a 0x27, para una pantalla de 16 x 2 caracteres. 
// Son posibles las direcciones 0x3f ó 0x20, por si algún modelo está así direccionado

byte grado[] = {
  B00111,
  B00101,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

// *****
// SETUP
// *****

void setup() {
  Serial.begin(9600);
  sensors.begin(); //Se inician los sensores
  lcd.init(); // Se inicializa el LCD
  lcd.backlight(); // Enciendo la retroiluminación
  lcd.createChar(0, grado);
}

// ******************
// Programa principal
// ******************

void loop() {

  contador = contador + 1;
  sensors.requestTemperatures(); //Prepara el sensor para la lectura
  temperatura = sensors.getTempCByIndex(0); // Obtiene la temperatura en grados centígrados
  
  Serial.println((String) contador + " " + (String) temperatura); //Se imprime la temperatura en grados Celsius en el monitor en serie
   
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Medida de temp:");
  lcd.setCursor(0,1);
  lcd.print((String) temperatura);
  lcd.write(0);
  lcd.print("C");
  
  
  delay(300); //Se provoca un lapso de 0,3 segundo antes de la próxima lectura
}
