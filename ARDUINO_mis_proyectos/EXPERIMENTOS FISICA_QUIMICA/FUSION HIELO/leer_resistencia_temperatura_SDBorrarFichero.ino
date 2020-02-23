// El experimento tiene conectada una NTC al pin A0 para leer datos analógicos
// Resistencia de 5K para hacer el divisor. NTC conectada a 5V y 5k a tierra.
// 5K conseguida con dos en paralelo de 10k
// LED rojo conectado al pin 8


// bibliotecas necesarias con la tarjeta
#include <SPI.h>
#include <SD.h>

// La placa ethernet NECESITA comunicarse con este PIN. No usuarlo en el experimento. Tb creo que no usar el 11. LEER
const int chipset = 4;

// VAriables temporales
long tiempo = 0; // cuenta de tiempo...
int cada = 15000; // cada 1000ms o un segundo

File mifichero; // variable declarada como de tipo fichero.

String dato =""; // cadena vacía ocn los datos
String nomFichero = "temp.csv"; // tiene que ser del tipo 8.3

void setup() {

  // En el pin 8, coloco un LED
  pinMode(8,OUTPUT);
  
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

  // Si el fichero existe, lo borra
  // Así lo puedo usar indefinidamente con el mismo nombre
  if (SD.exists(nomFichero)) {
    Serial.println("El fichero "+nomFichero+" ya existe. Lo borro.");
    SD.remove(nomFichero);
  }
  
}


void loop() {

  // cuenta del tiempo
  tiempo = millis();

  // lectura del puerto analógico A0
  int sensorValue = analogRead(A0);
  
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);

  // Cadena con los datos...
  dato = String(tiempo)+"; "+String(voltage);
  dato.replace('.', ','); // por si necesito la coma decimal y no el punto...
  
  // print out the value you read:
  Serial.println(dato);
  
  // Escribir valor en la tarjeta
  escribir(mifichero,dato);

  delay(cada); // retraso hasta la próxima lectura
  digitalWrite(8,LOW); // aseguro que el LED esté apagado
  
} // fin del loop



// ***************************
// Funciones
// ***************************

// F1) Función que escribe un dato en la tarjeta
int escribir(File fic, String valor) {
  
  // Abre el fichero en modo de escritura
  fic = SD.open(nomFichero, FILE_WRITE);

  // if the file is available, write to it:
  if (fic) {
    fic.println(valor);
    fic.close();
    // Encendido del LED
    digitalWrite(8,HIGH);
    delay(200);
    digitalWrite(8,LOW);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening temp.csv");
  }

}



