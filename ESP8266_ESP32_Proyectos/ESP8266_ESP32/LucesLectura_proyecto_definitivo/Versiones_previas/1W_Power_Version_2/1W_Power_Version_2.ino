

#define miLed D5

long tiempo = 0;

// Control manual 
int entrada = 0;
int entradaAnt = 0;
int maxPasoIntensidad = 10; // Niveles de luz posibles.
int tmpVariacionLuz = 100; //tiempo en el que la luz permanece, en ms.

// Control página web


void setup() {
  pinMode(miLed, OUTPUT);  
  analogWriteRange(4096); // Rango de valores a la salida PWM
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
    
    tiempo = millis()/1000;   

    // A) Control Manual
    // a1) valor de la entrada
    entrada = analogRead(A0);
    entrada = map(entrada,15,1024,0,maxPasoIntensidad);
    if (entrada!=entradaAnt) { suavizado(entrada,entradaAnt); }
    Serial.println(String (tiempo) +" -- "+ String (entrada)+ " Vol: "+ String (entrada*3.3/100) );
    // a2) valor entrada antigua
    entradaAnt = entrada;

    // B) Control por página Web
    // https://randomnerdtutorials.com/esp8266-web-server-spiffs-nodemcu/
    // https://randomnerdtutorials.com/install-esp8266-filesystem-uploader-arduino-ide/

    
    
}

// =========
// FUNCIONES
// =========

// **********************************************
// funcion que suaviza los cambios bruscos de luz
// **********************************************
void suavizado(int a, int b) {
   if (a>b) {
      for (int i = b; i<=a; i++) {
          analogWrite(miLed,map(i,0,maxPasoIntensidad,0,4096));
          delay(tmpVariacionLuz);
      }
   } else if (b>a) {
      for (int i = b; i>=a; i--) {
          analogWrite(miLed,map(i,0,maxPasoIntensidad,0,4096));
          delay(tmpVariacionLuz);
      }    
   }   
}
