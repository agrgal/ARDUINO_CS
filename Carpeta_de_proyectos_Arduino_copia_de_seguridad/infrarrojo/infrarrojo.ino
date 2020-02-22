#include <IRremote.h>
 
int RECV_PIN = 11;
int LED = 8;
 
IRrecv irrecv(RECV_PIN);
 
decode_results results;

long trigger = 0;
 
void setup()
{
  Serial.begin(9600);
  pinMode(LED,OUTPUT);
  irrecv.enableIRIn(); // Start the receiver
}
void loop() {
  if (irrecv.decode(&results)) {
    Serial.print(results.value,DEC);
    trigger = results.value;
    Serial.print(" - ");
    Serial.println(trigger);
    irrecv.resume(); // Receive the next value    
  }

  if (trigger==-516040152) {
    digitalWrite(LED,HIGH);
  } else {
    digitalWrite(LED,LOW);    
  }
  
}
