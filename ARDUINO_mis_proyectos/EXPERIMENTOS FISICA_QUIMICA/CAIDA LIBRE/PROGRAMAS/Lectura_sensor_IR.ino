long tiempo = 0;
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  tiempo = micros();
  if (digitalRead(7)==HIGH) {
    // read the input on analog pin 0:
    int sensorValue = analogRead(A0);
    Serial.println((String) tiempo + " - " + (String) sensorValue);
    delayMicroseconds(1);
   }
}
