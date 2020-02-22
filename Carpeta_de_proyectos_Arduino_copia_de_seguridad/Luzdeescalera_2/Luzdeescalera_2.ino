#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12); //    ( RS, EN, d4, d5, d6, d7)

unsigned long tempo = 0; // Variable que registra el tiempo de corte
unsigned long tiempo = 0; // Variable que registra el tiempo

unsigned long lapso = 0; // Tiempo que dura las luces encendidas
unsigned long maxTiempo = 0xFFFFFFFF; // 32 bits, que corresponde a 49 días...
unsigned long maxTiempo2 = 0xFFFFA000; // 32 bits, que corresponde a casi 49 días...

long diferencia = 0; // para que tenga valores positivos y negativos variable LONG.

int pinSalida=13; // Donde emite la señal
int pinPulsador = 6; // Pulsador de activación

int activado = 0; // señal de activación
int estadoBotonActual = 0; // estado de los botones
int estadoBotonPrevio = 0;

// *****
// SETUP
// *****
void setup()  
   {
       pinMode(pinSalida,OUTPUT);
       pinMode(pinPulsador,INPUT);
       lcd.begin(16, 2); // Fijar el numero de caracteres y de filas
       lcd.print("Luz de escalera"); // Enviar el mensaje
      Serial.begin(9600);
   }

// *****
// LOOP
// *****
void loop() 
   {
       // tiempo = millis()+maxTiempo2; // Señal de tiempo. Suma maxTiempo2 (desbordamiento);
       tiempo = millis(); // Señal de tiempo
       
       // tiempo de encendido de la luz según el valor del potenciómetro
       lapso = map(analogRead(A0),0,1023,0,34); // Pongo uno más de lo necesario porque es casi
       // imposible que alcance 1023, y por tanto el valor 33 que correspondería a 6 min.
       lapso = lapso * 10 + 30; // empezando en 30, de 10 en 10 segundos. Ultimo a 360 s, 6 minutos
  
      // Pulsando el botón pinPulsador
      estadoBotonActual = digitalRead(pinPulsador); // Leer estado del botón
      
      // Información que se manda por el puerto Serie.
      Serial.print("¿Activado?: ");
      Serial.print(activado);
      Serial.print(" - tiempo: ");
      Serial.print(tiempo); // tiempo del contador
      Serial.print(" - tempo: ");
      Serial.print(tempo); // tiempo de pulsacion
      Serial.print(" - Diferencia: ");
      Serial.println(diferencia); // tiempo de pulsacion
  
      if (estadoBotonActual==HIGH && estadoBotonPrevio==LOW && activado == LOW) { // si pasa de bajo a alto --> flanco de subida
         activado = HIGH;
         tempo = tiempo + lapso*1000; // Cambiar a lapso * 1000, para segundos  
         diferencia = calculaDiferencia(tempo,tiempo);
         // Información al pulsar
         Serial.println( (String) lapso + " - tempo:"+ (String) tempo +   " - tiempo:"+ (String) tiempo );   
      } 
      estadoBotonPrevio=estadoBotonActual; 

      // Mientras tempo sea mayor que tiempo y activado esté activo
      if (diferencia>0 && activado==HIGH) {
            digitalWrite(pinSalida,HIGH); // escribe valor alto en la salida
            lcd.setCursor(0, 1);  // set the cursor to column 0, line 1
            lcd.print("                ");  // vaciar contenido
            lcd.setCursor(0, 1);  // set the cursor to column 0, line 1
            // lcd.print((String) ((tempo-tiempo)/1000) + " s");  // Segundos que faltan                      
            lcd.print(convertirAminseg(1+diferencia/1000));
            diferencia = calculaDiferencia(tempo,tiempo);
        } else {
          digitalWrite(pinSalida,LOW); // escribe valor bajo en la salida
            activado = LOW; // me aseguro que este flag está desactivado
            tempo = 0; // me aseguro que es cero
            diferencia = 0; // me aseguro que es cero
            lcd.setCursor(0, 1);  // set the cursor to column 0, line 1
            lcd.print("                ");  // vaciar contenido
            lcd.setCursor(0, 1);  // set the cursor to column 0, line 1
            lcd.print("Activado: " + convertirAminseg(lapso));  // Segundos de encendido                 
        }
  
       delay(5); // Pequeño retraso      
   }

   
   // Functions
   String convertirAminseg (unsigned long valor ) {
      String segPrev= "";
      int minutos = 0;
      int segundos = 0;    
      minutos = valor/60;
      segundos = valor % 60;
       if (segundos<10) { segPrev="0"; } else { segPrev=""; }
      return (String) minutos + "'"+ segPrev + (String) segundos + '"';
   }

   // Calcula la diferencia temporal
   long calculaDiferencia( unsigned long tempo2, unsigned long tiempo2) {
   long difer=0;
    if (tempo2>tiempo2) {
      difer = tempo2 - tiempo2; // calcula diferencia normalmente
    } else {
      difer = maxTiempo-tiempo2+tempo2; // el máximo tiempo - tiempo + tempo. En caso de próximo a desbordamiento
    }
    return difer;
   }
   

