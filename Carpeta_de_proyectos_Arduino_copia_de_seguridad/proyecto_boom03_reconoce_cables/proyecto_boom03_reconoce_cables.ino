
// Librerías necesarias para la pantalla
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define RESTART asm("jmp 0x0000");


// Activación de la pantalla LCD con I2C
LiquidCrystal_I2C lcd(0x27,16,2);  // dirección de la pantalla LCD, número de columnas, número de filas

// Variables
unsigned long tiempo=0; // temporizador
unsigned long tiempo2=0; // temporizador 2
int i,j=0; // contador proposito general
boolean juego=0; // estado del juego 0--> coloca solución 1--> jugando...

byte respuestas= 0b00001111;
byte respuestasTemp= 0b00001111;
byte cables=0b00001111;

int solucionElegida=0;
byte solucionElegidaByte=B0;

// *********
// Funciones
// *********

void(* resetFunc) (void) = 0; // esta es la funcion

String numberToBin(byte numero) {
  int j=0;  
  String resultado;
  char num[8];
  for (j=0;j<8;j++) {
    num[7-j] = char(48+numero%2);
    numero = numero / 2; 
    resultado=num[7-j]+resultado; 
  }
  return resultado;
}

void leerCables() { // Lee el estado de los cables y escribe la variable respuestasCable
    byte valor=B0;
    for (i=0;i<=3;i++) {
       valor = !digitalRead(i+2); // lee el cable en la posicion adecuada y lo convierte al complementario. Estamos actuando por lógica negativa
       valor = valor << i; //desplaza i posiciones 
       valor = ~valor; //calcula el complementario. Por ejemplo, de 00001000 --> 11110111
       cables = cables & valor; // aplica bitwise AND       
       // Serial.println("Posicion i=" + (String) (i+2) + " - cables: " + numberToBin(cables));
       // delay(10);
    }
}

void escribeRespuestas(byte resp) { // Lee el estado de los cables y escribe la variable respuestasCable
    byte izquierdaPORTB = B0; // Tengo que quedarme con el número de los 4 bits más significativos de PORTB
    // al llamar a esta funcion se le pasa la variable cables a respuestas
    // Conseguir solo los 4 bitsd de PORTB
    izquierdaPORTB = PORTB >> 4; // Los 4 bits a la derecha los pone en la izquierda
    izquierdaPORTB = izquierdaPORTB << 4; // Vuelve a ponerlo y rellena con ceros los 4 menos significativos
    PORTB = izquierdaPORTB | resp; // Hace la operacion OR.
    // Serial.println("PORTB=" + numberToBin(PORTB) + " - IZQUIERDA_PORTB=" + numberToBin(izquierdaPORTB) + " - respuestas: " + numberToBin(resp));
}

// *********
// SetUP
// *********
void setup()
{
  lcd.init();                     // inicializa la pantalla LCD
  lcd.backlight();                // enciende la retroiluminación de la pantalla 
  lcd.setCursor(0,0);             // ponemos el cursor en la columna 3 de la fila 0
  lcd.print("Empezamos el juego..."); // escribimos
  lcd.clear(); // limpia la pantalla
  Serial.begin(9600); // numeros del serial monitor

  // Asignacion de pines de salida y de entrada en el registro del Atmega8. 1-> Output y 0-> Input: PIN MODE
  DDRD = B00000000;  // digital pins 7,6,5,4,3,2,1,0
  DDRB = B00011111;  // digital pins -,-,13,12,11,10,9,8

  // Empezamos a cero en PORTB
  PORTB = PORTB= 0 | 1 << 4;

}

// *********
// Principal
// *********
void loop()
{

  tiempo = millis(); // tiempo transcurrido desde el inicio del programa
  
  if (digitalRead(13)) { juego=1; } // Simplemente pulsando el boton de inicio del juego, se activa

  if (juego) { // Comprueba el estado del juego

    // A) escribe en la pantalla la solución del juego
    lcd.setCursor(0,0);             
    lcd.print("Juego en marcha!"); 
    lcd.setCursor(0,1);
    lcd.print("               "); // Borra la linea
    lcd.setCursor(0,1);
    lcd.print("Solucion en " + (String) char(solucionElegida+65));

    // B) Leer cables
    leerCables(); // Se leen los cables 
    // Ver cómo puede parpadear cuando quita uno de los cables... Quizás con otra variable tiempo + algo
    // si cables es distinto de respuestas ...
    if (respuestas!=cables) { 
      tiempo2 = tiempo + 4000; // Le sumo 4000 milisegundos 
      respuestasTemp = respuestas; // variable temporal
      respuestas=cables; // Asigno a respuestas la variable cables
      // Serial.println("tiempo2=" + (String) tiempo + " - tiempo2= "+ (String) tiempo2);
    }

    // C) Durante un tiempo puede parpadear hasta que se apaga
    if (tiempo<tiempo2) { // mientras tiempo no alcance a tiempo 2
      // hacer algo para que parpadee 
      if (((tiempo2-tiempo)/200)%2) {
         PORTB= respuestasTemp | 1 << 4; 
         // Serial.println("respuestas --> PORTB=" + numberToBin(PORTB));
      } else {
         PORTB= cables | 1 << 4;
         // Serial.println("Cables --> PORTB=" + numberToBin(PORTB));
      }      
    } else {   
      escribeRespuestas(respuestas); // Se escriben las respuestas
   
          // D) Se pierde o se gana. Lo pongo aquí para que parpadee...
          if (solucionElegidaByte==respuestas) { // Detectar ganador
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Has ganado!!"); 
            lcd.setCursor(0,1);
            lcd.print("Restaura cables"); 
              while (!digitalRead(13)) {
                PORTB= 15 | 1 << 4;
                delay(100);
                PORTB= 0 | 1 << 4;
                delay(100);
              }
              RESTART;
          } // Se gana
      
          if ((solucionElegidaByte & respuestas)==0) { 
            // La respuesta con un AND a solución elegida siempre da cero excepto en el bit de la respuesta.
            // A menos que la respuesta se haya anulado, por lo que el resultado sería 0
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Has perdido!!"); 
            lcd.setCursor(0,1);
            lcd.print("Restaura cables"); 
              while (!digitalRead(13)) {
                for (i=0;i<=3;i++) {
                    if (j==0) {PORTB = 1 << i | 1 << 4;} else {PORTB = 8 >> i | 1 << 4;}
                    delay(100);
                }
                j=(j+1)%2;
              }
              RESTART;
          } // Así se pierde
    }


     
  } else { // Condicional sobre la variable juego
    lcd.setCursor(0,0);             
    lcd.print("Elige una solucion...");  
    lcd.setCursor(0,1);
    lcd.print("Solucion n:");
    solucionElegida = map(analogRead(A0),0,1024,0,4);
    lcd.setCursor(13,1);
    lcd.print(char(solucionElegida+65));
    solucionElegidaByte = 1 << solucionElegida; // desplaza a la izquierda el 1 las posiciones elegidas
    // Serial.println("Solucion Elegida=" + numberToBin(solucionElegidaByte));
  } // Fin del IF sobre la variable juego

  // parpadeo estado de juego pin 12
  digitalWrite(12,(tiempo/500)%(2-juego*2)); // parpadea mientras pone la solucion; fija cuando estamos en juego.


  
} // Fin del Loop
