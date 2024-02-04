int sensorIR = 9; 
int LED = 13; 

void setup() {
  Serial.begin(115200); 
  pinMode(sensorIR, INPUT); 
  pinMode(LED, OUTPUT); 
}

void loop() {
  int estado = digitalRead(sensorIR); 
  if (estado == 1) 
  {
    digitalWrite(LED, LOW);
    Serial.println("Línea negra"); 
  }
  else  {
    digitalWrite(LED, HIGH); 
    Serial.println("Zona blanca"); 
  }
}
