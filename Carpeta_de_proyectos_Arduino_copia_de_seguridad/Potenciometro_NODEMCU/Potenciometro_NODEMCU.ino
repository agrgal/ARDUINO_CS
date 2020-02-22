
#define LED D1  // Lo conectamos a D0 o al GPIO16
// Mapeo de pines --> https://programarfacil.com/wp-content/uploads/2017/06/pines-nodemcu.png
// Todos los pines GPIO son salidas analógicas, y se puede usar PWM. Además con 2^10 niveles (10 bits).

int potencia = 0;

void setup() {
  Serial.begin(115200); //comunicacion en serie
  // El LED integrado está conectado al pin 2.
  pinMode(LED, OUTPUT);
  pinMode(A0, INPUT); // Entrada analógica
}

void loop() {
  // lectura del potenciómetro
  potencia =analogRead(A0); // La tensión básica es de 3.3V
  Serial.println(potencia);
  // Hago el cálculo contrario (En ESP8266 trabajan con lógica negativa ¿?)
  potencia = 1023-potencia;
  analogWrite(LED,potencia);  
}
