long tiempo = 0;
int maximo = 75;

void setup() {
  pinMode(3,OUTPUT); // Normal
  pinMode(5,OUTPUT); // Doble
}

void loop() {
  tiempo = millis()/50;
  analogWrite(3,tiempo%maximo);
  analogWrite(5,(tiempo%maximo)*((tiempo/maximo)%2==0)+(maximo-(tiempo%maximo))*((tiempo/maximo)%2==1));
}
