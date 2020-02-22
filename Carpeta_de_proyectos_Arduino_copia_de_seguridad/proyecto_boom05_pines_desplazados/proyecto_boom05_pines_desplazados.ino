
// Librerías necesarias para la pantalla
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define RESTART asm("jmp 0x0000"); //permite el RESET automático de la placa

// Activación de la pantalla LCD con I2C
LiquidCrystal_I2C lcd(0x27,16,2);  // dirección de la pantalla LCD, número de columnas, número de filas

// Variables
unsigned long tiempo=0; // temporizador
unsigned long tiempo2=0; // temporizador 2
int i,j=0; // contador proposito general
boolean juego=0; // estado del juego 0--> coloca solución 1--> jugando...

byte respuestas= 0b00001111; // En conexiones 8, 9, 10 y 11
byte respuestasTemp= 0b00001111;
byte cables=0b00001111; // En conexiones 2,3,4,5

int solucionElegida=0;
byte solucionElegidaByte=B0;

// *********
// Funciones
// *********

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
       valor = !digitalRead(i); // lee el cable en la posicion adecuada y lo convierte al complementario. Estamos actuando por lógica negativa
       valor = valor << i; //desplaza i posiciones 
       valor = ~valor; //calcula el complementario. Por ejemplo, de 00001000 --> 11110111
       cables = cables & valor; // aplica bitwise AND       
       // Serial.println("Posicion i=" + (String) (i+2) + " - cables: " + numberToBin(cables));
       // delay(10);
    }
}

void escribeRespuestas(byte resp) { // Lee el estado de los cables y escribe la variable respuestasCable
    byte derechaPORTD = B0; // Tengo que quedarme con el número de los 4 bits más significativos de PORTB
    // al llamar a esta funcion se le pasa la variable cables a respuestas
    // Conseguir solo los 4 bitsd de PORTB
    // derechaPORTD = PORTB >> 4; // Los 4 bits a la derecha los pone en la izquierda
    // derechaPORTD = derechaPORTD << 4; // Vuelve a ponerlo y rellena con ceros los 4 menos significativos
    // PORTD = derechaPORTD | resp; // Hace la operacion OR.
    PORTD = resp << 4; // ¿Simplemente desplaza la respuesta 4 bits a la izquierda?
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
  DDRD = B11110000;  // digital pins 7,6,5,4,3,2,1,0 --> los 4 primeros son entradas --> los otro 4, salidas
  DDRB = B00001001;  // digital pins -,-,13,12,11,10,9,8 --> el 8 es la salida a led de control (antiguo 12), el 9 entrada (pulsador), 11 salida (altavoz) y 13 tarjeta SD

  // Empezamos a cero en PORTD
  PORTD = 1; // ponemos todo a "1"
  PORTB = 0;

}

// *********
// Principal
// *********
void loop()
{

  tiempo = millis(); // tiempo transcurrido desde el inicio del programa

  leerCables();
 
  if (digitalRead(9) && cables==0B00001111) { juego=1; }// Simplemente pulsando el boton de inicio del juego, se activa
  // Y si los cables están todos en su sitio. Variable cables correcta
  
  if (cables!=0B00001111 && juego==0){ // Cuidado! si no están los cables puestos... y aún no ha empezado el juego, que te avise... Seguridad 
    lcd.clear();
    lcd.setCursor(0,0);             
    lcd.print("Pon los cables!!"); 
    delay(500);
    lcd.clear();
    cables=0B00001111; // restauro el inicio de la variable cables.
  }

  if (juego) { // Comprueba el estado del juego

    // A) escribe en la pantalla la solución del juego para informar a quien lleva el juego.
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
      // ¿Tocar algo de continuidad?
    }

    // C) Durante un tiempo puede parpadear hasta que se apaga
    if (tiempo<tiempo2) { // mientras tiempo no alcance a tiempo 2
      // hacer algo para que parpadee 
      if (((tiempo2-tiempo)/200)%2) {
         PORTD= respuestasTemp << 4 | cables; 
         // Serial.println("respuestas --> PORTB=" + numberToBin(PORTB));
      } else {
         PORTD= cables | cables << 4;
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
              while (!digitalRead(9)) {
                PORTD= 15 << 4;
                delay(100);
                PORTD= 0 << 4;
                delay(100);
              }
              RESTART;
          } else if ((solucionElegidaByte & respuestas)==0) { // Detecta si se pierde
            // La respuesta con un AND a solución elegida siempre da cero excepto en el bit de la respuesta.
            // A menos que la respuesta se haya anulado, por lo que el resultado sería 0
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Has perdido!!"); 
            lcd.setCursor(0,1);
            lcd.print("Restaura cables"); 
            PORTD = 0B00001111;
              while (!digitalRead(9)) {
                for (i=0;i<=3;i++) {
                    if (j==0) {PORTD = 16 << i | 15 ;} else {PORTD = 127 >> i | 15;}
                    delay(100);
                }
                j=(j+1)%2;
              }
              RESTART;
          }
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
  digitalWrite(8,(tiempo/500)%(2-juego*2)); // parpadea mientras pone la solucion; fija cuando estamos en juego.

  Serial.println("PORTD: " + numberToBin(PORTD));
  
} // Fin del Loop
