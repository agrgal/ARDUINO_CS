// proyecto semáforo acústico
// Consiste en detectar el nivel de ruido y emitir una luz de colores
// actuando como un semáforo

long valor = 0; // valor de entrada del potenciómetro analógico
int pinElegido[]={3,5,6}; // valores de los pins
int colores[3][3]={{255,0,0},{200,255,50},{0,255,0}}; // un color "amarillo" conseguido es este...
int numPin = 2;

void setup() {
  pinMode(3,OUTPUT); // salidas analógicas
  pinMode(5,OUTPUT); // salidas analógicas
  pinMode(6,OUTPUT); // salidas analógicas
  Serial.begin(9600);

}

void loop() {
  valor = analogRead(A0);
  encender(valor); // devuelve un número entre 0 y 2 con la situación del valor
  delay(10);
}

void encender(long valores) {
  int cual = map(valores,0,900,0,2);
  for (int i=0;i<=2;i++) {
    analogWrite(pinElegido[i],colores[cual][i]); //en cada pin Red 3, Green 5, Blue 6 se escribe los valores de "cual" he escogido
  }
  Serial.println((String) cual +" : "+ (String) colores[cual][0] +":" +(String) colores[cual][1] +":"+ (String) colores[cual][2] ) ;
}

