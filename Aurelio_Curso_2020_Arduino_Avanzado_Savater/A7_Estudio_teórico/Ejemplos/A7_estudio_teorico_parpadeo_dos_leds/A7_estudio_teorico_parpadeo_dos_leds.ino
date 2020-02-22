// Parpadeo dos LEDs

unsigned tiempo = 0;
int frecuencia1 = 1000; // cada segundo
int frecuencia2 = 500; // cada 0.3 s

void setup() { 
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
}

void loop() {
  tiempo = millis(); // cada segundo
  digitalWrite(6,(tiempo/frecuencia1)%2);
  digitalWrite(7,(tiempo/frecuencia2)%2);
}
