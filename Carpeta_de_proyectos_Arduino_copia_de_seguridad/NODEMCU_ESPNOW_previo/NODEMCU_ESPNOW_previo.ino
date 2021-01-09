
int entrada = D0;
int salida = D1;

void setup() {
  pinMode(entrada,INPUT);
  pinMode(salida,OUTPUT);
}

void loop() {
  digitalWrite(salida,digitalRead(entrada));

}
