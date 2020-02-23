#define led 8
#define ir 7

long tiempoCorte = 0;
int contar = 0;
long tiempo;


void setup() {
  // define el pin led como salida y el ir como entrada DIGITAL
  pinMode(led, OUTPUT);
  pinMode(ir, INPUT);
  // Serial
  Serial.begin(9600); // Pone en marcha la lectura en serie.
}

void loop() {

  // cuenta del tiempo;
  tiempo = micros();
  
  // parpadea led en pin 8
  digitalWrite(led,!(digitalRead(ir)));

  // Leo el estado
  // estado = digitalRead(ir);

  // Comparo estado, con el antiguo
  if (pulseIn(ir,LOW)) {
     contar+=1;
     // Serial.println(contar);
     delay(200);

       if (contar%2==0 && contar>0) {
          Serial.println(abs(tiempo-tiempoCorte));
          // Para el siguiente ciclo
          contar=0;
          tiempoCorte = tiempo;
       }
     
  }

  // al final 

  // estadoAntiguo = estado;


 
}
