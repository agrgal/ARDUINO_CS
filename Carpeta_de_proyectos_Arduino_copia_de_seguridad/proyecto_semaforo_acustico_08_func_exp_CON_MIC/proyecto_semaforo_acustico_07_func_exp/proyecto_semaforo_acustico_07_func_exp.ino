// proyecto semáforo acústico
// Consiste en detectar el nivel de ruido y emitir una luz de colores
// actuando como un semáforo

long valor = 0; // valor de entrada del potenciómetro analógico
int pinElegido[]={3,5,6}; // valores de los pins
float colorIntermedio[]={10.0,100.0,100.0}; // colores que se alcanzan en el valor intermedio (1) RGB
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
  delay(250);
}

// *********
// Funciones
// *********

void encender(long dato) {
  int valores = map(dato,0,1023,0,900); // depuración del rango... El sensor no da más de 920 (ver cuando cambie de sensor)
  int cual = map(dato,0,1023,0,10); // Elijo cual es el que me corresponde... Pero a lo mejor no hace falta
  float x = valores / 450.0; // paso a las x los valores, entre 0 y 2 (máximo se toma 900; comprobar con el máximo del micrófono).

  // Funcion parabólica para el rojo
  float red = (127.5-colorIntermedio[0])*x*x+(2*colorIntermedio[0]-382.5)*x+255.0; // esta función calcula los valores de rojo, pasando por los colores 255 en 0,colorIntermedio[0] en 1 y 0 en 2
  red=red*(red>=0.0); // corta los valores negativos y los pone a cero

  // **********************************
  // Función exponencial para el verde
  // **********************************
  float Agreen= (colorIntermedio[1]*colorIntermedio[1])/(2.0*colorIntermedio[1]-255.0);
  float Cgreen = log(1.0-colorIntermedio[1]/Agreen);
  // Serial.print(Agreen,6);   Serial.print(" -- " );   Serial.print(Cgreen,6);
  float green = Agreen*(1-exp(Cgreen*x));  
  // float green = (127.5-colorIntermedio[1])*x*x+(2*colorIntermedio[1]-127.5)*x+0.0; // esta función calcula los valores de verde, pasando por los colores 0 en 0,colorIntermedio[1] en 1 y 0 en 2
  // green=green*(green>=0.0); // corta los valores negativos y los pone a cero
  
  float blue = -colorIntermedio[2]*x*x+2*colorIntermedio[2]*x; // esta función calcula los valores de azul, pasando por los colores 0 en 0,50 en 1 y 0 en 2
  blue=blue*(blue>=0.0); // corta los valores negativos y los pone a cero
  
  analogWrite(pinElegido[0],(int) red); // Escribo en el pin primero el valor de los rojos. Valor absoluto corta los valores negativos
  analogWrite(pinElegido[1],(int) green); // Escribo en el pin primero el valor de los rojos
  analogWrite(pinElegido[2],(int) blue); // Escribo en el pin primero el valor de los rojos
  /* 
  for (int i=0;i<=2;i++) {
    analogWrite(pinElegido[i],colores[cual][i]); //en cada pin Red 3, Green 5, Blue 6 se escribe los valores de "cual" he escogido
  } */
  // Serial.println((String) cual +" : "+ (String) colores[cual][0] +":" +(String) colores[cual][1] +":"+ (String) colores[cual][2] ) ;
  Serial.println((String) dato +" --> "+ (String) x +" : RGB ("+ (String) red +"," +(String) green +","+ (String) blue + ") cualitativo: " + (String) cual ) ;
}

