// siete salidas digitales a cada tramo del display
int outa = 11;
int outb = 13;
int outc = 2;
int outd = 3;
int oute = 4;
int outf = 5;
int outg = 6;
int outmediosegundo = 12;
int outcoma = 1;

// defino RESTART con el codigo ensamblador que hace un RESET
#define RESTART asm("jmp 0x0000");

// a los anodos de los displays. Actuan por logica inversa
int anodo[4]={7,8,9,10};
// entradas para modificar la hora y minuto
int start = 0; // 1 --> empieza , 0--> para
int reset = 0; // 0 --> continua, 1 --> hace un reset
int mode = 0; // 0 --> modo segundos + decimas  1 --> modo minutos + segundos

// variables de entrada analogica
int instart = 0; 
int inreset = 1; 
int inmode = 2; 

// valores a pasar al display, y calculos
int h = 0;
int m = 0;
int hora = 0;
int minutos = 0;
int segundos = 0;
int mediosegundo = 0;
int valorcoma = 2; // display2

// variables de los displays
unsigned long displays[4]; // variable para cada display
int cualactivo = 0 ; //variable para guardar cual se activa

//contador general de tiempo
unsigned long tiempo1=0;
unsigned long tiempo2=0;
unsigned long tiempo3=0;
int retrasoboton = 100; //retraso del botn al pulsar
unsigned long retardo=5999000;

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

if (numdisplay==valorcoma) {
  digitalWrite(outcoma,LOW); 
} else {
  digitalWrite(outcoma,HIGH); 
}

// elige el nodo
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
void valordisplay(int rayah, int rayam) {
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
pinMode(outcoma, OUTPUT);
pinMode(outmediosegundo, OUTPUT);
pinMode(anodo[0], OUTPUT);
pinMode(anodo[1], OUTPUT);
pinMode(anodo[2], OUTPUT);
pinMode(anodo[3], OUTPUT);
// Serial.begin(115200);
}

/* ====================================== */
/* Bucles principales del programa: loop  */
/* ====================================== */

void loop () { // bucle

tiempo1=millis(); // milisegundostranscurridos al empezar el bucle

if (start==1) {
  tiempo3=retardo+(tiempo1-tiempo2)/100; // decimas de segundo
} else {
  tiempo2=tiempo1-(tiempo3*100); 
}

// calculo y mostrar
if (mode==0) {
  h = tiempo3 /100; // centenas - decenas segundos
  m = tiempo3 % 100; // unidades y decimas de segundo
  valorcoma = 2; // tercer display
  if (tiempo3>9999) { // caso que sobrepase la cantidad
     mode=2; // cambia al modo 2.      
  } 
} else if (mode==1) {
  // segundos sin decimas
  segundos = tiempo3 / 10; // calculo los segundos transcurridos, sin decimas
  h = segundos /100 ;
  m = segundos % 100;
  valorcoma = 3; // la coma en el ultimo digito
  if (segundos>9999) { // caso que sobrepase la cantidad
     mode=3; // cambia al modo 3.      
  } 
} else if (mode==2) {
  // minutos - segundos
  segundos = tiempo3 / 10; // calculo los segundos transcurridos
  minutos = segundos / 60;
  segundos = segundos % 60;
  h = minutos;
  m = segundos;
  valorcoma=1; // la coma en la segunda posicion
  if (segundos>5999) { // caso que sobrepase la cantidad
     mode=3; // cambia al modo 3.      
  } 
} else if (mode==3) {
  // minutos - segundos
  segundos = tiempo3 / 10; // calculo los segundos transcurridos
  mediosegundo = segundos % 2;
  minutos = segundos / 60;
  hora = minutos / 60 ;
  // hora = hora % 24 NO, no es un reloj
  minutos = minutos % 60;
  h = hora;
  m = minutos;
  digitalWrite(outmediosegundo, mediosegundo); // la coma en la segunda posicion, parpadeante
  valorcoma = 9; // este valor no existe, y no pondr como ninguna
  if (hora*3600+minutos*60>359940) {
    RESTART;
  }
}

// anula el parpadeo de leds centrales en caso de mode distinto de 3
if (mode!=3) {
  digitalWrite(outmediosegundo, HIGH);
}

  valordisplay(LOW,LOW);
  cualactivo = (cualactivo+1)*(cualactivo<3);
  mandasenal(cualactivo,displays[cualactivo]); 


// 1) comprueba start
if (analogRead(instart)>500) { //compara
delay(retrasoboton); // el retraso hace dar un pequeño salto al display.
// ajustar el retraso por cada boton. Se ha puesto un valor de 50 pero podria tener que ser mas
if (analogRead(instart)>500) {
   start=!start; // cambio la condicion de start
}
}

// 2) comprueba reset
if (analogRead(inreset)>500) { //compara
delay(retrasoboton); // el retraso hace dar un pequeño salto al display.
// ajustar el retraso por cada boton. Se ha puesto un valor de 50 pero podria tener que ser mas
if (analogRead(inreset)>500) {
   start=0;
   RESTART; // hago un RESTART en caliente
}
}

// 2) comprueba reset
if (analogRead(inmode)>500) { //compara
delay(retrasoboton); // el retraso hace dar un pequeño salto al display.
// ajustar el retraso por cada boton. Se ha puesto un valor de 50 pero podria tener que ser mas
if (analogRead(inmode)>500) {
   mode=(mode+1)*(mode<3);
}
}

delay(5); // si no introduzco este retraso los digitos no se ven correctamente

}
// Fin del programa

