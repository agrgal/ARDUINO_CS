const int BUTTON_PING =3;
const int LED_PING =4; 
int buttonState =0;
void setup() {
pinMode(LED_PING,OUTPUT);
pinMode(BUTTON_PING,INPUT);
}

void loop() { // Empieza el loop
digitalRead(BUTTON_PING);
  if(buttonState==HIGH) {
  digitalWrite(LED_PING, HIGH);
  }   else  {
  digitalWrite(LED_PING, LOW);
  }
}
