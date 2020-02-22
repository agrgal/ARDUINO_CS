/* 
 Stepper Motor Control - one revolution
 Este programa impulsa un motor paso a paso unipolar o bipolar. 
 El motor está conectado a los pines digitales 8 - 11 de la Arduino. 
 Created 11 Mar. 2007  by Tom Igoe
 Modificado por Aurelio Gallardo, 16/2/2020
 */

#include <Stepper.h>

const int stepsPerRevolution = 200;  
// cambie este valor por el numero de pasos de su motor

// inicializa la libreria 'stepper' en los pines 8 a 11
Stepper myStepper(stepsPerRevolution, 8,9,10,11);            

void setup() {
  // establece la velocidad en 5 rpm
  myStepper.setSpeed(5);
  Serial.begin(9600);
}

void loop() {
  Serial.println("clockwise"); // gira una vuelta en una direccion
  myStepper.step(stepsPerRevolution);
  delay(5000); // Espera 5 segundos  
  Serial.println("counterclockwise"); // gira otra vuelta en la otra direccion
  myStepper.step(-stepsPerRevolution);
  delay(5000); 
}
