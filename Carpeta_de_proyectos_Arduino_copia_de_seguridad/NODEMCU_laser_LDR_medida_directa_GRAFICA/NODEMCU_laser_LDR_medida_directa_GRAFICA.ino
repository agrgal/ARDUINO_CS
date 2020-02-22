/*
 Estudiar primero cómo recibe la señal el LDR; ver su máximo, y la luz a medioambiente. Obtner valores, o la gráfica directamente
 Si obtengo la gráfica, guardr en fichero CSV y abrir con LibreOffice Calc. Represnetar y en losdatos, mostrar categorías.
 Se ven muy bien las oscialciones. Reconocer la forma de la onda, y calcular el perído. 
 Comprobar con la fórmula T = 2*Pi*RAIZ(longitud/g) 
 */
#define pinLASER D8

long tiempo = 0;

float voltage = 0;

// the setup routine runs once when you press reset:
void setup() {

  pinMode(pinLASER,OUTPUT);
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);

  digitalWrite(pinLASER,HIGH); // El laser empieza, lógicamente, apagado  
}

// the loop routine runs over and over again forever:
void loop() {
 
  tiempo = millis();
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue;
  String voltaje = String(voltage);
  voltaje.replace(".",",");

  Serial.println(String(tiempo)+" ; "+voltaje);
  delay(10);
}
