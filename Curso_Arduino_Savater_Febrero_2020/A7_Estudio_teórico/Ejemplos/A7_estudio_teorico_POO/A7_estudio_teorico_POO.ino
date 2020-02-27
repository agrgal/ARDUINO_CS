
// POO objetos
// Ejemplo con 4 leds en los pines 4,5,6 y 7
// Lo primero que hago es definir una clase
class LEDS 
{
   private: // atributos internos de la clase
     boolean estado;
     void aplicarEstado(){ // método privado
      digitalWrite(pin,estado);
     }

   public:
     int pin; // atributo externo de clase, accesible desde el programa principal
     LEDS(){ // constructor. Método que se llama IGUAL que la clase y se ejecuta
     // al instanciar un objeto.
         estado=LOW;
         pin=6; // al inicio tiene el 6
     }
     void apagar(){ 
        estado = LOW; 
        aplicarEstado(); // método de apagado
     }
     void encender(){
        estado = HIGH; 
        aplicarEstado(); // método de encendido
     }
     void parpadear(int frecuencia){
       if ((millis()/frecuencia)%2==0) {encender();}
       if ((millis()/frecuencia)%2==1) {apagar();}
     }
};

LEDS l1,l2; // ambos se instancias apagados en pin 6

// *****
// SETUP
// *****
void setup() {
  DDRD=B11111111;
  l2.pin=7; // A uno tengo que cambiar a pin 7.
}


// *****
// LOOP
// *****
void loop() {
  l1.parpadear(300);
  l2.parpadear(1500);
}
