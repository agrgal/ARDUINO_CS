#include <SPI.h>
#include <SD.h>

const int chipset = 4;

long tiempo = 0; // cuenta de tiempo...
int cada = 1000; // cada 1000ms o un segundo

File mifichero; // variable declarada como de tipo fichero.

String dato =""; // cadena vacía ocn los datos

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  // Inicializo tarjeta
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipset)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}


void loop() {

  tiempo = millis();

  int sensorValue = analogRead(A0);
  
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  
  dato = String(voltage);
  dato.replace('.', ','); // por si necesito la coma decimal y no el punto...
  
  // print out the value you read:
  Serial.println(dato);
  
  // Escribir valor en la tarjeta
  escribir(mifichero,dato);

  delay(cada);
} // fin del loop

// ***************************

int escribir(File fic, String valor) {
  fic = SD.open("temp.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (fic) {
    fic.println(valor);
    fic.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening temp.csv");
  }

}



