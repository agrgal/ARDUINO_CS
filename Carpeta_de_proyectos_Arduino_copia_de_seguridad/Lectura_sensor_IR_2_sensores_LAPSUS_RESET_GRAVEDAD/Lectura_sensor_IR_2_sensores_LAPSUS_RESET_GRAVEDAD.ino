long tiempo = 0; // variable general de tiempo

int corte[] = {0,0}; // valores de corte Vin en los sensores.

long lapsus[]={0,0}; // tiempo en cada sensor

long medida = 0; // valor de la medida temporal

double deltat = 0; // medida temporal en segundos

double gravedad = 0.0; // valor de la gravedad

double error = 0.0; // error respecto a 9.8065m/s2

// the setup routine runs once when you press reset:
void setup() {
  corte [0] = analogRead(A0); // valor inicial sensor A0
  corte [1] = analogRead(A5); // valor inicial sensor A5
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Preparado para lanzar el móvil");
}

void(* resetFunc) (void) = 0; //declare reset function at address 0

// the loop routine runs over and over again forever:
void loop() {
  tiempo = micros(); //contando el tiempo desde que arranca ARDUINO

  // Sensor A
  // Si la lectura supera en un 20% a la de corte y aún no se ha definido lapsus[0]
  if (analogRead(A0)>corte[0]*1.05 && lapsus[0]==0) {
    lapsus[0]=tiempo;
    Serial.println("Valor del tiempo sensor A0: " + (String) lapsus[0]); // toma el valor de tiempo lapsus[0]
  }

  // Sensor B
  // Si la lectura supera en un 20% a la de corte del segundo sensor y aún no se ha definido medida
  if ((analogRead(A5)>corte[1]*1.05) && medida==0) {
    lapsus[1]=tiempo; // captura en lapsus[1] la segunda medida 
    medida = lapsus[1]-lapsus[0]; // calcula la medida temporal
    Serial.println("Valor del tiempo sensor A5: " + (String) lapsus[1]); // toma el valor de tiempo lapsus[1]
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


