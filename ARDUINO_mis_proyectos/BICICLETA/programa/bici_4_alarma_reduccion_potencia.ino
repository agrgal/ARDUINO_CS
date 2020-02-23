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
int estadoAlarma = 0; // controla si la bici est en posicin o no de emergencia.
int estadoEmergencia = 0; // controla el parpadeo de emergencia

// variables temporales
unsigned long t = 0;
unsigned long retardo = 10; // segundos que dura el parpadeo de los intermitentes
unsigned long cadencia =200; // cadencia o frecuencia de parpadeo
unsigned long cadenciaEmergencia = 500; // frecuencia de parpadeo en el estado de emergencia
// tiempo intermitente izquierdo
unsigned long tIIz = 0;
unsigned long difIIz = 0;
// tiempo intermitente derecho
unsigned long tIDr = 0;
unsigned long difIDr = 0;
// parpadeo luces largas cortas
unsigned long cadenciaPotencia = 7;

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
        estadoAlarma = LOW; // desconecta el estado de alarma
        } // 2ª condicional FIN 
  } else {
        estadoFreno=LOW;
  }// 1ª condicional FIN
  
  // Enciendo luz freno
   digitalWrite(luzFreno,estadoFreno);
  
  // *************************
  // Pulso luz LARGAS / CORTAS
  // *************************
  if (digitalRead(pulsadorLuces) == HIGH && estadoAlarma==LOW) { // 1ª condicional
     delay(200);
        if (digitalRead(pulsadorLuces) == HIGH && estadoAlarma==LOW) { // 2ª condicional   
        estadoLuces = estadoLuces + 1;
        // estadoAlarma = LOW; // desconecta el estado de alarma; mejor se quita el estado de alarma pulsando el freno.
        } // 2ª condicional FIN 
  }// 1ª condicional FIN

  // desbordamiento
  if (estadoLuces>=3) { estadoLuces =0; } // condicional de desbordamiento
  
  if (estadoLuces==1 && (t/cadenciaPotencia)%2==HIGH ) {
      digitalWrite(Largas,LOW);
      digitalWrite(Cortas,HIGH);   
  } else if (estadoLuces==2 && (t/cadenciaPotencia)%2==HIGH)  {
      digitalWrite(Largas,HIGH);
      digitalWrite(Cortas,HIGH);
  } else {
      digitalWrite(Largas,LOW);
      digitalWrite(Cortas,LOW); 
  }
  
  // *************************
  // Intermitentes IZQUIERDOS
  // *************************
  if (digitalRead(pulsadorIzquierdo) == HIGH  && estadoAlarma==LOW ) { // 1ª condicional
     delay(100);
        if (digitalRead(pulsadorIzquierdo) == HIGH && estadoAlarma==LOW) { // 2ª condicional   
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
  if (digitalRead(pulsadorDerecho) == HIGH  && estadoAlarma==LOW) { // 1ª condicional
     delay(100);
        if (digitalRead(pulsadorDerecho) == HIGH && estadoAlarma==LOW) { // 2ª condicional   
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

  // ************************** FIN PROGRAMA PRINCIPAL ********************************************
  
  // *******************************
  // Luces de Emergencia
  // *******************************
  // Se encienden todas las luces (parpadeando) al pulsar los dos intermitentes a la vez
  if (digitalRead(pulsadorIzquierdo) == HIGH && digitalRead(pulsadorDerecho) == HIGH) { // 1ª condicional
     delay(300);
        if (digitalRead(pulsadorIzquierdo) == HIGH && digitalRead(pulsadorDerecho) == HIGH) { // 2ª condicional
            estadoAlarma = HIGH;
            difIIz=0; // anula intermitente izquierdo
            difIDr=0; // anula intermitente derecho
            estadoLuces = LOW; // apaga las luces
            estadoFreno = LOW; // anula el freno
        } // Fin del 2º condicional
  } // Fin del 1º condicional
      
  if (estadoAlarma) {
    estadoEmergencia = (t/cadenciaEmergencia)%2;
    digitalWrite(Largas,estadoEmergencia^HIGH);
    digitalWrite(Cortas,estadoEmergencia);
    digitalWrite(intermitenteDerecho,estadoEmergencia^HIGH);
    digitalWrite(intermitenteIzquierdo,estadoEmergencia^HIGH);
    digitalWrite(luzFreno,estadoEmergencia);
  }  // condiconal de Alarma
  
  //****************************************************************************************
  // En estado de Alarma, pulso Intermitente derecho para aumentar la cadencia de emergencia
  //****************************************************************************************
  if (digitalRead(pulsadorDerecho) == HIGH  && estadoAlarma==HIGH) { // 1ª condicional
     delay(50);
        if (digitalRead(pulsadorDerecho) == HIGH && estadoAlarma==HIGH) { // 2ª condicional   
        cadenciaEmergencia = (cadenciaEmergencia+50)*(cadenciaEmergencia<=1000)+1000*(cadenciaEmergencia>1000);
        } // 2ª condicional FIN 
  } // 1ª condicional FIN
  
  //****************************************************************************************
  // En estado de Alarma, pulso Intermitente izquierdo para disminuir la cadencia de emergencia
  //****************************************************************************************
  if (digitalRead(pulsadorIzquierdo) == HIGH  && estadoAlarma==HIGH) { // 1ª condicional
     delay(50);
        if (digitalRead(pulsadorIzquierdo) == HIGH && estadoAlarma==HIGH) { // 2ª condicional   
        cadenciaEmergencia = (cadenciaEmergencia-50)*(cadenciaEmergencia>50)+50*(cadenciaEmergencia<=50); // cadencia minima 50
        } // 2ª condicional FIN 
  } // 1ª condicional FIN
  
} // FIN DEL PROGRAMA


