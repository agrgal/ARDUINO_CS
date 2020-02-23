#include <Servo.h>

int pinServo=3;
int pinVolante=0; //pin donde se lee el Volante

int volante=0;

int pwm_a = 4;  //PWM control for motor outputs 1 and 2 
int pwm_b = 7;  //PWM control for motor outputs 3 and 4 
int dir_a = 5;  //direction control for motor outputs 1 and 2 
int dir_b = 6;  //direction control for motor outputs 3 and 4 

Servo miVolante;

void setup()
{
  pinMode(pwm_a, OUTPUT);  //Set control pins to be outputs
  pinMode(pwm_b, OUTPUT);
  pinMode(dir_a, OUTPUT);
  pinMode(dir_b, OUTPUT);  
 
  miVolante.attach(pinServo); // Pin donde esta cogido el servo
}

void loop()
{
  volante = analogRead(pinVolante);            // reads the value of the potentiometer (value between 0 and 1023) 
  volante = map(volante, 0, 1023, 0, 4);     // scale it to use it with the servo (value between 0 and 180) 
  miVolante.write(volante); 
  delay(100);
}


