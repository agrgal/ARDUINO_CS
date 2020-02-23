
int pinBoton = 7;
int pinLASER = 8;

long tiempo = 0;

boolean estado = false;

boolean estadoBoton = LOW;

// the setup routine runs once when you press reset:
void setup() {

  pinMode(pinBoton,INPUT);
  pinMode(pinLASER,OUTPUT);
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {

  // Encender el laser o no
  int estadoBotonAhora = digitalRead(pinBoton);

  if (estadoBotonAhora==HIGH && estadoBoton==LOW) {
      estado = estado xor 1;
      delay(50);
  }

  estadoBoton = estadoBotonAhora;
  digitalWrite(pinLASER,estado);
  // Fin de encender el laser
  
  tiempo = millis();
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue;
  String voltaje = String(voltage);
  voltaje.replace(".",",");

  // if (voltage<800) {
  Serial.println(String(tiempo)+" ; "+voltaje);
  // }
  delay(100);  
  
}
