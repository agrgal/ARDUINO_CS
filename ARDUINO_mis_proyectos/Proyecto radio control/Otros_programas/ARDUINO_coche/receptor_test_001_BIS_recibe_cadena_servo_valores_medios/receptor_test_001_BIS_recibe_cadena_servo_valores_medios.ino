#include <ServoTimer2.h>
#include <VirtualWire_Config.h>
#include <VirtualWire.h>

#define pinVolante 3 //donde cojo el Volante

const int valorVolanteMIN = 750; // Inicialmente para una amplitud de unos 120º 750 - 2250 de rango.
const int valorVolanteMAX = 2250; // Cada grado, aumento en PLL de 12,5us
const int valorAmplitudEnGrados = 25; // grados a izquierda y derecha.

ServoTimer2 miVolante;

char Acelerador[5]; // una posicion ms de la necesaria
char Volante[5]; // una posicion ms de la necesaria

int valorAceleracion = 0;
int valorVolanteNuevo = 0; 
int valorVolanteAntiguo = 512; //valor intermedio de comienzo
int valorVolanteMedio = 1475; // valor intermedio
int valorVolanteIZQ = 0;
int valorVolanteDRCH = 0; // 
int marchaAD = true;
int Luces = false;

long recibidos = 0; // cuenta los recibidos
int i; //variable global de recepcion

void setup()
{
miVolante.attach(pinVolante); // cojo el servo en el pin 3.
valorVolanteMedio = (valorVolanteMIN+valorVolanteMAX) / 2;
valorVolanteIZQ=round(valorVolanteMedio-valorAmplitudEnGrados*12.5);
valorVolanteDRCH=round(valorVolanteMedio+valorAmplitudEnGrados*12.5);
Serial.begin(9600);
Serial.println("setup");
vw_setup(4000); // Bits per sec. A 2000 bits/sec
vw_rx_start();
// Start the receiver PLL running
// Por defecto, es el pin 11 el que recibe los datos por VirtualWire
}

void loop()
{
uint8_t buf[VW_MAX_MESSAGE_LEN];
uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) { // Non-blocking  
  
  if (recibidos<1000) {recibidos++; } else {recibidos=0;} //contador

  // Message with a good checksum received, dump HEX
  Serial.print("Got ");
  Serial.println(recibidos);
  
  // Comprueba codigo del mando...
  
  // *******************
  // Obtiene aceleracion
  // *******************
  // Recibiendo primer dato. De la posicin 2 a la 5 inclusive
  for (i = 2; i <=5; i++) {Acelerador[i-2] = char(buf[i]); }
  Acelerador[4]='\0'; // termina la cadena
  valorAceleracion = String(Acelerador).toInt();
  
  // *******************
  // Obtiene Volante
  // *******************
  Serial.print(" Volante: ");
  // Recibiendo primer dato. De la posicin 2 a la 5 inclusive
  for (i = 6; i <=9; i++) {Volante[i-6] = char(buf[i]); }
  Volante[4]='\0'; // termina la cadena
  valorVolanteNuevo = String(Volante).toInt(); // valorRecibido
 
  // Valor de volante a la salida
  valorVolanteNuevo=map(valorVolanteNuevo,0,1023,valorVolanteIZQ,valorVolanteDRCH); //valores en microsegundos experimentales
    // que acepta mi servo. Amplitud aproximada de 120º. A los 1475us estamos en la mitad.
  if ((valorVolanteNuevo > 1.01*valorVolanteAntiguo) || (valorVolanteNuevo < 0.99*valorVolanteAntiguo)) { // Si em muevo en una horquilla de 5%
      // miVolante.attach(pinVolante);
      miVolante.write(valorVolanteNuevo); //activa el servo a la nueva variable
      valorVolanteAntiguo=valorVolanteNuevo; //refresca la variable valorVolanteAntiguo
  } else {
     // desconecto Servo
     // miVolante.detach();
  }// y si no, no hace nada, deja el servo en esa posicion.
 
  // Obtiene si va marcha atrs o delante
  if (char(buf[10])=='1') {marchaAD=true;} else {marchaAD=false;}
 
  // Obtiene si tiene luces encendidas
  if (char(buf[11])=='1') {Luces=true;} else {Luces=false;}
  
  // Serial print
  Serial.print("A: ");
  Serial.print(valorAceleracion);
  Serial.print(" - V: ");
  Serial.print(valorVolanteNuevo);
  Serial.print(" - Vant: ");
  Serial.print(valorVolanteAntiguo);
  Serial.print(" - MAD: ");
  Serial.print(marchaAD);
  Serial.print(" - Luces: ");
  Serial.println(Luces);
  
  delay(5); // retraso en la recepcion del mensaje
  
  } // fin del if de recibir el mensaje
}
