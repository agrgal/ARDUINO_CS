// proyecto semáforo acústico
// Consiste en detectar el nivel de ruido y emitir una luz de colores
// actuando como un semáforo

long valor = 0; // valor de entrada del potenciómetro analógico
int pinElegido[]={3,5,6}; // valores de los pins
float colorIntermedio[]={10.0,100.0,100.0}; // colores que se alcanzan en el valor intermedio (1) RGB
int numPin = 2;
int ajustePIN = A0;

const int sensitivity = -58;     // microphone sensitivity in dB
const int gain = 20;             // op-amp gain dB
const int sensorPIN = A5;
const int sampleWindow = 200;  // sample for 200 milliseconds
unsigned int sample;

void setup() {
  pinMode(3,OUTPUT); // salidas analógicas
  pinMode(5,OUTPUT); // salidas analógicas
  pinMode(6,OUTPUT); // salidas analógicas
  Serial.begin(9600);

}

void loop() {

   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;         // peak-to-peak level
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;       // 10 bit ADC = 2^10

   // collect data for Sample window width in mS 
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(sensorPIN);

      if (sample < 1024)  // toss out spurious readings
      {
         // see if you have a new maxValue
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;               // max - min = peak-peak amplitude
   
   double volts = ((peakToPeak * 5.0) / 1024)*10;    // convert to volts,gain 20db  = 10V
                                                     // Uno/Mega analogRead() voltages between 0 and 5 volts into integer values between 0 and 1023
                                                     // change if have different board

                                                        
      // https://electronics.stackexchange.com/questions/96205/how-to-convert-volts-in-db-spl
      // The microphone sensitivity is -58db, so V RMS / PA is 0.001259
      // conversion using : http://www.sengpielaudio.com/calculator-gainloss.htm
   double volts_db = 20*log10(volts/0.001259); 
                                         
    
     //  94 is a 1 Pa expressed as dB SPL  
     // https://support.biamp.com/General/Audio/Microphone_sensitivity     
     // https://forums.adafruit.com/download/file.php?id=38278&sid=3f8400ec828f5b22ed3be3ef64eb4e86                             
   double spl_db = volts_db + 94 + sensitivity - gain;      
  
  encender(spl_db); // devuelve un número entre 0 y 2 con la situación del valor

  // Output results
  /*
   Serial.print("volts: " + String(volts) );
   Serial.print("\t");                        // prints a tab
   Serial.print("dB: " + String(volts_db) );
   Serial.print("\t");
   Serial.println("dB-spl: " + String(spl_db) ); */

}

// *********
// Funciones
// *********

void encender(long dato) {

  int ajuste = analogRead(ajustePIN); // valor del potenciómetro de ajuste
  ajuste = map(ajuste,0,1023,50,300); // Valores de decibelios considerados como máximo.

  int cual = map(dato,0,ajuste,0,10); // Elijo cual es el que me corresponde... Pero a lo mejor no hace falta
  float x = 2.0-((dato>=0 and dato<=ajuste)*(2.0*dato/ajuste)+(dato>ajuste)*2.0); // Cuando dato = 0, entonces x=2 y x va bajando...
  
  // Funcion parabólica para el rojo, en el rango de x entre 0 y 2
  float red = (127.5-colorIntermedio[0])*x*x+(2*colorIntermedio[0]-382.5)*x+255.0; // esta función calcula los valores de rojo, pasando por los colores 255 en 0,colorIntermedio[0] en 1 y 0 en 2
  red=red*(red>=0.0); // corta los valores negativos y los pone a cero

  // **********************************
  // Función exponencial para el verde
  // **********************************
  float Agreen= (colorIntermedio[1]*colorIntermedio[1])/(2.0*colorIntermedio[1]-255.0);
  float Cgreen = log(1.0-colorIntermedio[1]/Agreen);
  // Serial.print(Agreen,6);   Serial.print(" -- " );   Serial.print(Cgreen,6);
  float green = Agreen*(1-exp(Cgreen*x));  
  // float green = (127.5-colorIntermedio[1])*x*x+(2*colorIntermedio[1]-127.5)*x+0.0; // esta función calcula los valores de verde, pasando por los colores 0 en 0,colorIntermedio[1] en 1 y 0 en 2
  // green=green*(green>=0.0); // corta los valores negativos y los pone a cero
  
  float blue = -colorIntermedio[2]*x*x+2*colorIntermedio[2]*x; // esta función calcula los valores de azul, pasando por los colores 0 en 0,50 en 1 y 0 en 2
  blue=blue*(blue>=0.0); // corta los valores negativos y los pone a cero
  
  analogWrite(pinElegido[0],(int) red); // Escribo en el pin primero el valor de los rojos. Valor absoluto corta los valores negativos
  analogWrite(pinElegido[1],(int) green); // Escribo en el pin primero el valor de los rojos
  analogWrite(pinElegido[2],(int) blue); // Escribo en el pin primero el valor de los rojos
  /* 
  for (int i=0;i<=2;i++) {
    analogWrite(pinElegido[i],colores[cual][i]); //en cada pin Red 3, Green 5, Blue 6 se escribe los valores de "cual" he escogido
  } */
  // Serial.println((String) cual +" : "+ (String) colores[cual][0] +":" +(String) colores[cual][1] +":"+ (String) colores[cual][2] ) ;
  Serial.println("Ajuste: "+ (String) ajuste + "  DB: "+(String) dato +" --> "+ (String) x +" : RGB("+ (String) red +"," +(String) green +","+ (String) blue + ") cualitativo: " + (String) cual ) ;
}


