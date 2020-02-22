// Esta es la librería para utilizar las funciones de red del ESP8266
#include <ESP8266WiFi.h> 

const char* ssid = "JAZZTEL_shny"; // Rellena con el nombre de tu red WiFi
const char* password = "3z7s5tvbtu4s"; // Rellena con la contraseña de tu red WiFi

const char* host = "api.wunderground.com";
const char* apiKey = "8f7e8fc559d74636be8fc559d7f636fe"; // Puedes obtenerla en

// Huella digital del certificado del servidor https://www.wunderground.com
const char * fingerprint = "a9 1f b9 fe 35 b8 38 b9 54 67 e7 34 52 8a 24 d4 17 29 fa 32";

void setup() {
  Serial.begin(115200);
  delay(10);

  // Conectamos a la red WiFi

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  /* Configuramos el ESP8266 como cliente WiFi. Si no lo hacemos 
  se configurará como cliente y punto de acceso al mismo tiempo */
  WiFi.mode(WIFI_STA); // Modo cliente WiFi
  WiFi.begin(ssid, password);

  // Esperamos a que estemos conectados a la red WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected"); 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // Mostramos la IP
}

void loop() {

  Serial.print("connecting to ");
  Serial.println(host);
 
  // Creamos el cliente
  WiFiClientSecure client;
  const int httpPort = 443; // Puerto HTTPS
  if (!client.connect(host, httpPort)) {
    // ¿hay algún error al conectar?
    Serial.println("Ha fallado la conexión");
    return;
  }

  // Verificamos que el certificado coincide con el esperado
  if(client.verify(fingerprint,host)){
    Serial.println("El certificado es correcto"); 
  } else {
    Serial.println("Error de certificado"); 
  }
 
  // Creamos la URL para la petición
  String url = "/api/";
  url += apiKey;
  url += "/conditions/lang:SP/q/autoip.json";
 
  Serial.print("URL de la petición: https://");
  Serial.print(host);
  Serial.print(":");
  Serial.print(httpPort);
  Serial.println(url);
 
  // Enviamos la petición
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Superado el tiempo de espera !");
      client.stop();
      return;
    }
  }

  // Consutar la memoria libre
  // Utilizando HTTPS se necesitan unos 20 kB de memoria libres adicionales
  Serial.printf("\nMemoria libre en el ESP8266: %d Bytes\n\n",ESP.getFreeHeap());
 
  // Leemos la respuesta y la enviamos al monitor serie
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
 
  Serial.println();
  Serial.println("Cerrando la conexión");

  while(1){
    delay(0); // Siempre que hay un bucle que pueda durar mucho tiempo
              // hay que llamar a la función delay() para atender a los 
              // procesos de la conexión WiFi. Si no se hace el ESP8266
              // generará un error y se reiniciará a los pocos segundos
   }
}
