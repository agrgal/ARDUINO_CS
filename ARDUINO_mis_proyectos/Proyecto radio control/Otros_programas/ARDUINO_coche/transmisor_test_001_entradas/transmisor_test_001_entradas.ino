#include <VirtualWire.h>

const int pinData = 12; // cambio el puerto de entrada de datos del mando.
const int pinCheck = 13; // pin de comprobar que est funcionando

const int pinAcelerador = A0;
const int pinVolante = A1;
const int pinMarchaAdelante = 7;
const int pinLuces = 6;

int Acelerador = 0; // valor de 0 a 1023 de cmo acelera.
int Volante = 0; // valor del sensor de giro
int MarchaAdelante = true; // valor de si anda hacia adelante o hacia atrs
int Luces = false; // si estn o no las luces conectadas.

int contador = 0;
int salidaCheck=0;

String SSend; // Codigo a enviar
char msg[13]; // cadena de caracteres

void setup()
{
pinMode(pinMarchaAdelante,INPUT); // de entrada
pinMode(pinLuces,INPUT); // de entrada
pinMode(pinCheck,OUTPUT); // de entrada
vw_setup(2000); // Bits per sec. Lo cambio de 2000
vw_set_tx_pin(pinData);
Serial.begin(9600); // empiezo comunicacin serie
// Por defecto el pin de transmisin es el numero 12
}

void loop()
{
// **********************
// Valores de los sensores  
// **********************

//Valor del acelerador
Acelerador=analogRead(pinAcelerador);
//Valor del volante
Volante=analogRead(pinVolante);
//Valor de las Luces
Luces=digitalRead(pinLuces);
//Valor de si estoy en marcha adelante o atras
MarchaAdelante=digitalRead(pinMarchaAdelante);

// **********************
// Mostrar en Serial  
// **********************

Serial.print("Acelerador: ");
Serial.print(Acelerador);
Serial.print(" - ");
Serial.print("Volante: ");
Serial.print(Volante);
Serial.print(" - ");
Serial.print("Luces: ");
Serial.print(Luces);
Serial.print(" - ");
Serial.print("A/D: ");
Serial.print(MarchaAdelante);
Serial.print(" Check: ");
Serial.print(salidaCheck);
Serial.println(""); 

contador=contador+1;
salidaCheck = (contador%100)%2;
digitalWrite(pinCheck,salidaCheck);


// ******************************************
// Convertir los datos en cadena alfanumerica
// ******************************************
SSend = String("AA"); //Codigo de la tarjeta
SSend+= formateo(Acelerador);
SSend+= formateo(Volante);
SSend+= String(MarchaAdelante);
SSend+= String(Luces);
// Serial.println(SSend);

SSend.toCharArray(msg,13);
vw_send((uint8_t *)msg, strlen(msg));

delay(10); // retraso en la recepcion del mensaje
}

// funcion que formatea los numeros
String formateo (int numero) {
  char cadena[5];
  cadena[0]='0'+(numero/1000);
    numero = numero - 1000*round(numero/1000); 
  cadena[1]='0'+(numero/100);
    numero = numero - 100*round(numero/100); 
  cadena[2]='0'+(numero/10);
    numero = numero - 10*round(numero/10); 
  cadena[3]='0'+(numero%10);
  cadena[4]='\0'; //terminar cadena
  return String(cadena);
}

