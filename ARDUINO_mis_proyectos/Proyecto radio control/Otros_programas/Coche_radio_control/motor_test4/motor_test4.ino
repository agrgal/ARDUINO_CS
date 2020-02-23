
#include <Servo.h>

int pinServo=3;

int E1 = 5; // motor 1 PWM
int M1 = 4; // control de direccion motor 1
int E2 = 6;                     
int M2 = 7; 

int aceleracion, volante, marchaAD; // tres variables

Servo miVolante;

void setup () {
   pinMode(M1, OUTPUT);  
   pinMode(M2, OUTPUT); 
   miVolante.attach(pinServo); // Pin donde esta cogido el servo
  
  Serial.begin(9600); 
  Serial.println("Reading...");
}

void loop() {

while (Serial.available() > 0) {
  
  delay(10);
  
  Serial.println("Available");
  
  // Lectura de datos del puerto serial
  
  int n1 = Serial.parseInt();
  int n2 = Serial.parseInt();
  int n3 = Serial.parseInt();
  
  if (n1>=0 && n1<=1) { // entre 0 y uno
     marchaAD=n1;
  } else if (n1>=1000 && n1<2000) {
     aceleracion = n1 - 1000;
  } else if (n1>=5000) {
     volante = n1-5000; 
  }
  
  if (n2>=0 && n2<=1) { // entre 0 y uno
     marchaAD=n2;
  } else if (n2>=1000 && n2<2000) {
     aceleracion = n2 - 1000;
  } else if (n2>=5000) {
     volante = n2-5000; 
  }
  
  if (n3>=0 && n3<=1) { // entre 0 y uno
     marchaAD=n3;
  } else if (n3>=1000 && n3<2000) {
     aceleracion = n3 - 1000;
  } else if (n3>=5000) {
     volante = n3-5000; 
  }
  
  // if (Serial.read() == '\n') {
    Serial.print(aceleracion);
    Serial.print("     -      ");
    Serial.print(volante);
    Serial.print("     -      ");
    Serial.println(marchaAD);
  // } // end of serial read
  
  aceleracion = map(aceleracion,0,255,50,255); // control de potencia del motor
  analogWrite(E1,aceleracion); // potencia del motor
  digitalWrite(M1,marchaAD); //cambio de direccion del motor
  
  int giro = map(volante,0,255,70,115); // cambiar ngulo y ver
  miVolante.write(giro); 
  
  // Activacin del motor...
  
} // end of while serial available 

  analogWrite(E1,0);
   
}
