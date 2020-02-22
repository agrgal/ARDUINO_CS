#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
// #include <ArduinoJson.h>

// https://circuits4you.com/2019/01/11/nodemcu-esp8266-arduino-json-parsing-example/

const char* ssid = "JAZZTEL_shny"; // Rellena con el nombre de tu red WiFi
const char* password = "3z7s5tvbtu4s"; // Rellena con la contraseña de tu red WiFi

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Conectando...");
  }
}

void loop() 
{
  if (WiFi.status() == WL_CONNECTED)   
  {

    Serial.println("Conectado...");
    
    HTTPClient http; //Object of class HTTPClient
    http.begin("http://jsonplaceholder.typicode.com/users/2");
    // http.begin("https://spreadsheets.google.com/feeds/list/16Jq882ztpGE1-uxmKxqR1xB_-SCfljfFjplwKcw90W0/1/public/values?alt=json");
    int httpCode = http.GET();

    Serial.print("Response Code:"); //200 is OK
    Serial.println(httpCode);   //Print HTTP return code

    if (httpCode > 0) 
    {

      String payload = http.getString();    //Get the response payload from server
 
      Serial.print("Returned data from Server:");
      Serial.println(payload);    //Print request response payload
      
    }
    http.end(); //Close connection
  }
  delay(60000);
}
