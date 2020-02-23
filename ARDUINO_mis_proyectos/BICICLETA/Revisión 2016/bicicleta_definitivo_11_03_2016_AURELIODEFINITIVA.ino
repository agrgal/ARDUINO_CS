
  // Definicin de variables PINES
  int pinLuzFreno = 3;
  int pinIntermitenteDerecho = 4;
  int pinIntermitenteIzquierdo = 5;
  int pinLuzLarga = 7;
  int pinLuzCorta = 6;
  int pinPulsadorFreno = 10;
  int pinPulsadorIntermitenteDerecho = 11;
  int pinPulsadorIntermitenteIzquierdo = 12;
  int pinPulsadorLucesDelanteras = 8;
  
  
  // Definicin de variables. OTRAS
  int LuzFreno = LOW;
  int IntermitenteDerecho = LOW;
  int IntermitenteIzquierdo = LOW;
  int duracionParpadeoIntermitenteEnSeg = 10; // Aurelio
  // ************* PINES PARA EL ESTADO BOTN DELANTERO *****************
  int estadoLucesDelanterasANTES = 0; // Aurelio
  int estadoLucesDelanterasACTUAL = 0; // Aurelio
  int LucesDelanteras = 0;// Aurelio
  int contadorLucesDelanteras = 0; // Aurelio
  // ************* PINES PARA EL ESTADO BOTN DELANTERO *****************
  int i = 0;
  int j = 0;
  int k = 0;
  int randNumber1;
  int randNumber2;
  int randNumber3;
  int randNumber4;
  int randNumber5;
  unsigned long time;
  unsigned long time2;
  unsigned long time3;
  unsigned long time4;
  unsigned long time5;
  unsigned long time6;
  unsigned long time7;
  unsigned long time8;
  unsigned long time9;
  
  boolean emergencia = false;  
  
  // Funcin que devuelve un nmero aleatorio
  int aleatorio (int num) { 
    return random(num);
  }

  // SETUP. Definicin de pines de entrada y salida.
  void setup() {
    pinMode (pinLuzFreno, OUTPUT); //luz freno
    pinMode (pinIntermitenteDerecho, OUTPUT); //intermitente_derecho
    pinMode (pinIntermitenteIzquierdo, OUTPUT); //intermitente_izquierdo
    pinMode (pinLuzLarga, OUTPUT); //largas
    pinMode (pinLuzCorta, OUTPUT); //cortas
    pinMode (pinPulsadorFreno, INPUT); //pulsador_freno
    pinMode (pinPulsadorIntermitenteDerecho, INPUT); //pulsador_intermitente_derecho
    pinMode (pinPulsadorIntermitenteIzquierdo, INPUT); //pulsador_intermitente_izquierdo
    pinMode (pinPulsadorLucesDelanteras, INPUT); //pulsador_luces_delanteras
    Serial.begin(9600);
  }

  // Loop
  void loop () {
    
   randomSeed(millis()); // Activa la semilla de nmeros aleatorios
   time = millis(); //tiempo desde que se pone en marcha. 
 
   // ******************************** INTERMITENTE DERECHO **********************+
   //Pulsador Intermitente Derecho
    if (digitalRead(pinPulsadorIntermitenteDerecho) == true ) {            //Lectura del estado del pulsador del intermitente derecho
      time2 = time + duracionParpadeoIntermitenteEnSeg * 1000;                                            
      time3 = time;                                                       //Si el pulsador esta pulsado, a time 2 se le suman 5000, y time 3 se hace 0, para apagar el otro intermitente
      if (emergencia == true) { //La bandera, el estado de emergencia, se apaga al pulsar este pulsador
          emergencia = false;
          time2 = time; // desactivo el parpadeo
          delay(500);
      }                                            
   }
 
   //Activacion intermitente Derecho
   if (time<time2) {                                                       
      digitalWrite(pinIntermitenteDerecho, ((time2 - time)/200)%2);       //Si time2 es mayor que time, el intermitente derecho parpadea durante esos 5 segundos
   } 
    else {
      digitalWrite(pinIntermitenteDerecho, LOW);                          //Si time2 = time, se apaga el intermitente
      time2=time;
    }
   // ************************************************************************+  

   // ******************************** INTERMITENTE IZQUIERDO **********************+  
   //Pulsador intermitente Izquierdo
   if (digitalRead(pinPulsadorIntermitenteIzquierdo) == true ) {        //Lectura del estado del pulsador del intermitente derecho
      time3 = time + duracionParpadeoIntermitenteEnSeg * 1000;
      time2 = time;                                                      //Si el pulsador esta pulsado, a time 2 se le suman 5000, y time 3 se hace 0, para apagar el otro intermitente
      if (emergencia == true) { //La bandera, el estado de emergencia, se apaga al pulsar este pulsador
          emergencia = false;
          time3 = time; // desactivo el parpadeo
          delay(500);
      }
   } 
   
    //Activacion intermitente izquierdo
    if (time<time3) {
      digitalWrite(pinIntermitenteIzquierdo, ((time3 - time)/200)%2);    //Si time3 es mayor que time, el intermitente derecho parpadea durante esos 5 segundos
    } else {
      digitalWrite(pinIntermitenteIzquierdo, LOW);                       //Si time3 = time, se apaga el intermitente
      time3=time;
    }
   // ************************************************************************+   
  
  // **************************** Activo o no las luces de emergencia *************+
  if ((digitalRead(pinPulsadorIntermitenteDerecho)==true) && (digitalRead(pinPulsadorIntermitenteIzquierdo)==true)) {    
    delay(200);
    if ((digitalRead(pinPulsadorIntermitenteDerecho)==true) && (digitalRead(pinPulsadorIntermitenteIzquierdo)==true)){
      delay(500); // Aurelio. Cambio 1000 por 500
      digitalWrite (IntermitenteIzquierdo, HIGH);
      digitalWrite (IntermitenteDerecho, HIGH);
      delay (500); 
    emergencia = emergencia xor 1;                                                         //Cuando se pulsan los dos interruptores de los intermitentes, y se mantienen pulsados durante 0,2 segundos, se encienden ambos intermitentes y la bandera se hace verdadera, lo que activa el estado de emergencia.
    }
  }
  // ************************************************************************


  // ************************** LUCES EMERGENCIA ACTIVADAS ********************
  if (emergencia == true) {                                                               //Cuando emergencia es verdaderp, todas las luces parpadean aleatoriamente
     time4 = time + 10 * 1000;
    digitalWrite (pinIntermitenteDerecho, (time4/aleatorio(aleatorio(1000)))%2);
    digitalWrite (pinIntermitenteIzquierdo, (time4/aleatorio(aleatorio(1000)))%2);
    digitalWrite (pinLuzLarga, (time4/aleatorio(aleatorio(1000)))%2);
    digitalWrite (pinLuzCorta,(time4/aleatorio(aleatorio(1000)))%2);
    digitalWrite (pinLuzFreno, (time4/aleatorio(aleatorio(1000)))%2);
    delay(50);
  }
  // ***************************************************************************
  

  // ***************************************************************************
  //Activacion freno
  // ***************************************************************************
  if (digitalRead ((pinPulsadorFreno)) == true) {        //Cuando se pulse el pulsador del freno, la luz de freno se encendera.
    digitalWrite (pinLuzFreno, HIGH);
    digitalWrite (pinIntermitenteDerecho, HIGH);         // Ademas activo los intermitentes
    digitalWrite (pinIntermitenteIzquierdo, HIGH);
    // desactivar emergencia, al pulsar el freno
    emergencia = false;
  } else {                                               //Mientras que no esta pulsado el pulsador del freno, parpadea con un intervalo de 8 sgundos apagado y 2 encendido, constantemente
      
   // time8 = (time % 10000) / 8000;
   time8 = (time % 5000) / 3500; // Aurelio. Mas rapido.
   digitalWrite (pinLuzFreno, time8);
  }
  // ***************************************************************************
  
  //Activacion pulsador Luces delanteras
  /* if(digitalRead(pinPulsadorLucesDelanteras)==true){                   //Segun las veces que se pulse esta pulsador, se le suma uno a LucesDelanteras, desde 0 hasta 3. 
        delay(125);
        if(digitalRead(pinPulsadorLucesDelanteras)==true){
            // delay(75); // Lo quito, Aurelio
              if(digitalRead(pinPulsadorLucesDelanteras)==true){
               LucesDelanteras = LucesDelanteras+1;
               emergencia = false;
                  }
             }
        } */
  
  // *************************************************************************** 
  //AURELIO: Activacion pulsador Luces delanteras. Nueva activacion luces delanteras
  // ***************************************************************************
  estadoLucesDelanterasACTUAL = digitalRead(pinPulsadorLucesDelanteras);
  if (estadoLucesDelanterasANTES != estadoLucesDelanterasACTUAL)  // ha habido un cambio de estado
  {
    contadorLucesDelanteras++;                          // cuenta los cambios de estado
    int validarPar = contadorLucesDelanteras % 2;       // solo queremos los cambios pares
    if (validarPar != 1)                 // si el cambio es par
    {
      if (emergencia==true) { // Si estaba en estado de emergencia
          emergencia = false; // desactivo luces de emergencia
          LucesDelanteras=0; // desactivo luces
      } else {
          LucesDelanteras = LucesDelanteras+1; // Suma uno a LucesDelanteras.
          contadorLucesDelanteras = 0;
      }     
      delay(100);
    }
    estadoLucesDelanterasANTES = estadoLucesDelanterasACTUAL;
  }
  // ***************************************************************************
  //activacion luces delanteras
      switch (LucesDelanteras){                                            //Si empiezan apagadas ambas luces, la primera vez que se pulsa pasa a case 1
        case 0:
            digitalWrite (pinLuzCorta, LOW);
            digitalWrite (pinLuzLarga, LOW);
            
           break;
      
        case 1:
          digitalWrite (pinLuzCorta, HIGH);                               //Cuando esta en case 1, se enciende nada mas la luz corta
          digitalWrite (pinLuzLarga, LOW); 
          
          break;
        case 2:  
          digitalWrite (pinLuzCorta, HIGH);                              //Si se pulsa una segunda vez, pasa a case 2, y se encienden ambas luces
          digitalWrite (pinLuzLarga, HIGH);
          
        break;
       
        // Aurelio. Parpadeo de Luces
        case 3:  
          digitalWrite (pinLuzCorta, ((time/700)%2)%2);                  //Si se pulsa una tercera vez parpadean
          digitalWrite (pinLuzLarga, (1+(time/700)%2)%2);
          
        break;
        
        // Aurelio. Parpadeo de Luces Ms rpido
        case 4:  
          digitalWrite (pinLuzCorta, ((time/200)%2)%2);                  //Si se pulsa una tercera vez parpadean
          digitalWrite (pinLuzLarga, (1+(time/200)%2)%2);
          
        break;
         
        default: LucesDelanteras=0;                                        //Al pulsarlo otra vez, LucesDelanteras se hace 0 y se apagan ambas luces de nuevo
        break;
      }
    // ***************************************************************************
  
  
    
}     // Fin del program  



 


