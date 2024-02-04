String miCadena = "$Uno,dos,tres,cuatro,cinco#";
String cadaParte =""
long i=0; 

void setup (  ) {
  Serial.begin(9600);
}

void loop() {

  cadaParte = parte(parte(miCadena,'$',1),'#',0);
  if (cadaParte.length()>0) {
     Serial.println(cadaParte)
  }
}

// ********************************
// encuentra parte de una cadena
// ********************************
String parte(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}