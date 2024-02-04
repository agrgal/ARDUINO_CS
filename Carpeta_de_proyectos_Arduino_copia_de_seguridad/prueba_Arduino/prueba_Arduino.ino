// ==========
// VARIABLES
// ==========

int disparo= 0;
double velocidadSonido = 343.0;
int distancia = 0;

// =====
// SETUP
// =====

void setup()
{
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  // establece la distancia de disparo en cm
  disparo = 30;
  // cambio la velocidad del sonido a cm por microsegundos
  velocidadSonido = (velocidadSonido * 100) / 1e6;
}

// ========================
// LOOP: programa principal
// ========================

void loop()
{

  // medida en centimetros
  distancia = velocidadSonido * lecturaUltrasonidos(7, 6);
  distancia = distancia / 2; // porque va y viene la señal
  // 
  Serial.print(distancia);
  Serial.println("cm");
  
  if (distancia > disparo) {
    digitalWrite(4, LOW);    
  }
  if (distancia <= disparo) {
    digitalWrite(4, HIGH);
  }  
  delay(100); // Espera 100 milisegundos
}

// ==========
// FUNCIONES
// ==========

long lecturaUltrasonidos(int pinDisparo, int pinEco)
{
  pinMode(pinDisparo, OUTPUT);  // Inicializa trigger
  digitalWrite(pinDisparo, LOW);
  delayMicroseconds(2);
  // Durante 10 microsegundos se emite señal
  digitalWrite(pinDisparo, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinDisparo, LOW);
  pinMode(pinEco, INPUT); // inicializa echo
  // retorna el tiempo en microsegundos leido en el ECO
  return pulseIn(pinEco, HIGH);
}
