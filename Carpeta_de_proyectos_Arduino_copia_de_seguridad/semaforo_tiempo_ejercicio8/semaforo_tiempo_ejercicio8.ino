// defino una variable

int tiempo=0;

// programa ARDUINO

void setup()
{
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  Serial.begin(9600); // Envío de datos a la placa
}

void loop()
{
  
  Serial.println(tiempo); // Imprime en pantalla datos de tiempo recibidos de la placa.
  
  if (tiempo>=7000) { // reinicia el contador de tiempo si supera los 7 segundos
   tiempo=0; 
  }
  
  tiempo = tiempo+1; // Sumo uno a la variable tiempo
  
  
  if (tiempo<=3000) { // enciendo el verde
    digitalWrite(8,HIGH);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW); // apago amarillo y rojo
  } else if(tiempo>3000 && tiempo<=4000) { // enciendo amarillo
    digitalWrite(8,LOW);
    digitalWrite(9,HIGH);
    digitalWrite(10,LOW); // apago verde y rojo   
  } else if (tiempo>4000) { // enciendo rojo
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,HIGH); // apago verde y amarillo  
  }

  delay(1); // retraso de cada milisegundo
}

