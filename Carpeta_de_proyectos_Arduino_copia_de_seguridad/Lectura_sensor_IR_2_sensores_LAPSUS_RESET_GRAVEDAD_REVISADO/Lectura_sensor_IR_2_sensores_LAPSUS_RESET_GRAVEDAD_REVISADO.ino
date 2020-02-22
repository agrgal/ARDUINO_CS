long tiempo = 0; // variable general de tiempo
int i=0; // contador general

int corte[] = {0.0,0.0}; // valores de corte Vin en los sensores.

long lapsus[]={0,0}; // tiempo en cada sensor

long medida = 0; // valor de la medida temporal

double umbral = 1.3; // valor del umbral de corte

double deltat = 0; // medida temporal en segundos

double gravedad = 0.0; // valor de la gravedad

double error = 0.0; // error respecto a 9.8065m/s2

// the setup routine runs once when you press reset:
void setup() {
  for (i=0;i<=100;i++) {
    corte[0] = corte[0]+analogRead(A0); // valor inicial sensor A0
    corte[1] = corte[1]+analogRead(A5); // valor inicial sensor A5
  }
  corte[0]=corte[0]/100;
  corte[1]=corte[1]/100;
  // initialize serial communication at 9600 bits per second:
  
  Serial.begin(9600);
  Serial.println("Preparado para lanzar el móvil");

  Serial.println("Valor inicial sensor A0: " + (String) corte[0]);
  Serial.println("Valor inicial sensor A5: " + (String) corte[1]);
}

void(* resetFunc) (void) = 0; //declare reset function at address 0

// the loop routine runs over and over again forever:
void loop() {
  tiempo = micros(); //contando el tiempo desde que arranca ARDUINO

  // Serial.println("sensor A0: " + (String) analogRead(A0));
  // Serial.println("sensor A5: " + (String) analogRead(A5));

  // Sensor A
  // Si la lectura supera en un 100% a la de corte y aún no se ha definido lapsus[0]
  if (analogRead(A0)>corte[0]*umbral && lapsus[0]==0) {
    lapsus[0]=tiempo;
    Serial.println("Valor del tiempo sensor A0: " + (String) lapsus[0] + " // Valor sensor A0: " + analogRead(A0)); // toma el valor de tiempo lapsus[0]
  }

  // Sensor B
  // Si la lectura supera en un 100% a la de corte del segundo sensor y aún no se ha definido medida
  if ((analogRead(A5)>corte[1]*umbral) && medida==0 && lapsus[0]>0) {
    lapsus[1]=tiempo; // captura en lapsus[1] la segunda medida 
    medida = lapsus[1]-lapsus[0]; // calcula la medida temporal
    Serial.println("Valor del tiempo sensor A5: " + (String) lapsus[1]+ " // Valor sensor A5: " + analogRead(A5)); // toma el valor de tiempo lapsus[1]
    Serial.println("Diferencia de tiempo en us: " + (String) medida); // diferencia temporal
    Serial.println("Medida en milisegundos: " + (String) ((float) (medida/1000.0)) +" ms.");
    deltat = (double) (medida/1000000.0); // 
    Serial.println("Medida en segundos: " + (String) (deltat) +" s."); 
    gravedad = sqrt(2*0.24) - sqrt(2*0.03); // fórmula.
    gravedad = gravedad / deltat;
    gravedad = pow(gravedad,2);
    error = 100* (abs(gravedad - 9.8065) / 9.8065);
    // gravedad = g
    Serial.println("RESULTADO: " + (String) gravedad +" m/s2.");
    Serial.println("Error respecto valor teórico: "+ (String) error + "%");
  }

  // Si se presiona PIN 6, hay un RESET
  if (digitalRead(7)==HIGH) {
    Serial.println("Otra vez!!!");
    delay(1000);
    resetFunc(); //call reset  
  }
 
}
