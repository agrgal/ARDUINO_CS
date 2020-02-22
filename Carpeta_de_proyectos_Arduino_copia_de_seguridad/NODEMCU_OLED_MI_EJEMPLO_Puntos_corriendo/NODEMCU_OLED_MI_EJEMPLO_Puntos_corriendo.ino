/*
 ESP8266 - Ejemplo de uso de pantalla OLED 128x64 monocroma por Dani No www.esploradores.com

 Este ejemplo utiliza el núcleo de la librería gráfica de Adafruit GFX y 
 la librería SSD1306 específica para pantallas monocromas 128x64 y 128x32.
 Estas librerías deben estar instaladas para su funcionamiento.
 Para instalarlas se debe seleccionar:
   -Programa -> Incluir librería -> Gestionar Librerías -> Adafruit GFX Library
   -Programa -> Incluir librería -> Gestionar Librerías -> Adafruit SSD1306

 Cuando se instala la librería SSD1306 está configurada por defecto para pantallas 128x32.
 Al utilizarla para una pantalla 128x64 es preciso reconfigurarla.
 Para hacerlo debe abrir el fichero Adafruit_SSD1306.h con un editor y 
 descomentar la línea 69 (// #define SSD1306_128_64  --->  #define SSD1306_128_64) y 
 comentar la línea 70 (#define SSD1306_128_32  --->  //#define SSD1306_128_32).
 Así mismo se debe verificar en la línea 51 que la dirección de I2C de la pantalla es correcta 
 (#define SSD1306_I2C_ADDRESS   0x3C  // 128x64 0x3D (default) or 0x3C (if SA0 is grounded))

Por último, también se debe verificar que en la línea 108 de este sketch
la dirección I2C de la pantalla también es correcta.
 */

 /* 
  *  https://www.diarioelectronicohoy.com/blog/pantalla-oled-0-96  (descripción de las órdenes)
  *  
  */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>      //Nucleo de la librería gráfica.
#include <Adafruit_SSD1306.h>  //Librería para pantallas OLED monocromas de 128x64 y 128x32
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeSerifBold9pt7b.h>  // Biblioteca de fuentes: https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts


// =============================================================================================
// IMPORTANTE: definición de pantalla, y RESET que funciona, sin tocar librería adafruit_SSD1306
// =============================================================================================
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     0 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// =============================================================================================

// #define OLED_RESET 0 
// Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

int x = 0;
int y = 0;

static const unsigned char PROGMEM logo[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

void setup()   {                
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // //Inicializa pantalla con en la dirección 0x3D para la conexión I2C.
}

void loop() {
  
  display.fillScreen(0);         //Limpiamos la pantalla

  // PUNTO Corriendo --> variación con círculos
  // display.drawPixel(x,y,1);   //Dibujo un pixel en pantalla
  // display.drawCircle(x,y,10,1);  // O bien un círculo
  // display.fillCircle(x,y,10,1);  // relleno

  // Caracteres
  // display.setFont();                //Fuente por defecto -si no la hemos cambiado no es necesario seleccionarla
  // display.setFont(&FreeSansBoldOblique12pt7b); // Especificada antes al cargar el programa
  // display.drawChar(x,y,'a',1,0,4);  //Tamaños de 1 a 8. Los tamaños típicos son 1, 2 o 4  
                                    // drawChar (uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t tamaño)

  // Un texto en pantalla
  // display.setFont(&FreeMonoBoldOblique12pt7b);     // Si no se quiere cambiar la fuente por defecto, ya sabes...
  /*
  display.setFont(&FreeSerifBold9pt7b);
  display.setTextSize(2);                          // Tamaño
  display.setTextColor(1,0);                       // Color (1,0) ó 0,1 invertido
  display.setCursor(x,y);                         // Posiciones x e y
  display.println("Hola, soy Aurelio"); */

  // Logo
  display.drawBitmap(0,0,logo,16,16,1);         // drawBitmap (int16_t x, int16_t y, uint8_t * mapa de bits, int16_t w, int16_t h, uint16_t color)
                                                 // https://www.brainy-bits.com/create-arduino-array-from-pictures/
  x=x+1;
  if (x==128) {y=y+5; x=0;}    // Si se acaba en x, entonces vuelve a la y=0
  if (y==64) {y=0;}            // Si se acaba la y, empieza
  // display.drawPixel(0,0,1);      //Esquina superior izquierda
  display.display();             //Refrescamos la pantalla para visualizarlos
  delay(50);  
 }
