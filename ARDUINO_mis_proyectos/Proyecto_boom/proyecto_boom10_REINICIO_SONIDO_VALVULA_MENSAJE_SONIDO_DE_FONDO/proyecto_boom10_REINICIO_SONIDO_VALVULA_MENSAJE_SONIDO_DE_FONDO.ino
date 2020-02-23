
// LIBRERíAS
// *********
// Librerías necesarias para la pantalla
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
// Librerías necesarias para el altavoz y el sonido
#include <SoftwareSerial.h>
#include <DFMiniMp3.h>

// Definición de orden RESETEO
// #define RESTART asm("jmp 0x0000"); //permite el RESET automático de la placa

// Clase para DFPlayer Mini
// Estado del módulo DFPlayer Mini

class Mp3Notify
{
public:
  static void OnError(uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
  }

  static void OnPlayFinished(uint16_t globalTrack)
  {
    Serial.println();
    Serial.print("Play finished for #");
    Serial.println(globalTrack);   
  }

  static void OnCardOnline(uint16_t code)
  {
    Serial.println();
    Serial.print("Card online ");
    Serial.println(code);     
  }

  static void OnCardInserted(uint16_t code)
  {
    Serial.println();
    Serial.print("Card inserted ");
    Serial.println(code); 
  }

  static void OnCardRemoved(uint16_t code)
  {
    Serial.println();
    Serial.print("Card removed ");
    Serial.println(code);  
  }
};

// Defino el puerto serie por Software, a los pines 6 y 7 
// Asocio al módulo DFplayer Mini
SoftwareSerial secondarySerial(6, 7); // RX, TX // puertos 6 y 7 
DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(secondarySerial);

// Activación de la pantalla LCD con I2C
LiquidCrystal_I2C lcd(0x27,16,2);  // dirección de la pantalla LCD, número de columnas, número de filas

// *********
// Variables
// *********

unsigned long tiempo=0; // temporizador
unsigned long tiempo2=0; // temporizador 2
int i,j=0; // contador proposito general
boolean juego=0; // estado del juego 0--> coloca solución 1--> jugando...
boolean tocarCabeceraFlag = 0; // No se ha tocado la canción de la cabecera
boolean tocarContinuaFlag = 0; // No se ha tocado la de continúa

byte respuestas= 0b00001111; // En conexiones 8, 9, 10 y 11
byte respuestasTemp= 0b00001111;
byte cables=0b00001111; // En conexiones 2,3,4,5

int solucionElegida=0;
byte solucionElegidaByte=B0;

int sonidos[] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,40,41,42,43,44,45,46,47,48,49,50,51};
int duracion[] = {10, 5, 10, 7, 2, 30, 15, 4, 13, 28, 27, 29, 16, 29, 6, 5, 19, 13, 27, 8, 1, 5, 21, 7};
int sonidoAleatorio =0;

// *********
// Funciones
// *********

// **********************************************************************
// Función que convierte un BYTE en binario para mostrar en Monitor Serie
// **********************************************************************
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

// **********************************************************************
// Función que lee el estado de los cables y modifica la variables cables
// **********************************************************************
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

// ***********************************************************************************
// Función que escribe la respuesta 8 al 11 conservando el estado del resto del puerto
// ***********************************************************************************
void escribeRespuestas(byte resp) { // Lee el estado de los cables y escribe la variable respuestasCable
    byte izquierdaPORTB = B0; // Tengo que quedarme con el número de los 4 bits más significativos de PORTB
    // Ejemplo: PORTB = 0111 1110 y nueva respuesta 1010
    // Conseguir solo los 4 bitsd de PORTB
    izquierdaPORTB = PORTB >> 4; // Los 4 bits a la derecha los pone en la izquierda. Ejemplo: PORTB = 0000 0111 
    izquierdaPORTB = izquierdaPORTB << 4; // Vuelve a ponerlo y rellena con ceros los 4 menos significativos  Ejemplo: PORTB = 0111 0000
    PORTB = izquierdaPORTB | resp; // Hace la operacion OR. Ejemplo: PORTB = 0111 1010
    // Serial.println("PORTB=" + numberToBin(PORTB) + " - IZQUIERDA_PORTB=" + numberToBin(izquierdaPORTB) + " - respuestas: " + numberToBin(resp));
}

// *************************************************
// Función toca una de las cuatro pistas almacenadas
// *************************************************
void tocaMusica(int pista, int tiempo, int vv) {
  ponerVolumen(vv);
  mp3.playMp3FolderTrack(pista);  // sd:/mp3/0001.mp3
  waitMilliseconds(tiempo); // Espera 15 segundos
}

// *************************************************
// Función que espera un tiempo y refresca el sonido
// *************************************************
void waitMilliseconds(uint16_t msWait)
{
  uint32_t start = millis();  
  while ((millis() - start) < msWait)
  {
    // calling mp3.loop() periodically allows for notifications 
    // to be handled without interrupts
    mp3.loop(); 
    delay(1);
  }
}

// *************************************************
// Parpadeo del led correspondiente al cable cortado
// *************************************************
void parpadeo(byte resp, byte cab) { // parpadeo al quitar el cable.
    for (j=0;j<5000;j++){
      if ((j/200)%2) {PORTB= resp | 1 << 4;  } else {PORTB= cab | 1 << 4;}
      delay(1);
    }
}

// *************************************************
// muestra mensaje en pantalla
// *************************************************
void mensajeEnPantalla(String cadena1, String cadena2, boolean limpiar) {
    if (limpiar) {lcd.clear();}
    lcd.setCursor(0,0);             
    lcd.print(cadena1); 
    lcd.setCursor(0,1);
    lcd.print(cadena2);
}

// *************************************************
// Modificar volumen
// *************************************************
void ponerVolumen(int volumen) {
  // uint16_t volume = mp3.getVolume();
  Serial.println("Volumen: " + (String)volumen);
  mp3.setVolume(volumen); // 30 el máximo.
}

// *************************************************
// Reinicio
// *************************************************
void reinicio(int retraso) {

  
  
  lcd.init();                     // inicializa la pantalla LCD
  lcd.backlight();                // enciende la retroiluminación de la pantalla 
  lcd.setCursor(0,0);             // ponemos el cursor en la columna 3 de la fila 0
  
  if (retraso<100) {
    lcd.print("Empezamos el juego..."); // escribimos
   } else {
    lcd.print("Reiniciando..."); // escribimos    
   }

   delay(retraso); // retraso que permite limpiar el estado del botón
   
   lcd.clear(); // limpia la pantalla

   digitalWrite(15,0); // Apago. Es el testigo para activar una posible válvula.

  // Empezamos a cero en PORTB
  PORTB= 0 | 1 << 4;

  // Inicio de variables
  juego = 0;
  tocarCabeceraFlag = 0; // No se ha tocado la canción de la cabecera
  tocarContinuaFlag = 0; // No se ha tocado la de continúa
  respuestas= 0b00001111; // En conexiones 8, 9, 10 y 11
  respuestasTemp= 0b00001111;
  cables=0b00001111; // En conexiones 2,3,4,5
  solucionElegida=0;
  solucionElegidaByte=B0;
  sonidoAleatorio =0;

}


//******************************** 
//******************************** 
//******************************** 

// *********
// SetUP
// *********
void setup()
{
 
  Serial.begin(9600); // numeros del serial monitor
  randomSeed(analogRead(A3)); // semilla aleatoria en pin 3
  
  // Inicializando altavoz al máximo de volumen
  mp3.begin();
  ponerVolumen(30);
  
  pinMode(15,OUTPUT); // puerto Analógico 1 como salida digital

  // Asignacion de pines de salida y de entrada en el registro del Atmega8. 1-> Output y 0-> Input: PIN MODE
  DDRD = B10000000;  // digital pins 7,6,5,4,3,2,1,0  ¿El pin 7 es de Salida --> TX del DFPlayer Mini?
  DDRB = B00011111;  // digital pins -,-,13,12,11,10,9,8

  reinicio(10);

}

// *********
// Principal
// *********
void loop()
{

  tiempo = millis(); // tiempo transcurrido desde el inicio del programa

  // Serial.println("PIND: " + numberToBin(PIND));

  leerCables();
 
  if (digitalRead(13) && cables==0B00001111) { juego=1; }// Simplemente pulsando el boton de inicio del juego, se activa
  // Y si los cables están todos en su sitio. Variable cables correcta (todos conectados)
  
  if (cables!=0B00001111 && juego==0){ // Cuidado! si no están los cables puestos... y aún no ha empezado el juego, que te avise... Seguridad 
    mensajeEnPantalla("Pon los cables!!","",1);
    delay(500);
    cables=0B00001111; // restauro el inicio de la variable cables.
  }

  if (juego) { // Comprueba el estado del juego

    // *************************************************************************************
    // A) escribe en la pantalla la solución del juego para informar al control
    // *************************************************************************************
    mensajeEnPantalla("Juego en marcha!","Solucion en " + (String) char(solucionElegida+65) +" ",0);
    
     if (!tocarCabeceraFlag) {
       // toca la musica de cabecera si no se ha tocado
       tocarCabeceraFlag=1;
       Serial.println("Musica de cabecera"); 
       tocaMusica(1,14000,30);
       tiempo2 = millis() + 8000; // permite que el sonido de fondo no se ejecute inmediatamente después de quitar cable.
     }

     if (tocarCabeceraFlag==1 && tiempo2 < tiempo) {
        sonidoAleatorio = random(0, 12); // 11 uno más...
        tiempo2 = tiempo + duracion[sonidoAleatorio]*1000+2500; // aumento del tiempo 2. Pausa 
        tocaMusica(sonidos[sonidoAleatorio],50,map(analogRead(A2),0,1023,0,30));
        Serial.println("Toca el sonido: "+ (String) sonidos[sonidoAleatorio] +" -- tiempo: "+ (String) duracion[sonidoAleatorio]);      
     }
    
    // ***************  
    // B) Leer cables
    // ***************
    leerCables(); // Se leen los cables 
    if (respuestas!=cables) { // Si los cables no coinciden con las respuestas
      respuestasTemp = respuestas; // variable temporal. Almaceno también en variable temporal
      respuestas=cables; // Asigno a respuestas la variable cables
      // ¿Tocar algo de continuidad?  
      tocarContinuaFlag=1; // activo la bandera para activar música de continúa. 
    }

      escribeRespuestas(respuestas); // Se escriben las respuestas en los LEDs
   
          // *************************************************
          // C)/ Condicional que detecta si se gana o pierde
          // *************************************************
          if (solucionElegidaByte==respuestas) { // Detectar ganador
            tocaMusica(3,100,30); // musica de ganador. sd://mp3/0003.mp3
            parpadeo(respuestasTemp, cables); // parpadeo del cable cortado.
            mensajeEnPantalla("Has ganado!!","Restaura cables",1);
            // Juego de luces del ganador
              while (!digitalRead(13)) {
                PORTB= 15 | 1 << 4;
                delay(100);
                PORTB= 0 | 1 << 4;
                delay(100);
              }

              reinicio(2000);
                            
          } else if ((solucionElegidaByte & respuestas)==0) { // Detecta si se pierde
            // La respuesta con un AND a solución elegida siempre da cero excepto en el bit de la respuesta.
            // A menos que la respuesta se haya anulado, por lo que el resultado sería 0  
            tocaMusica(4,100,30); // musica de perdedor. sd://mp3/0003.mp3
            parpadeo(respuestasTemp, cables); // parpadeo del cable cortado.
            digitalWrite(15,HIGH); // Posible activación de válvula.
            mensajeEnPantalla("Has perdido!!","Restaura cables",1);
            // Juego de luces del perdedor
              while (!digitalRead(13)) {
                for (i=0;i<=3;i++) {
                    if (j==0) {PORTB = 1 << i | 1 << 4;} else {PORTB = 8 >> i | 1 << 4;}
                    delay(100);
                }
                j=(j+1)%2;
              }
              
              reinicio(2000);
              
          } else if (tocarContinuaFlag==1){ // si tengo bandera activa
            // musica de continua
            tocarContinuaFlag=0; // Desactivo bandera para que solo toque una vez
            tocaMusica(2,100,30); // musica de continuar. sd://mp3/0003.mp3
            parpadeo(respuestasTemp, cables); // parpadeo del cable cortado.
            mensajeEnPantalla("Puede continuar"," ",1);
            tiempo2 = millis() + 8000; // permite que el sonido de fondo no se ejecute inmediatamente después de quitar cable.
            delay(2000); // Pequeño retraso.            
          }    
    
  } else { // Condicional sobre la variable juego. Mientras no se empiece. 
    
    solucionElegida = map(analogRead(A0),0,1024,0,4); // Solución según la posición del cursor.   
    mensajeEnPantalla("Elige una solucion...","Solucion en " + (String) char(solucionElegida+65) +" ",0);    
    solucionElegidaByte = 1 << solucionElegida; // desplaza a la izquierda el 1 las posiciones elegidas
    // Serial.println("Solucion Elegida=" + numberToBin(solucionElegidaByte));

    if (tiempo2 < tiempo) { // Toca OTRAS historias mientras tanto
        sonidoAleatorio = (sonidoAleatorio + 1)%12;
        tiempo2 = tiempo + duracion[12+sonidoAleatorio]*1000+15000; // aumento del tiempo 15". Pausa 
        tocaMusica(sonidos[12+sonidoAleatorio],50,map(analogRead(A2),0,1023,0,30));
        Serial.println("Toca el sonido: "+ (String) sonidos[12+sonidoAleatorio] +" -- tiempo: "+ (String) duracion[12+sonidoAleatorio]);       
    }
    
  } // Fin del IF sobre la variable juego

  // parpadeo estado de juego pin 12
  digitalWrite(12,(tiempo/500)%(2-juego*2)); // parpadea mientras pone la solucion; fija cuando estamos en juego.
  
} // Fin del Loop
//******************************** 

