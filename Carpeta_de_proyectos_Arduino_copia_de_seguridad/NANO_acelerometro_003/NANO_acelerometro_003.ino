//Incluimos la librería para I2C
#include <Wire.h>
 
//Definimos la direccion I2C del MPU
#define MPU 0x68
 
//Definimos los ratios de conversión
#define A_R 16384.0
#define G_R 131.0
 
//Definimos la conversion de radianes a grados 180/PI
#define RAD_A_DEG = 57.295779
 
//El MPU da los valores en enteros de 16 bits
//Declaramos memorias para los valores brutos
int16_t AcX; 
int16_t AcY; 
int16_t AcZ;
int16_t GyX; 
int16_t GyY;
int16_t GyZ;
 
//Declaramos cadena de caracteres para los ángulos
float Acc[2];
float Gy[2];
float Angle[2];

void setup()
{
//Inicializamos el I2C y el puerto serie
Wire.begin();
Wire.beginTransmission(MPU);
Wire.write(0x6B);
Wire.write(0);
Wire.endTransmission(true);
Serial.begin(9600);
}

void loop()
{
   //Leemos los valores del Acelerometro
   Wire.beginTransmission(MPU);
   Wire.write(0x3B); //Pedimos el registro 0x3B - corresponde al AcX
   Wire.endTransmission(false);
   Wire.requestFrom(MPU,6,true); //A partir del 0x3B, se piden 6 registros
   AcX=Wire.read()<<8|Wire.read(); //Cada valor ocupa 2 registros
   AcY=Wire.read()<<8|Wire.read();
   AcZ=Wire.read()<<8|Wire.read();
 
   //Calculamos los angulos Y, X respectivamente.
  Acc[1] = atan(-1*(AcX/A_R)/sqrt(pow((AcY/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
  Acc[0] = atan((AcY/A_R)/sqrt(pow((AcX/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
 
   //Leemos los valores del Giroscopio
   Wire.beginTransmission(MPU);
   Wire.write(0x43);
   Wire.endTransmission(false);
   Wire.requestFrom(MPU,4,true); //A diferencia del Acelerometro, solo se piden 4 registros
   GyX=Wire.read()<<8|Wire.read();
   GyY=Wire.read()<<8|Wire.read();
 
   //Calculamos el angulo del Giroscopio
   Gy[0] = GyX/G_R;
   Gy[1] = GyY/G_R;
 
   //Aplicamos un Filtro Complementario
   Angle[0] = 0.98 *(Angle[0]+Gy[0]*0.010) + 0.02*Acc[0];
   Angle[1] = 0.98 *(Angle[1]+Gy[1]*0.010) + 0.02*Acc[1];
 
   //Mostramos los valores por el monitor serial
   Serial.print("Angle X: "); Serial.println(Angle[0]);
   Serial.print("Angle Y: "); Serial.println(Angle[1]);
   
  //Esperamos 1 segundo para poder visualizarlo en el monitor serial
   delay(1000);
}
