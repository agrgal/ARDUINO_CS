// proyecto semáforo acústico
// Consiste en detectar el nivel de ruido y emitir una luz de colores
// actuando como un semáforo

long valor = 0; // valor de entrada del potenciómetro analógico
int pinElegido[]={3,5,6}; // valores de los pins
int numPin = 2;

void setup() {
  pinMode(3,OUTPUT); // salidas analógicas
  pinMode(5,OUTPUT); // salidas analógicas
  pinMode(6,OUTPUT); // salidas analógicas
  Serial.begin(9600);

}

void loop() {
  valor = analogRead(A0);
  numPin = map(valor,0,900,0,2);
  encender(numPin);
  Serial.println((String) pinElegido[numPin] + " -- " + (String) valor);
}

void encender(int cual) {
  int i;
  for (i=0;i<=2;i++) {
    if (i==cual) {
      digitalWrite(pinElegido[i],true);
    } else {
      digitalWrite(pinElegido[i],false);
    }
  }
}

