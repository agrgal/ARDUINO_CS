long tiempo = 0;
int corte[] = {0,0};

long lapsus[]={0,0};

long medida = 0;

// the setup routine runs once when you press reset:
void setup() {
  corte [0] = analogRead(A0);
  corte [1] = analogRead(A5);
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Preparado para lanzar el móvil");
}

// the loop routine runs over and over again forever:
void loop() {
  tiempo = micros();
  if (analogRead(A0)>corte[0]*1.2 && lapsus[0]==0) {
    lapsus[0]=tiempo;
    Serial.println(lapsus[0]);
  }
  if ((analogRead(A5)>corte[1]*1.2) && medida==0) {
    lapsus[1]=tiempo;
    medida = lapsus[1]-lapsus[0];
    Serial.println(lapsus[1]);
    Serial.println(medida);
    Serial.println((String) ((float) (medida/1000.0)) +" ms.");
    Serial.println((String) ((float) (medida/1000000.0)) +" s.");
    exit;
  }
 
}
