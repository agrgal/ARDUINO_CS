// this sample code provided by www.programmingboss.com

int ledPin=13;
bool estado=LOW;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin,OUTPUT);
}
void loop() {
  Serial.write(estado);
  delay(1500);
  estado = !estado;
  digitalWrite(ledPin,estado);
  Serial.flush();
}
