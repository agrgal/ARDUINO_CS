#define RXp2 3 // Rx de ESP 32 CAM
#define TXp2 1 // Tx de ESP 32 CAM

int valor = 0;

void setup() {
  // put your setup code here, to run once:
  // Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);

  pinMode(4,OUTPUT);
  
}

void loop() {

  if (Serial2.available() > 0) {
    // read the incoming byte:
    valor = Serial2.read();
    digitalWrite(4,valor);    
  }
  delay(100);    
}
