// proyecto semáforo acústico
// Consiste en detectar el nivel de ruido y emitir una luz de colores
// actuando como un semáforo

long valor = 0; // valor de entrada del potenciómetro analógico
int pinElegido = 2;

void setup() {
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  Serial.begin(9600);

}

void loop() {
  valor = analogRead(A0);
  pinElegido=map(valor,0,900,2,4);
  encender(pinElegido);
  Serial.println(valor);
}

void encender(int cual) {
  int i;
  for (i=2;i<=4;i++) {
    if (i==cual) {
      digitalWrite(i,true);
    } else {
      digitalWrite(i,false);
    }
  }
}

