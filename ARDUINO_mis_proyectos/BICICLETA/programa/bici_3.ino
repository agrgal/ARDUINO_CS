// declaracion de variables
// Freno
const int luzFreno = 3;
const int pulsadorFreno = 10;
// Intermitentes
const int intermitenteIzquierdo= 5;
const int intermitenteDerecho= 4;
const int pulsadorIzquierdo= 12;
const int pulsadorDerecho= 11;
// Luces cortas y largas
const int Largas = 6;
const int Cortas = 7;
const int pulsadorLuces = 13;

// variables de estado
int estadoFreno = LOW;
int estadoLuces = 0;
int estadoIIz = 0;
int estadoIDr = 0;

// variables temporales
unsigned long t = 0;
unsigned long retardo = 10; // segundos que dura el parpadeo
unsigned long cadencia =200; // cadencia o frecuencia de parpadeo
// tiempo intermitente izquierdo
unsigned long tIIz = 0;
unsigned long difIIz = 0;
// tiempo intermitente derecho
unsigned long tIDr = 0;
unsigned long difIDr = 0;

void setup () {
  // salidas
  pinMode(luzFreno,OUTPUT);
  pinMode(intermitenteIzquierdo,OUTPUT);
  pinMode(intermitenteDerecho,OUTPUT);
  pinMode(Largas,OUTPUT);
  pinMode(Cortas,OUTPUT);
  // pulsadores
  pinMode(pulsadorFreno,INPUT);
  pinMode(pulsadorIzquierdo,INPUT);
  pinMode(pulsadorDerecho,INPUT);
  pinMode(pulsadorLuces,INPUT);
}

void loop() {
  // Prueba de encendido
  /* digitalWrite(luzFreno,HIGH);
  digitalWrite(intermitenteIzquierdo,HIGH);
  digitalWrite(intermitenteDerecho,HIGH);
  digitalWrite(Largas,HIGH);
  digitalWrite(Cortas,HIGH); */
  
  // ******************
  // Conteo tiempo
  // ******************
  t = millis(); // microsegundos despus de encendido
  // if (((t/cantidadMicrosegundos)%2)==0) {estadoGeneral=LOW;} else {estadoGeneral=HIGH;}
  
  // ******************
  // Pulso luz de freno
  // ******************
  if (digitalRead(pulsadorFreno) == HIGH) { // 1ª condicional
     delay(50);
        if (digitalRead(pulsadorFreno) == HIGH) { // 2ª condicional   
        estadoFreno=HIGH;
        } // 2ª condicional FIN 
  } else {
        estadoFreno=LOW;
  }// 1ª condicional FIN
  
  // Enciendo luz freno
   digitalWrite(luzFreno,estadoFreno);
  
  // *************************
  // Pulso luz LARGAS / CORTAS
  // *************************
  if (digitalRead(pulsadorLuces) == HIGH) { // 1ª condicional
     delay(150);
        if (digitalRead(pulsadorLuces) == HIGH) { // 2ª condicional   
        estadoLuces = estadoLuces + 1;
        } // 2ª condicional FIN 
  }// 1ª condicional FIN

  // desbordamiento
  if (estadoLuces>=3) { estadoLuces =0; } // condicional de desbordamiento
  
  if (estadoLuces==0) {
      digitalWrite(Largas,LOW);
      digitalWrite(Cortas,LOW);
  } else if (estadoLuces==1) {
      digitalWrite(Largas,LOW);
      digitalWrite(Cortas,HIGH);   
  } else if (estadoLuces==2) {
      digitalWrite(Largas,HIGH);
      digitalWrite(Cortas,HIGH);
  }
  
  // *************************
  // Intermitentes IZQUIERDOS
  // *************************
  if (digitalRead(pulsadorIzquierdo) == HIGH) { // 1ª condicional
     delay(200);
        if (digitalRead(pulsadorIzquierdo) == HIGH) { // 2ª condicional   
        difIIz = retardo * 1000; // diferencia entre los milisegundos actuales mas el retardo   
        tIIz = t + difIIz; // tiempo en el que acaba la secuencia 
        difIDr = 0; // anula el intermitente izquierdo   
        } // 2ª condicional FIN 
  } // 1ª condicional FIN
  // } else {
  //       estadoIIz = LOW;
  // } // 1ª condicional FIN

  if (difIIz>0) {
      estadoIIz = (difIIz/cadencia)%2;
      digitalWrite (intermitenteIzquierdo,estadoIIz);
      difIIz = tIIz - t; // voy calculando la diferencia
  }  else if (difIIz<=0) {
      difIIz = 0; // fuerza a cero
      digitalWrite (intermitenteIzquierdo,LOW); // apaga intermitente
  }   
  
  // *************************
  // Intermitentes DERECHOS
  // *************************
  if (digitalRead(pulsadorDerecho) == HIGH) { // 1ª condicional
     delay(200);
        if (digitalRead(pulsadorDerecho) == HIGH) { // 2ª condicional   
        difIDr = retardo * 1000; // diferencia entre los milisegundos actuales mas el retardo   
        tIDr = t + difIDr; // tiempo en el que acaba la secuencia    
        difIIz = 0; // anula el intermitente izquierdo
        } // 2ª condicional FIN 
  } // 1ª condicional FIN
  // } else {
  //       estadoIIz = LOW;
  // } // 1ª condicional FIN

  if (difIDr>0) {
      estadoIDr = (difIDr/cadencia)%2;
      digitalWrite (intermitenteDerecho,estadoIDr);
      difIDr = tIDr - t; // voy calculando la diferencia
  }  else if (difIDr<=0) {
      difIDr = 0; // fuerza a cero
      digitalWrite (intermitenteDerecho,LOW); // apaga intermitente
  } 
  
} // FIN DEL PROGRAMA


