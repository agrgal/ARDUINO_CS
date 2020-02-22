
int salida =19;
int entrada = 7;

void setup() {
  pinMode(salida,OUTPUT);
  pinMode(entrada,INPUT);
}

void loop() {
  digitalWrite(salida,digitalRead(entrada));
  delay(15);
}
