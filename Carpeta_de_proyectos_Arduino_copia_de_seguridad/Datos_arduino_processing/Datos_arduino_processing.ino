int estadoBoton = LOW;
int pinBoton = 8;
int pinLED = 12;

void setup() {
  pinMode(pinBoton, INPUT);
  pinMode(pinLED, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {

  if (digitalRead(pinBoton)) {
      delay(200);
      if (digitalRead(pinBoton)) {
         estadoBoton = estadoBoton xor 1;
      }
  }

  digitalWrite(pinLED, estadoBoton);
  Serial.println(estadoBoton);
  delay(100);
  
}
