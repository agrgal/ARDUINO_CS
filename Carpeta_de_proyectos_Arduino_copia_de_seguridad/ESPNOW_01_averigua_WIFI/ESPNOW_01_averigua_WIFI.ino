#include <ESP8266WiFi.h>
void setup() {
Serial.begin(115200); Serial.println();Serial.println();
Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());
}

void loop() {}
