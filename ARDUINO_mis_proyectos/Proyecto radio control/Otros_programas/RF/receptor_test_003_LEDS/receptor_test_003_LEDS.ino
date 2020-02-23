#include <VirtualWire_Config.h>
#include <VirtualWire.h>

#define pinVolante 3 //donde cojo el Volante
#define pinAceleracion 5 //donde cojo la aceleracio
#define pinMAD 6 // Marcha atras a delante
#define pinLuces 7 // Luces on off

char Acelerador[5]; // una posicion mas de la necesaria
char Volante[5]; // una posicion mas de la necesaria

int valorAceleracion = 0;
int valorVolanteNuevo = 0; 
int marchaAD = true;
int Luces = false;

long recibidos = 0; // cuenta los recibidos
int i; //variable global de recepcion

void setup()
{
Serial.begin(9600);
Serial.println("setup");

pinMode(pinVolante,OUTPUT);
pinMode(pinAceleracion,OUTPUT);
pinMode(pinMAD,OUTPUT);
pinMode(pinLuces,OUTPUT);

vw_setup(2000); // Bits per sec. A 2000 bits/sec
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
  valorAceleracion=map(valorAceleracion,0,1023,0,255);
  analogWrite(pinAceleracion,valorAceleracion);
  
  
  // *******************
  // Obtiene Volante
  // *******************
  Serial.print(" Volante: ");
  // Recibiendo primer dato. De la posicin 2 a la 5 inclusive
  for (i = 6; i <=9; i++) {Volante[i-6] = char(buf[i]); }
  Volante[4]='\0'; // termina la cadena
  valorVolanteNuevo = String(Volante).toInt(); // valorRecibido
  valorVolanteNuevo=map(valorVolanteNuevo,0,1023,0,255);
  analogWrite(pinVolante,valorVolanteNuevo);

  // Obtiene si va marcha atrs o delante
  if (char(buf[10])=='1') {marchaAD=true;} else {marchaAD=false;}
  digitalWrite(pinMAD,marchaAD);
 
  // Obtiene si tiene luces encendidas
  if (char(buf[11])=='1') {Luces=true;} else {Luces=false;}
  digitalWrite(pinLuces,Luces);
  
  // Serial print
  Serial.print("A: ");
  Serial.print(valorAceleracion);
  Serial.print(" - V: ");
  Serial.print(valorVolanteNuevo);
  // Serial.print(" - Vant: ");
  // Serial.print(valorVolanteAntiguo);
  Serial.print(" - MAD: ");
  Serial.print(marchaAD);
  Serial.print(" - Luces: ");
  Serial.println(Luces);
  
  delay(5); // retraso en la recepcion del mensaje
  
  } // fin del if de recibir el mensaje
}
