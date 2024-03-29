#define LED_PIN 13
#include "Wire.h" // Necesaria para I2Cdev
#include "I2Cdev.h" //Creo que es una biblioteca incorporada a Arduino core. 
#include "MPU6050.h" // biblioteca para el MPU650.h
#include <Adafruit_BMP085.h> //biblioteca para el BMP180

/*  Constantes que definen el rango del acelerómetro
#define MPU6050_ACCEL_FS_2          0x00
#define MPU6050_ACCEL_FS_4          0x01
#define MPU6050_ACCEL_FS_8          0x02
#define MPU6050_ACCEL_FS_16         0x03

Y del giróscopo
#define MPU6050_GYRO_FS_250         0x00
#define MPU6050_GYRO_FS_500         0x01
#define MPU6050_GYRO_FS_1000        0x02
#define MPU6050_GYRO_FS_2000        0x03

// Anchos de banda
#define MPU6050_DLPF_BW_256         0x00
#define MPU6050_DLPF_BW_188         0x01
#define MPU6050_DLPF_BW_98          0x02
#define MPU6050_DLPF_BW_42          0x03
#define MPU6050_DLPF_BW_20          0x04
#define MPU6050_DLPF_BW_10          0x05
#define MPU6050_DLPF_BW_5           0x06
 */

// Objetos de las distintas clases
Adafruit_BMP085 bmp;
const int mpuAddress = 0x68;  // Nuestro caso 0x68. Puede ser 0x69
MPU6050 mpu(mpuAddress);

// Constantes y variables
const float g0 = 9.80665; //valor de la gravedad estándar en m/s2
const float pi = 4*atan(1);
int16_t ax, ay, az;
int16_t gx, gy, gz;

double escalaAcelerometro = 0;
double escalaGiroscopio = 0;

bool blinkState = false;

void setup() {

  delay(2000); // espera 2 segundos
  
  Serial.begin(9600); 
  
  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 , check wiring!");
	while (1) {}
  }

  // initialize mpu6050
  Wire.begin();
  mpu.initialize();
  Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  // mpu.setI2CBypassEnabled(true); // set bypass mode for gateway to hmc5883L

  // configure Arduino LED for checking activity
  pinMode(LED_PIN, OUTPUT);
  
  // set accelerometer range to .... +-2 +-4 +-8G +-16
  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_4); // rango 2 --> 0, 4 --> 1, 8 --> 2, 16 --> 3
  escalaAcelerometro = pow(2.0, float(1 + mpu.getFullScaleAccelRange()) ); // elevo a la potencia de 2
  // Serial.println("Escala del acelerómetro: --> " + String (escalaAcelerometro));
  
  // set gyro range to +- 250 deg/s +- 500 deg/s +- 1000 deg/s +- 2000 deg/s
  mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_2000); // rango 250 --> 0, 500 --> 1, 1000 --> 2, 2000 --> 3
  escalaGiroscopio = 250.0 * pow(2.0,float(mpu.getFullScaleGyroRange())); // elevo a la potencia de 2 y multiplico por 250
  // Serial.println("Escala del giróscopo: --> " + String (escalaGiroscopio));

  // set filter bandwidth to 98 Hz  
  Serial.println("Ancho de banda: " + String (mpu.getDLPFMode()));
  mpu.setDLPFMode(MPU6050_DLPF_BW_98); // Investigar un mejor ancho de banda...
  
}
  
void loop() {

    // ****
    // BMP
    // ****
    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    
    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(F(" Pa"));
    
    Serial.print(F("Real altitude = "));
    Serial.print(bmp.readAltitude(102280)); // Introducir la presión atmosférica a nivel del suelo.
    Serial.println(F(" meters"));

    // *******
    // MPU6050
    // *******
    /* Get new sensor events with the readings */
    // read raw accel/gyro measurements from device
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    // mpu.getAcceleration(&ax, &ay, &az);
    // mpu.getRotation(&gx, &gy, &gz);
    
    // display tab-separated accel/gyro x/y/z values
    /* Serial.print(F("a/g:\t"));
    // Serial.print(String (amxs2(ax)) +" : "+ String (ax)); Serial.print("\t");
    Serial.print(amxs2(ax)); Serial.print("\t"); // En posición vertical me da la aceleración
    Serial.print(amxs2(ay)); Serial.print("\t");
    Serial.print(amxs2(az)); Serial.print("\t");
    Serial.print(gradoporsegundo(gx)); Serial.print("\t");
    Serial.print(gradoporsegundo(gy)); Serial.print("\t");
    Serial.print(gradoporsegundo(gz)); */

    // Muestra los ángulos de inclinación. Recordemos que nuestra placa se encuentra en posición vertical, luego la componente 
    // Normal es el eje X.
    az = amxs2(az); ay = amxs2(ay); ax = amxs2(ax); 
    float angZ = atan(az/sqrt(ax*ax+ay*ay))*(180/pi);
    float angY = atan(ay/sqrt(ax*ax+az*az))*(180/pi);
    Serial.println("Ángulo Y: " + String (angY) + "º // Ángulo Z: " + String(angZ));

  // ***************************************
    Serial.println();
    blinkState = !blinkState;
    digitalWrite(LED_PIN,blinkState);
    delay(1000);
}

  // ***************************************
  // Funciones
  // ***************************************

float amxs2 (int valor) { // Función que dado un valor int signed de 16 bits, te devuelven la aceleración em m/s2
  // Los valores de nuestro GY87 se obtienen como enteros con signo, entre -32768 to 32767
  // Si la escala de nuestro acelerómetro está entre -2g y 2g  
  // y = ax +b , variablemedida = a * aceleracion + b 
  // aceleracion = (variablemedida -b)/a --> para valores [-2*g, -32768] y [2g, 32767]
  float a = (32767+0.5)/(escalaAcelerometro*g0); // en vez de 2, elijo la escala con la variable escalaAcelerometro
  float b = -0.5;
  return ((valor - b)/a);
}

float gradoporsegundo (int valor) { //función que convierte valores int signed de 16 bits, en grado/segundo
  return valor/escalaGiroscopio;
}
