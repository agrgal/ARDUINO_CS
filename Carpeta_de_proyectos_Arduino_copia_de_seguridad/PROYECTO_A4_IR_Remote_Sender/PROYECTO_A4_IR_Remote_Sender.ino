/*
 * Aurelio gallardo Rodríguez
 * proyecto mando a distancia con un solo botón y ATtiny 85
 * 27-Diciembre-2020
 */
#include <IRremote.h>

IRsend irsend;

int i;
int LED=2;
int pulsador=0;
int estado=0;
int estadoAnterior=0;
long tiempo2 =0;
long tiempo=0;
uint32_t numEnviado=0xC40BF; // C40BF

void setup()
{
 pinMode(LED,OUTPUT);
 pinMode(pulsador,INPUT);
}

void loop() {

tiempo=millis();

estado=digitalRead(pulsador);

if (estado==1 && estadoAnterior==0) { //flanco de subida
  parpadeo();
  envio(numEnviado);
  envio(numEnviado); 
  parpadeo();
}

estadoAnterior=estado;

digitalWrite(LED,((tiempo/10000)%8==0));

}

// Función parpadeo
void parpadeo() {
  tiempo2=millis()+500;
  while(millis()<tiempo2) {
    digitalWrite(LED,(millis()/125)%2);
  }
}

// Función envío dato
void envio(uint32_t num) {
   irsend.sendNEC(num, 32); // NEC code
   tiempo2=millis()+70; // aumento el tiempo de envío en trescientos
   while (millis()<tiempo2) {
     tiempo2=tiempo2+0; // do something useless;
   }   
}
