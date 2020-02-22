// siete salidas digitales a cada tramo del display
int outa = 11;
int outb = 13;
int outc = 2;
int outd = 3;
int oute = 4;
int outf = 5;
int outg = 6;
int outsegundero = 12;
int outalarma=1;

// a los anodos de los displays. Actuan por logica inversa
int anodo[4]={7,8,9,10};
// entradas para modificar la hora y minuto
int inhoraria = 0;
int inminutero = 1; //entrada 1
int inalarma = 2; //entrada 2

int inDisplaySec = 5; // Por razones educativas... DISPLAYS EN SECUENCIA

// cuenta el tiempo
unsigned long hora = 0; // de 0 a 23.
unsigned long minuto = 0; // de 0 a 59
unsigned long hora0 = 10; // de 0 a 23.
unsigned long minuto0 = 10; // de 0 a 59 // :-) Como los relojes de esfera, a las 10 y 10.
unsigned long segundo0 = 0; // de 0 a 59
unsigned long segundo = 0; // de 0 a 59
unsigned long segundostranscurridos = 0; // de 0 a 59. segundostranscurridos transcurridos
unsigned long mediosegundo = 0; // de 0 a 59
unsigned long muestraminuto = 0;
unsigned long muestrahora = 0;

// alarma //Inicializo a las 10:05
unsigned long horaalarma=10;
unsigned long minutoalarma=5;
int alarma=0; // desactivada 0 y activada 1

// variables de los displays
unsigned long displays[4]; // variable para cada display
int cualactivo = 0 ; //variable para guardar cual se activa

//contador general de tiempo
unsigned long tiempo1=0;
int retrasoboton = 100; //retraso del botn al pulsar

int lecturaDisplaySec = 0; // Por razones educativas... DISPLAYS EN SECUENCIA

// contador de proposito general
int i=0;

/* ====================== */
/* Funciones y subrutinas */
/* ====================== */

// *************************************************
// Funcion que pone un valor en un display a la vez
// *************************************************

void mandasenal(int numdisplay, int valor) {
// ********************************************
// a) activa cada tramo del display segun valor
// ********************************************
  // recibe el numero de display y el valor que tiene que entregar
switch (valor) {
case 0:
digitalWrite(outa,LOW);
digitalWrite(outb,LOW);
digitalWrite(outc,LOW);
digitalWrite(outd,LOW);
digitalWrite(oute,LOW);
digitalWrite(outf,LOW);
digitalWrite(outg,HIGH);
break;
case 1:
digitalWrite(outa,HIGH);
digitalWrite(outb,LOW);
digitalWrite(outc,LOW);
digitalWrite(outd,HIGH);
digitalWrite(oute,HIGH);
digitalWrite(outf,HIGH);
digitalWrite(outg,HIGH);
break;
case 2:
digitalWrite(outa,LOW);
digitalWrite(outb,LOW);
digitalWrite(outc,HIGH);
digitalWrite(outd,LOW);
digitalWrite(oute,LOW);
digitalWrite(outf,HIGH);
digitalWrite(outg,LOW);
break;
case 3:
digitalWrite(outa,LOW);
digitalWrite(outb,LOW);
digitalWrite(outc,LOW);
digitalWrite(outd,LOW);
digitalWrite(oute,HIGH);
digitalWrite(outf,HIGH);
digitalWrite(outg,LOW);
break;
case 4:
digitalWrite(outa,HIGH);
digitalWrite(outb,LOW);
digitalWrite(outc,LOW);
digitalWrite(outd,HIGH);
digitalWrite(oute,HIGH);
digitalWrite(outf,LOW);
digitalWrite(outg,LOW);
break;
case 5:
digitalWrite(outa,LOW);
digitalWrite(outb,HIGH);
digitalWrite(outc,LOW);
digitalWrite(outd,LOW);
digitalWrite(oute,HIGH);
digitalWrite(outf,LOW);
digitalWrite(outg,LOW);
break;
case 6:
digitalWrite(outa,HIGH);
digitalWrite(outb,HIGH);
digitalWrite(outc,LOW);
digitalWrite(outd,LOW);
digitalWrite(oute,LOW);
digitalWrite(outf,LOW);
digitalWrite(outg,LOW);
break;
case 7:
digitalWrite(outa,LOW);
digitalWrite(outb,LOW);
digitalWrite(outc,LOW);
digitalWrite(outd,HIGH);
digitalWrite(oute,HIGH);
digitalWrite(outf,HIGH);
digitalWrite(outg,HIGH);
break;
case 8:
digitalWrite(outa,LOW);
digitalWrite(outb,LOW);
digitalWrite(outc,LOW);
digitalWrite(outd,LOW);
digitalWrite(oute,LOW);
digitalWrite(outf,LOW);
digitalWrite(outg,LOW);
break;
case 9:
digitalWrite(outa,LOW);
digitalWrite(outb,LOW);
digitalWrite(outc,LOW);
digitalWrite(outd,HIGH);
digitalWrite(oute,HIGH);
digitalWrite(outf,LOW);
digitalWrite(outg,LOW);
break;
case 10:
digitalWrite(outa,HIGH);
digitalWrite(outb,HIGH);
digitalWrite(outc,HIGH);
digitalWrite(outd,HIGH);
digitalWrite(oute,HIGH);
digitalWrite(outf,HIGH);
digitalWrite(outg,LOW);
break;
}
// **********************************
// b) activo un solo display cada vez
// **********************************
for (int i=0;i<=3;i++) { // recorrido de 0 hasta 3
if (i==numdisplay) {
digitalWrite(anodo[i],HIGH); // activo por 
} else {
digitalWrite(anodo[i],LOW); // desactivo por 
} // fin del if
} // fin del for
}

// **********************************
// displays
// **********************************
void valordisplay(unsigned long h,unsigned long m,int rayah, int rayam) {
  if (rayah==LOW) {
   displays[0]= h/10;
   displays[1]= h % 10; // resto de la division entre 10
  } else {
   displays[0]= 10;
   displays[1]= 10; // guion
  }
  if (rayam==LOW) { 
   displays[2]= m / 10;
   displays[3]= m % 10; // resto de la division entre 10
  } else {
    displays[2]= 10;
    displays[3]= 10; // guion
  }
}

// **********************************
// calcula el tiempo
// **********************************

void calculamuestratiempo() {
  // 1) calculos para el reloj
  segundostranscurridos = hora0*3600+minuto0*60+segundo0+((tiempo1/1000) % 86400);
  segundo= segundostranscurridos % 60;
  minuto = segundostranscurridos/60;
  hora = minuto/60;
  hora = hora % 24;
  minuto = minuto % 60;
}

/* ===================================== */
/* Bucles principales del programa: setup */
/* ====================================== */
void setup () { // inicializo
pinMode(outa, OUTPUT);
pinMode(outb, OUTPUT);
pinMode(outc, OUTPUT);
pinMode(outd, OUTPUT);
pinMode(oute, OUTPUT);
pinMode(outf, OUTPUT);
pinMode(outg, OUTPUT);
pinMode(outsegundero, OUTPUT);
pinMode(anodo[0], OUTPUT);
pinMode(anodo[1], OUTPUT);
pinMode(anodo[2], OUTPUT);
pinMode(anodo[3], OUTPUT);
pinMode(outalarma, OUTPUT);
// Serial.begin(115200);
}

/* ====================================== */
/* Bucles principales del programa: loop  */
/* ====================================== */

void loop () { // bucle

tiempo1=millis(); // milisegundostranscurridos al empezar el bucle

// 1) comprueba horaria
if (analogRead(inhoraria)>500 && analogRead(inalarma)<500) { //compara
delay(retrasoboton); // tiempo de espera 50 milisegundostranscurridos de pulsacion de boton. El boton debe mantenerse pulsado 50 milisegundostranscurridos
// hacerlo de esta manera evita que una pulsacion fisica sea reconocida como muchas pulsaciones
// ajustar el retraso por cada boton. Se ha puesto un valor de 50 pero podria tener que ser mas
if (analogRead(inhoraria)>500) {
   hora0=(hora0+1)*(hora0<23);
   calculamuestratiempo();
   valordisplay(hora,0,LOW,HIGH);
   for(i=0;i<=300;i++) {
      mandasenal(i%4,displays[i%4]); 
      delay(1);
   }
}
}

// 2) comprueba minutero
if (analogRead(inminutero)>500 && analogRead(inalarma)<500) { //compara
delay(retrasoboton); // tiempo de espera 50 milisegundostranscurridos de pulsacion de boton. El boton debe mantenerse pulsado 50 milisegundostranscurridos
if (analogRead(inminutero)>500) {
   minuto0=(minuto0+1)*(minuto0<59);
   calculamuestratiempo();
   valordisplay(0,minuto,HIGH,LOW);
   for(i=0;i<=300;i++) {
      mandasenal(i%4,displays[i%4]); 
      delay(1);
   }
}
}


// 3) comprueba segundero. Solo verlo
visualizo:
if (analogRead(inalarma)>500) { //compara
delay(retrasoboton); // tiempo de espera 50 milisegundostranscurridos de pulsacion de boton. El boton debe mantenerse pulsado 50 milisegundostranscurridos
if (analogRead(inalarma)>500 && analogRead(inhoraria)<500 && analogRead(inminutero)<500) {
   alarma=!alarma;
   valordisplay(horaalarma,minutoalarma,LOW,LOW);
   digitalWrite(outsegundero,!alarma);
   for(i=0;i<=2000;i++) {
      mandasenal(i%4,displays[i%4]); 
      delay(1);
   }
} else if(analogRead(inalarma)>500 && analogRead(inhoraria)>500 && analogRead(inminutero)<500) { // caso que pulse las dos a la vez
   horaalarma=(horaalarma+1)*(horaalarma<23);
   valordisplay(horaalarma,minutoalarma,LOW,LOW);
   for(i=0;i<=500;i++) {
      mandasenal(i%4,displays[i%4]); 
      delay(1);
   }
   goto visualizo;
} else if(analogRead(inalarma)>500 && analogRead(inhoraria)<500 && analogRead(inminutero)>500) {
  minutoalarma=(minutoalarma+1)*(minutoalarma<59);
   valordisplay(horaalarma,minutoalarma,LOW,LOW);
   for(i=0;i<=500;i++) {
      mandasenal(i%4,displays[i%4]); 
      delay(1);
   }
   goto visualizo;
} // fin del if secundario
} // fin del if principal

// *******************************************
// previo: si llega a la cuenta de los 50 dias
// *******************************************
if (tiempo1<=10 && segundostranscurridos>=1) { // esta situacin no se puede dar de inicio
// si segundostranscurridos>=1 entonces como tiempo1>=1000
// Esta situacion puede darse si se alcanzan los 50 dias (limite maximo del conteo de milisegundos) y se reinicie la cuenta de tiempo1
// En ese caso fuerzo a que la hora inicial sea la hora actual
   hora0=hora;
   minuto0=minuto; // pongo la hora y los minutos de inicio como las actuales
   segundo0=segundo;
}

//a) segundero
mediosegundo = ((tiempo1/500) % 2);
digitalWrite(outsegundero,mediosegundo);

// b) calculo el tiempo
calculamuestratiempo(); // llama a la funcin que calcula las variables temporales
valordisplay(hora,minuto,LOW,LOW);

//c) visualizo
cualactivo=(cualactivo+1)*(cualactivo<3);
mandasenal(cualactivo,displays[cualactivo]); // activa los pines del display elegido

// c1) Delay para Propósitos Educativos
lecturaDisplaySec = analogRead(inDisplaySec);
if (lecturaDisplaySec>100) { delay(map(analogRead(inDisplaySec),100,1023,0,500)); } // Para que no haga delay si el valor es muy bajo.


// d) activo alarma
if (horaalarma==hora && minutoalarma==minuto && alarma==1) { // control de la alarma. En la hora, en el minuto y si alarma esta activado
  tone(outalarma,100*mediosegundo*2,100); 
} else {
  noTone(outalarma);
}

}
// Fin del programa

