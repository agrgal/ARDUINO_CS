#include <VirtualWire_Config.h>
#include <VirtualWire.h>


char Acelerador[5]; // una posicion ms de la necesaria
char Volante[5]; // una posicion ms de la necesaria

int valorAceleracion = 0;
int valorVolante = 0;
int marchaAD = true;
int Luces = false;

int pinVolante = 6; // pin en el que esta el servo

long recibidos = 0; // cuenta los recibidos
int i; //variable global de recepcion

void setup()
{
pinMode(pinVolante,OUTPUT);  
Serial.begin(9600);
Serial.println("setup");
vw_setup(4000); // Bits per sec. Lo cambio de 2000 a 4000
vw_rx_start();
// Start the receiver PLL running
}

void loop()
{
uint8_t buf[VW_MAX_MESSAGE_LEN];
uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) { // Non-blocking  
  recibidos++;

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
  valorVolante = String(Volante).toInt();
 
  // Valor de volante a la salida
  valorVolante=map(valorVolante,0,1023,0,255);
  analogWrite(pinVolante,valorVolante);
 
  // Obtiene si va marcha atrs o delante
  if (char(buf[10])=='1') {marchaAD=true;} else {marchaAD=false;}
 
  // Obtiene si tiene luces encendidas
  if (char(buf[11])=='1') {Luces=true;} else {Luces=false;}
  
  // Serial print
  Serial.print("A: ");
  Serial.print(valorAceleracion);
  Serial.print(" - V: ");
  Serial.print(valorVolante);
  Serial.print(" - MAD: ");
  Serial.print(marchaAD);
  Serial.print(" - Luces: ");
  Serial.println(Luces);
  
  delay(50); // retraso en la recepcion del mensaje
  
  } // fin del if de recibir el mensaje
}
