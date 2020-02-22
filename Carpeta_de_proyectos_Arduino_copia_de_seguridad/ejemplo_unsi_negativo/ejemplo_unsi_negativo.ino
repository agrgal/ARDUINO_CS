int i = 0;
unsigned long b=0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  for (i=1;i<7;i=i+2) {
    Serial.println("i : " + (String) i);
    b=-1;
    Serial.println("b : " + (String) b);
  }
}
