// defino una variable

unsigned long tiempo=0; // cuenta de tiempo
long temp=0; // ciclo del semaforo
int estado=0; // 0 --verde, 1 -- amarillo, 2 -- rojo
int i; // contador de proposito general para BUCLES.

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

  // condicional numérica
  estado=(temp>3000 && temp<=4000)+2*(temp>4000); // paréntesis ciertos valen 1 y los falsos 0
  
  // ejecuto la función encender
  encender();

  // delay(1); --> ANULADO // retraso de cada milisegundo ¡¡Ya no me hace falta!!
}

// ==================================
// funciones definidas por el usuario
// ==================================

void encender() {
  // como "estado" e "i" es una variable global, no es necesario pasarla a la función
  for (i=0;i<=2;i++) {
     // if(i==estado) {digitalWrite(8+i,HIGH);} else {digitalWrite(8+i,LOW);} --> Esto funciona, pero mejor lo siguiente...
     digitalWrite(8+i,(i==estado));
  }
}
