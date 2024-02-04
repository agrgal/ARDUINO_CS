void CWSTA (ip, gateway,subnet,newHostname) {

   WiFi.mode(WIFI_STA);  // Modo estático
  WiFi.config(ip,gateway,subnet);
  WiFi.hostname(newHostname.c_str()); // establece el hostname
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
    //Get Current Hostname
    Serial.printf("New hostname: %s\n", WiFi.hostname().c_str());
  }
  
  }
