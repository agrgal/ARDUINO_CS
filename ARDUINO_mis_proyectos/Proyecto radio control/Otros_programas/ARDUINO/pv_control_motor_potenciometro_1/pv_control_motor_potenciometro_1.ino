#include <Servo.h>

int pwm_a = 5;  //PWM control for motor outputs 1 and 2 
int pwm_b = 6;  //PWM control for motor outputs 3 and 4 
int dir_a = 4;  //direction control for motor outputs 1 and 2 
int dir_b = 7;  //direction control for motor outputs 3 and 4 

int pinCambioDireccion = 12; //cambiodireccion
int pinLedCambioDireccion=3;
int pinServo=11; // tiene que ser con salida analogica. El 11 esta libre
int CambioDireccion = LOW; //valor del cambio direccion

int entradaPotenciometro = 0; // valor de la entrada del potencimetro
int salidaServo=0; //valor del giro del Servo.

Servo miVolante; //objeto volante

void setup()
{
  pinMode(pwm_a, OUTPUT);  //Set control pins to be outputs
  pinMode(pwm_b, OUTPUT);
  pinMode(dir_a, OUTPUT);
  pinMode(dir_b, OUTPUT);   
  pinMode(pinCambioDireccion,INPUT);
  pinMode(pinLedCambioDireccion,OUTPUT);
  miVolante.attach(pinServo);
  Serial.begin(9600);
}

void loop()
{
  if (digitalRead(pinCambioDireccion)==HIGH) { // primera comprobacion
     delay(75);
     if (digitalRead(pinCambioDireccion)==HIGH) { 
       CambioDireccion=CambioDireccion^1;
     }
  }
  digitalWrite(dir_a, CambioDireccion); // direccion motor A
  digitalWrite(pinLedCambioDireccion,CambioDireccion); // giro del Motor
  entradaPotenciometro = analogRead(A0); // lectura 
  salidaServo=map(entradaPotenciometro,0,1023,0,180); //convertir a valores de grado 
  entradaPotenciometro=map(entradaPotenciometro,0,1023,25,150); // conversin entre valores 
    miVolante.write(salidaServo); //escribe el valor en el Servo
  analogWrite(pwm_a, entradaPotenciometro); //coloca ese valor a la salida. 
  delay(100); // algo de retraso
  Serial.println(CambioDireccion);
}


