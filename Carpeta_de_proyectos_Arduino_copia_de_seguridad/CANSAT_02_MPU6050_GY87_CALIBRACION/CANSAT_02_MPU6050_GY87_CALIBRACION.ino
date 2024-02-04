/*
 * 
 * El programa básicamente está modificando constantemente los offset intentando eliminar el error con la medida real que deseamos, en esta caso ax=0,ay=0,az=1g y gx=0,gy=0,gz=0.

Inicialmente leemos los offsets actuales y esperamos que el usuario envía un carácter por el puerto serie.
Antes de enviar el carácter es necesario ubicar el sensor en posición horizontal y evitar moverlo durante la calibración, dicha posición será nuestro nivel para futuras mediciones.

Después de enviar el carácter el programa realiza las lecturas tanto del acelerómetro como del giroscopio, usando un filtro estabilizamos un poco las lecturas y cada 100 lecturas comprobamos si los valores son cercanos a los valores que deseamos leer, dependiendo de esto se aumenta o disminuye los offsets. Esto hará que las lecturas filtradas converjan a:

-aceleración: p_ax=0 , p_ay=0 , p_az=+16384

-Velocidad angular: p_gx=0 , p_gy=0 , p_gz=0

Cuando en el monitor serial se observen valores cercanos a los anteriores debemos desconectar o reiniciar nuestro Arduino. Con esto el MPU6050 quedará configurado con el último offset calculado en el programa de calibración.
 * 
 * DEBE TENER UNAS SALIDAS (POSICION VERTICAL)  DEL TIPO:  16384  0  0    0  0  0 --> Por ejemplo,  16384  -2  1    0  1  15 
 */



// Librerias I2C para controlar el mpu6050
// la libreria MPU6050.h necesita I2Cdev.h, I2Cdev.h necesita Wire.h
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

// La dirección del MPU6050 puede ser 0x68 o 0x69, dependiendo 
// del estado de AD0. Si no se especifica, 0x68 estará implicito
MPU6050 mpu;

// Valores RAW (sin procesar) del acelerometro y giroscopio en los ejes x,y,z
int16_t ax, ay, az;
int16_t  gx, gy, gz;

//Variables usadas por el filtro pasa bajos
long f_ax,f_ay, f_az;
int p_ax, p_ay, p_az;
long f_gx,f_gy, f_gz;
int p_gx, p_gy, p_gz;
int counter=0;

//Valor de los offsets
int ax_o,ay_o,az_o;
int gx_o,gy_o,gz_o;

void setup() {
  Serial.begin(57600);   //Iniciando puerto serial
  Wire.begin();           //Iniciando I2C  
  mpu.initialize();    //Iniciando el mpu

  if (mpu.testConnection()) Serial.println("mpu iniciado correctamente");

  // Leer los offset los offsets anteriores
  ax_o=mpu.getXAccelOffset();
  ay_o=mpu.getYAccelOffset();
  az_o=mpu.getZAccelOffset();
  gx_o=mpu.getXGyroOffset();
  gy_o=mpu.getYGyroOffset();
  gz_o=mpu.getZGyroOffset();
  
  Serial.println("Offsets:");
  Serial.print(ax_o); Serial.print("\t"); 
  Serial.print(ay_o); Serial.print("\t"); 
  Serial.print(az_o); Serial.print("\t"); 
  Serial.print(gx_o); Serial.print("\t"); 
  Serial.print(gy_o); Serial.print("\t");
  Serial.print(gz_o); Serial.print("\t");
  Serial.println("\nnEnvie cualquier caracter para empezar la calibracion\n");  
  // Espera un caracter para empezar a calibrar
  while (true){if (Serial.available()) break;}  
  Serial.println("Calibrando, no mover IMU");  
  
}

void loop() {
  // Leer las aceleraciones y velocidades angulares
  mpu.getAcceleration(&ax, &ay, &az);
  mpu.getRotation(&gx, &gy, &gz);

  // Filtrar las lecturas
  f_ax = f_ax-(f_ax>>5)+ax;
  p_ax = f_ax>>5;

  f_ay = f_ay-(f_ay>>5)+ay;
  p_ay = f_ay>>5;

  f_az = f_az-(f_az>>5)+az;
  p_az = f_az>>5;

  f_gx = f_gx-(f_gx>>3)+gx;
  p_gx = f_gx>>3;

  f_gy = f_gy-(f_gy>>3)+gy;
  p_gy = f_gy>>3;

  f_gz = f_gz-(f_gz>>3)+gz;
  p_gz = f_gz>>3;

  //Cada 100 lecturas corregir el offset
  if (counter==100){
    //Mostrar las lecturas separadas por un [tab]
    Serial.print("promedio:"); Serial.print("t: ");
    Serial.print(p_ax); Serial.print("\t");
    Serial.print(p_ay); Serial.print("\t");
    Serial.print(p_az); Serial.print("\t");
    Serial.print(p_gx); Serial.print("\t");
    Serial.print(p_gy); Serial.print("\t");
    Serial.println(p_gz);

    //Calibrar el acelerometro a 1g en el eje z (ajustar el offset)
    
    // Posición de la placa orientada en posición vertical.
    // Cambiar los valores si la coloco en forma horizontal.
    int valorEjeX = 16384; // orientado hacia arriba según eje X
    int valorEjeZ = 0;     // Z en plano horizontal 
    if (p_ax-valorEjeX>0) ax_o--;
    else {ax_o++;} 
    if (p_ay>0) ay_o--;
    else {ay_o++;}
    if (p_az-valorEjeZ>0) az_o--;
    else {az_o++;} 
    
    mpu.setXAccelOffset(ax_o);
    mpu.setYAccelOffset(ay_o);
    mpu.setZAccelOffset(az_o);

    //Calibrar el giroscopio a 0º/s en todos los ejes (ajustar el offset)
    if (p_gx>0) gx_o--;
    else {gx_o++;}
    if (p_gy>0) gy_o--;
    else {gy_o++;}
    if (p_gz>0) gz_o--;
    else {gz_o++;}
    
    mpu.setXGyroOffset(gx_o);
    mpu.setYGyroOffset(gy_o);
    mpu.setZGyroOffset(gz_o);   

    // offsets
    Serial.println("Offsets: ");
    Serial.print(ax_o); Serial.print("\t"); 
    Serial.print(ay_o); Serial.print("\t"); 
    Serial.print(az_o); Serial.print("\t"); 
    Serial.print(gx_o); Serial.print("\t"); 
    Serial.print(gy_o); Serial.print("\t");
    Serial.println(gz_o); 
    Serial.println();

    counter=0;
  }
  counter++;
}
