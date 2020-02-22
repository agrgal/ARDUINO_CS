// defino una variable

unsigned long tiempo=0; // cuenta de tiempo
unsigned long temp=0; // ciclo del semaforo
unsigned long offset=0; // valor de temporizador offset
int estado=0; // 0 --verde, 1 -- amarillo, 2 -- rojo
int estadoAnterior=0; // al empezar el loop el estado que teníaen el loop anterior
int i; // contador de proposito general para BUCLES.

// ==========================
// programa principal ARDUINO
// ==========================

void setup()
{
  pinMode(8, OUTPUT); // Verde 8 segundos
  pinMode(9, OUTPUT); // Amarillo durante 2 segundos
  pinMode(10, OUTPUT); // Rojo, durante 8 segundos
  pinMode(13, OUTPUT);
  pinMode(4,INPUT); // pulsador peatones 
  Serial.begin(9600); // Envío de datos a la placa
}

void loop()
{

  tiempo = millis(); // milisegundos desde que se encendió la placa 

  temp = tiempo%18000; // cuenta cíclica del tiempo. Resto de la división entre 18000  
  // Cuando tiempo llegue a 18001, tempo volverá a ser cero.

  // condicional numérica
  if (digitalRead(4)==HIGH && estado==0) { // si pulso el botón y está en verde
      digitalWrite(13,HIGH); // Enciendo lED de la placa 
      delay(25);
        if (digitalRead(4)==HIGH) {
          Serial.println("Pulsado");
          offset=8000-temp;
          Serial.println("OFFSET: --"+ String(offset));
          Serial.println("======================");
          digitalWrite(13,LOW);
        }
  } 
  
  estado=(temp+offset>8000 && temp+offset<=10000)+2*(temp+offset>10000); // paréntesis ciertos valen 1 y los falsos 0
  if (estado==0 && estadoAnterior==2) {
     offset=0;   
  } // cuando pasa del estado 2 al 0, el parámetro offset debe volver a cero.     
  
  // ejecuto la función encender
  encender();

  // delay(100); // --> ANULADO // retraso de cada milisegundo ¡¡Ya no me hace falta!!
 
  estadoAnterior = estado; // calcula el estado al terminar el proceso, que será el anterior para el siguiente
}

// ==================================
// funciones definidas por el usuario
// ==================================

void encender() {
  // como "estado" e "i" es una variable global, no es necesario pasarla a la función
  for (i=0;i<=2;i++) {
     if(i==estado && i!=1) {digitalWrite(8+i,HIGH); } else {digitalWrite(8+i,LOW);} 
     if(estado==1 && i==1) {digitalWrite(9,((temp+offset)/250)%2);} 
  }
}



