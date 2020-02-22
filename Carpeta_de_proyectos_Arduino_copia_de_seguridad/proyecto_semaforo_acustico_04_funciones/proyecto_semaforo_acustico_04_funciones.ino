// proyecto semáforo acústico
// Consiste en detectar el nivel de ruido y emitir una luz de colores
// actuando como un semáforo

long valor = 0; // valor de entrada del potenciómetro analógico
int pinElegido[]={3,5,6}; // valores de los pins
// int colores[3][3]={{255,0,0},{200,255,50},{0,255,0}}; // un color "amarillo" conseguido es este...
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
  delay(50);
}

void encender(long dato) {
  int valores = map(dato,0,920,0,900); // depuración del rango... El sensor no da más de 920 (ver cuando cambie de sensor)
  int cual = map(dato,0,920,0,); // Elijo cual es el que me corresponde... Pero a lo mejor no hace falta
  float x = valores / 450.0; // paso a las x los valores, entre 0 y 2 (máximo se toma 900; comprobar con el máximo del micrófono).
  float red = -72.5*x*x+17.5*x+255.0; // esta función calcula los valores de rojo, pasando por los colores 255 en 0,200 en 1 y 0 en 2
  // 1er intento --> float green = -127.50*x*x+382.5*x; // esta función calcula los valores de verde, pasando por los colores 0 en 0,255 en 1 y 255 en 2
  float green = -72.50*x*x+272.5*x; // esta función calcula los valores de verde, pasando por los colores 0 en 0, 200 en 1 y 255 en 2
  float blue = -50.0*x*x+100.0*x; // esta función calcula los valores de azul, pasando por los colores 0 en 0,50 en 1 y 0 en 2
  analogWrite(pinElegido[0],(int) red); // Escribo en el pin primero el valor de los rojos
  analogWrite(pinElegido[1],(int) green); // Escribo en el pin primero el valor de los rojos
  analogWrite(pinElegido[2],(int) blue); // Escribo en el pin primero el valor de los rojos
  /* 
  for (int i=0;i<=2;i++) {
    analogWrite(pinElegido[i],colores[cual][i]); //en cada pin Red 3, Green 5, Blue 6 se escribe los valores de "cual" he escogido
  } */
  // Serial.println((String) cual +" : "+ (String) colores[cual][0] +":" +(String) colores[cual][1] +":"+ (String) colores[cual][2] ) ;
  Serial.println((String) dato +" --> "+ (String) x +" : "+ (String) red +":" +(String) green +":"+ (String) blue + " cualitativo: " + (String) cual ) ;
}

