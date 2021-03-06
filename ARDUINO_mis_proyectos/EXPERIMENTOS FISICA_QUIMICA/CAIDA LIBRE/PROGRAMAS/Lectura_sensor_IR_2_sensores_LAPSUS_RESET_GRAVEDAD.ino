long tiempo = 0;
int corte[] = {0,0};

long lapsus[]={0,0};

long medida = 0;

double deltat = 0;

double gravedad = 0.0;

// the setup routine runs once when you press reset:
void setup() {
  corte [0] = analogRead(A0);
  corte [1] = analogRead(A5);
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Preparado para lanzar el móvil");
}

void(* resetFunc) (void) = 0;//declare reset function at address 0

// the loop routine runs over and over again forever:
void loop() {
  tiempo = micros();

  // Sensor A
  if (analogRead(A0)>corte[0]*1.2 && lapsus[0]==0) {
    lapsus[0]=tiempo;
    Serial.println(lapsus[0]);
  }

  // Sensor B
  if ((analogRead(A5)>corte[1]*1.2) && medida==0) {
    lapsus[1]=tiempo;
    medida = lapsus[1]-lapsus[0];
    Serial.println(lapsus[1]);
    Serial.println(medida);
    Serial.println((String) ((float) (medida/1000.0)) +" ms.");
    deltat = (double) (medida/1000000.0);
    Serial.println((String) (deltat) +" s."); 
    gravedad = sqrt(2*0.24) - sqrt(2*0.03);
    gravedad = gravedad / deltat;
    gravedad = pow(gravedad,2);
    // gravedad = g
    Serial.println((String) gravedad +" m/s2.");
  }

  // Si se presiona PIN 6, hay un RESET
  if (digitalRead(7)==HIGH) {
    Serial.println("Otra vez!!!");
    delay(1000);
    resetFunc(); //call reset  
  }
 
}


