/*
El programa primero detecta la diferencia entre la señal con y sin LDR, calculando un valor de corte como la semisuma. 
Después . si la masa corta dos veces por el lado izquierdo de su recorrido el vector, al volver, cuenta el espaciado entre
el primer y tercer valle de los números, averiguando el período. Podemos comprobar con la fórmula: T = 2*Pi*RAIZ(longitud/g) 
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>      //Nucleo de la librería gráfica.
#include <Adafruit_SSD1306.h>  //Librería para pantallas OLED monocromas de 128x64 y 128x32
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeSerifBold9pt7b.h>  // Biblioteca de fuentes: https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
 
#define pinBoton D5
#define pinLASER D8

// =============================================================================================
// IMPORTANTE: definición de pantalla, y RESET que funciona, sin tocar librería adafruit_SSD1306
// =============================================================================================
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     0 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// =============================================================================================

long tiempo = 0;
long tiempo1 = 0;
long diferencia = 0;

long tiempocorte1=0;
long tiempocorte2=0;

float voltage = 0;
float voltageAnterior = 0;
float valorCorte = 0;

int contar = 0;

boolean estado = false;

boolean estadoBoton = LOW;

// the setup routine runs once when you press reset:
void setup() {

  pinMode(pinBoton,INPUT);
  pinMode(pinLASER,OUTPUT);
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);

  digitalWrite(pinLASER,LOW); // El laser empieza, lógicamnete, apagado  
}

// the loop routine runs over and over again forever:
void loop() {

  if (valorCorte<=0) {vcorte();} // Si el valor de corte no está establecido, lo establece

  // Encender el laser o no
  int estadoBotonAhora = digitalRead(pinBoton);

  if (estadoBotonAhora==HIGH && estadoBoton==LOW) {
      estado = estado xor 1;
      
      delay(50);
  }

  estadoBoton = estadoBotonAhora;
  digitalWrite(pinLASER,estado);
  // Fin de encender el laser
  
  tiempo = millis();
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue;
  String voltaje = String(voltage);
  voltaje.replace(".",",");

  // Serial.println(String(tiempo)+" ; "+voltaje+" ; corte: "+String(valorCorte));

  // Reconocimiento del paso
  if (estado==HIGH && voltage<valorCorte && voltageAnterior>=valorCorte) { // Si se sucede un paso hacia abajo por el valor de Corte      
     contar = contar + 1;
     if (contar==1) { // Primer paso 
        tiempocorte1 = tiempo;
        Serial.println("=========================================");
        Serial.println ("Valor de corte: "+String(valorCorte));
        Serial.println("Primer paso en el valor de tiempo (ms): "+String(float(tiempocorte1)));
     } else if (contar==3 && tiempocorte1>0) { // Tercer paso
        diferencia = tiempo-tiempocorte1;
        Serial.println("T= "+String( float(diferencia) /1000)+"s; L="+longitud(diferencia)+" cm");
        Serial.println("=========================================");
        // Refresco tiempo
        contar = 0; 
        tiempocorte1=0;
     } // Fin de la condición que el conteo sea par
  } 

  voltageAnterior = voltage;
  // delay(1);  
  // Serial.println(voltage);
}

// Funcion que devuelve la longitud
String longitud(long periodo) {
  // perido viene en milisegundos, convertir a segundos.
  float calculo = 100*9.8*pow(((periodo/1000.0) / (2.0 * 3.141596254)),2.0) ;
  // devuelve la longitud del péndulo en centímetros
  String cadena = String(calculo);
  cadena.replace(".",",");
  return cadena;
}

// Funcion que reconoce el valor de corte
void vcorte() {
    // Al principio establece el valor de corte. Una vez establecido, esta rutina no se vuelve a ejecutar
      digitalWrite(pinLASER,LOW); 
      valorCorte = analogRead(A0);
      delay(10); 
      digitalWrite(pinLASER,HIGH);
      delay(10);
      valorCorte = (valorCorte +analogRead(A0))/2; // La semisuma
      digitalWrite(pinLASER,estado);     
}
