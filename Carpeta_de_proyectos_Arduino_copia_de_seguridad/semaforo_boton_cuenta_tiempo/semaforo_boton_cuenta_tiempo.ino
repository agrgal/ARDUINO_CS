// defino una variable

unsigned long tiempo=0; // cuenta de tiempo
unsigned long temp=0; // ciclo del semaforo

long offset=0; // valor de temporizador offset
int estado=0; // 0 --verde, 1 -- amarillo, 2 -- rojo

int i; // contador de proposito general para BUCLES.

// ==========================
// programa principal ARDUINO
// ==========================

void setup()
{
  pinMode(8, OUTPUT); // Verde 8 segundos
  pinMode(9, OUTPUT); // Amarillo durante 2 segundos
  pinMode(10, OUTPUT); // Rojo, durante 8 segundos
  pinMode(6, OUTPUT); // Verde peatones
  pinMode(7, OUTPUT); // Rojo peatones
  pinMode(13, OUTPUT);
  pinMode(4,INPUT); // pulsador peatones 
  Serial.begin(9600); // Envío de datos a la placa
}

void loop()
{

  tiempo = millis(); // milisegundos desde que se encendió la placa 

  temp = tiempo%8500; // cuenta cíclica del tiempo. Resto de la división entre 18000  
  // Cuando tiempo llegue a 8501, tempo volverá a ser cero.

  // condicional numérica
  estado=((temp+offset)>3000 && (temp+offset)<=4500)+2*((temp+offset)>4500); // paréntesis ciertos valen 1 y los falsos 0
 
  
 // Pulsando el botón 
 if (digitalRead(4)==HIGH && estado==0) { // si pulso el botón y está en verde
      delay(25); // Evitar el bouncing del Botón
      if (digitalRead(4)==HIGH) {
        digitalWrite(13,HIGH); // Enciendo lED de la placa 
        Serial.println("Pulsado");
        offset=3000-(temp-25);
        Serial.println("OFFSET: --"+ String(offset));
        Serial.println("==========================");  
        Serial.println("tiempo: "+ (String) tiempo + " -- temp: " + (String) temp);
        }
  } 
  
  if (offset>0 && (temp+offset)>=8500) {
     offset=0;  
     digitalWrite(13,LOW);
     Serial.println("tiempo: "+ (String) tiempo + " -- temp: " + (String) temp);
  } // cuando pasa del estado 2 al 0, el parámetro offset debe volver a cero.     
  
  // ejecuto la función encender
  encender();

  // delay(100); // --> ANULADO // retraso de cada milisegundo ¡¡Ya no me hace falta!!
 
}

// ==================================
// funciones definidas por el usuario
// ==================================

void encender() {
  digitalWrite(8,(estado==0)); // Verde
  digitalWrite(9,(estado==1)*((temp+offset)/250)%2); // 
  digitalWrite(10,(estado==2)); // Rojo
  digitalWrite(6,(estado==2)); // Verde peatones
  digitalWrite(7,(estado<=1)); // Rojo peatones  
}
