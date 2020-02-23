// this sketch cycles three servos at different rates

#include <ServoTimer2.h>  // the servo library

// define the pins for the servos
#define rollPin  2
#define rectoPin 4
int pinEntrada=A0;

ServoTimer2 servoRoll;    // declare variables for up to eight servos

int i;
int val,val2;

void setup() {
  pinMode(rectoPin,INPUT);
  Serial.begin(9600);
  servoRoll.attach(rollPin);     // attach a pin to the servos and they will start pulsing
}


// this function just increments a value until it reaches a maximum
int incPulse(int val, int inc){
   if( val + inc > 2000 )
	return 1000 ;
   else
	 return val + inc;
}


// Funcin principal
void loop()
{
   if (digitalRead(rectoPin)) {
     delay(50);
     if (digitalRead(rectoPin)) {
        servoRoll.write(1475); //punto medio 
     } 
   } else {
   
   val=analogRead(pinEntrada);
      
   val2=map(val,0,1023,700,2250); // mapeo a los valores en microseconds que dice ServoTimer2.h
   servoRoll.write(val2);
   
   }

   Serial.print(val);
   Serial.print(" - ");
   Serial.println(val2);

   delay(10);
}
