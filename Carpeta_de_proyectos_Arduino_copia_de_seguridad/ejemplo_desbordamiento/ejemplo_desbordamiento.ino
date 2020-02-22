//Pretend "counter" is "millis"
unsigned char counter = 0;
 
unsigned char previousCounter = 0;
unsigned int interval=47;
 
void setup() {
   Serial.begin(9600);
   delay(5000);  // give enough time to open the serial monitor after uploading
   Serial.println("Starting...");
}
void loop() {
   for (int x=0; x<1000; x++) { // run through uchar a few times
// ******* simulate millis()
       counter++; // simulate millis()
       Serial.println(counter);
// ******
       unsigned char currentCounter = counter;
       if ((unsigned char)(currentCounter - previousCounter) >= interval) { // check for rollover
         Serial.println("Trigger Event!");
         previousCounter = currentCounter;
      }
   }
Serial.println("...stopping!");
while(1); // Stop the Serial monitor output
}
