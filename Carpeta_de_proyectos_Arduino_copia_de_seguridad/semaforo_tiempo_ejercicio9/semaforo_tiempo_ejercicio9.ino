// defino una variable

unsigned long tiempo=0; // cuenta de tiempo
long temp=0; // ciclo del semaforo
int estado=0; // 0 --verde, 1 -- amarillo, 2 -- rojo

// ==========================
// programa principal ARDUINO
// ==========================

void setup()
{
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  Serial.begin(9600); // Envío de datos a la placa
}

void loop()
{

  tiempo = millis(); // milisegundos desde que se encendió la placa

  temp = tiempo%10000; // cuenta cíclica del tiempo. Resto de la división entre 10000
  // Cuando tiempo llegue a 10001, tempo volverá a ser cero.
  
  Serial.println(String(tiempo) +" -- " + String(temp)); // Imprime en pantalla datos de tiempo recibidos de la placa.  
  
  if (temp<=3000) { // enciendo el verde
    estado=0; // Verde
  } else if(temp>3000 && temp<=4000) { // enciendo amarillo
    estado=1; // Amarillo
  } else if (temp>4000) { // enciendo rojo
    estado=2;// Rojo 
  }

  // ejecuto la función encender
  encender();

  // delay(1); --> ANULADO // retraso de cada milisegundo ¡¡Ya no me hace falta!!
}

// ==================================
// funciones definidas por el usuario
// ==================================

void encender() {
  // como "estado" es una variable global, no es necesario pasarla a la función
  if (estado==0) {digitalWrite(8,HIGH); } else {digitalWrite(8,LOW); }
  if (estado==1) {digitalWrite(9,HIGH); } else {digitalWrite(9,LOW); }
  if (estado==2) {digitalWrite(10,HIGH); } else {digitalWrite(10,LOW); }
}
