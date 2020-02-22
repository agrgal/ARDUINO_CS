int intIz=3;
int intDr=5;
int frIz=9;
int frDr=11;

int tiempoIntermitencia = 200;

int estado=0;

void setup() {
  // Inicializa las salidas
  pinMode(intIz,OUTPUT);
  pinMode(intDr,OUTPUT);
  pinMode(frIz,OUTPUT);
  pinMode(frDr,OUTPUT);
  Serial.begin(9600);
  randomSeed(analogRead(A0)); // Semilla aleatoria
}

// the loop function runs over and over again forever
void loop() {
  delay(50);
  estado=map(analogRead(A0),0,1023,1,4);
  delay(50);
    
  Serial.println(estado);
 
  switch (estado) {
      case 1:
         // estado1: intermitente 1, intermitente 2 y frena
        noFrenar(); 
        Intermitente(random(1,5)+random(1,5),intIz,tiempoIntermitencia);
        Intermitente(random(1,5)+random(1,5),intDr,tiempoIntermitencia);  
        Frenar();  
        delay(random(500,1000)+random(500,1000));
        break;
      case 2:
        //do something when var equals 2
        break;
      default:
          IntermitentesEncendidos();
          Frenar();
      break;
    }
  
          
}

void Frenar() {
  digitalWrite(frIz, HIGH); 
  digitalWrite(frDr, HIGH); 
}

void noFrenar() {
  digitalWrite(frIz, LOW); 
  digitalWrite(frDr, LOW); 
}

// Función que pone en marcha un intermitente
void Intermitente(int veces, int cual, int tiempo) {
  int i;
  for (i=0;i<veces;i++) {
    digitalWrite(cual, HIGH);   
    delay(tiempo);                       
    digitalWrite(cual, LOW);    
    delay(tiempo);
  }
}

void IntermitentesEncendidos(){
    digitalWrite(intDr, HIGH); 
    digitalWrite(intIz, HIGH); 
}

void IntermitentesApagados(){
    digitalWrite(intDr, LOW); 
    digitalWrite(intIz, LOW); 
}



