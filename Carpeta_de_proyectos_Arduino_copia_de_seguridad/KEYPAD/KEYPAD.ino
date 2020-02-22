/* @file HelloKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #
*/
#include <Keypad.h>


#include <IRremote.h>
IRsend irsend;

const byte TAMANO_CHAR = 6;

char BOTON_ON_OFF[TAMANO_CHAR] = "C40BF";
char BOTON_FREEZE[TAMANO_CHAR] = "CC03F";
char BOTON_MENU[TAMANO_CHAR] = "CF00F";
char BOTON_ARRIBA[TAMANO_CHAR] = "CD02F";
char BOTON_AUTO[TAMANO_CHAR] = "C10EF";
char BOTON_IZQUIERDA[TAMANO_CHAR] = "CB04F";
char BOTON_MODE[TAMANO_CHAR] = "C08F7";
char BOTON_DERECHA[TAMANO_CHAR] = "C708F";
char BOTON_BLANK[TAMANO_CHAR] = "CE01F";
char BOTON_ABAJO[TAMANO_CHAR] = "C30CF";
char BOTON_SOURCE[TAMANO_CHAR] = "C20DF";
char BOTON_MAS_VOLUME[TAMANO_CHAR] = "C41BE";
char BOTON_MENOS_VOLUME[TAMANO_CHAR] = "CC13E";
char BOTON_MAS_MAGNIFY[TAMANO_CHAR] = "C18E7";
char BOTON_MENOS_MAGNIFY[TAMANO_CHAR] = "C9867";
char BOTON_ON_TIMER[TAMANO_CHAR] = "CA45B";
char BOTON_SETUP_TIMER[TAMANO_CHAR] = "C649B";

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {11, 10, 9, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);
}
  
void loop(){
  char key = keypad.getKey();
  
  if (key){
    Serial.println(key);
  }
  switch (key) {
    case '1':
      Serial.println(BOTON_ON_OFF);
      irsend.sendNEC(0x00c40bf, 32);
      
    break;
  }
}
