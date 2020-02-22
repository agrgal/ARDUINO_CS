/*
 Estudiar primero cómo recibe la señal el LDR; ver su máximo, y la luz a medioambiente. Obtner valores, o la gráfica directamente
 Si obtengo la gráfica, guardr en fichero CSV y abrir con LibreOffice Calc. Represnetar y en losdatos, mostrar categorías.
 Se ven muy bien las oscialciones. Reconocer la forma de la onda, y calcular el perído. 
 Comprobar con la fórmula T = 2*Pi*RAIZ(longitud/g) 
 */
int pinBoton = 7;
int pinLASER = 8;

long tiempo = 0;
long tiempo1 = 0;
long diferencia = 0;

float voltage = 0;
float voltagemenos1 = 0;
float voltagemenos2 = 0;

int contar = 0;

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

  // Serial.println(String(tiempo)+" ; "+voltaje);

  // Reconocimiento de 
  // if (voltage<900 && estado==HIGH) { // Cuando el voltaje es menor de 900 y el LASER está encendido
  if (estado==HIGH && voltagemenos2>=voltagemenos1 && voltage>=voltagemenos1 && voltagemenos1<900) {   
     contar = contar + 1;
     if (contar%2==0) { // Si el conteo es par
        diferencia = tiempo - tiempo1;
        Serial.println(String(diferencia)+"; "+String(longitud(diferencia)));
        // Refresco tiempo
        tiempo1 = tiempo;
        contar = 0;
     } // Fin de la condición que el conteo sea par
     // pequeño retraso para evitar más de una lectura
     // delay(75);     
  } 

  voltagemenos2 = voltagemenos1;
  voltagemenos1 = voltage;
  
  delay(25);    
}

// Funcion que devuelve la longitud
float longitud(long periodo) {
  float calculo = 100*9.8*pow((periodo / (2.0 * 3.141596254)),2) ;
  // devuelve la longitud del péndulo en centímetros
  return calculo;
}


