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
     delay(250);
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
        estadoIIz = HIGH;
        } // 2ª condicional FIN 
  } else {
        estadoIIz = LOW;
  }// 1ª condicional FIN
  
  digitalWrite (intermitenteIzquierdo,estadoIIz);
  
  // *************************
  // Intermitentes DERECHOS
  // *************************
  if (digitalRead(pulsadorDerecho) == HIGH) { // 1ª condicional
     delay(200);
        if (digitalRead(pulsadorDerecho) == HIGH) { // 2ª condicional   
        estadoIDr = HIGH;
        } // 2ª condicional FIN 
  } else {
        estadoIDr = LOW;
  }// 1ª condicional FIN
  
  digitalWrite (intermitenteDerecho,estadoIDr);
  
} // FIN DEL PROGRAMA


