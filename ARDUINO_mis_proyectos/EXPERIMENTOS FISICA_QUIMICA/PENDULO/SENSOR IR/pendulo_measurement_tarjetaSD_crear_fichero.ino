#include <SPI.h>
#include <SD.h>

#define led 8
#define ir 7

long tiempoCorte = 0;
int contar = 0;
long tiempo;

File miFichero;

const int chipSelect = 4;

void setup() {
  // define el pin led como salida y el ir como entrada DIGITAL
  pinMode(led, OUTPUT);
  pinMode(ir, INPUT);
  // Serial
  Serial.begin(9600); // Pone en marcha la lectura en serie.

  
  // ver si la tarjeta puede 
  if (!SD.begin(chipSelect)) {
    Serial.println("No tengo tarjeta!!");
    // don't do anything more:
    return;
  }
  Serial.println("Tarjeta inicializada");
  
}

void loop() {

  // cuenta del tiempo;
  tiempo = micros();

  // Inicializa la cadena
  String dataEscribir = "";
  
  // parpadea led en pin 8
  digitalWrite(led,!(digitalRead(ir)));

  // Leo el estado
  // estado = digitalRead(ir);

  // Comparo estado, con el antiguo
  if (pulseIn(ir,LOW)) {
     contar+=1;
     // Serial.println(contar);
     delay(200);

       if (contar%2==0 && contar>0) {
         
          dataEscribir+=String(abs(tiempo-tiempoCorte));
          // abro el fichero
          miFichero = SD.open("nuevo.csv", FILE_WRITE); // Importante: nombre con 8 caracteres, tres de extensión.
          // escribo 
          miFichero.println(dataEscribir);
          // Cierro el fichero
          miFichero.close();

          Serial.println(dataEscribir);
           
          // Para el siguiente ciclo
          contar=0;
          tiempoCorte = tiempo;
       }
     
  }

  // al final 

  // estadoAntiguo = estado;


 
}
