/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-cam-pir-motion-detector-photo-capture/
 
  IMPORTANT!!!
   - Select Board "AI Thinker ESP32-CAM"
   - GPIO 0 must be connected to GND to upload a sketch
   - After connecting GPIO 0 to GND, press the ESP32-CAM on-board RESET button to put your board in flashing mode
 
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/
 
#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
// #include <EEPROM.h>            // read and write from flash memory
// define the number of bytes you want to access
// #define EEPROM_SIZE 1
 
RTC_DATA_ATTR int bootCount = 0;

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22
 
int pictureNumber = 0;

// ledPin refers to ESP32 GPIO 1
const int ledPin = 12; // No tiene todos los pines. pruebo en puerto IO12
const int Button = 2; // No tiene todos los pines. pruebo en puerto IO2
bool estado = 0; // estado del sistema
bool estadoAnt = 0; // estado anterior del sistema
bool pulsado = 0; // botón pulsado
  
void setup() {
 
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  
  // initialize digital pin ledPin as an output.
  pinMode(ledPin, OUTPUT);
  pinMode(Button, INPUT);
  pinMode(4, OUTPUT); // flash como salida

  digitalWrite(ledPin, LOW);  
  
  Serial.begin(115200);
 
  Serial.setDebugOutput(true);
 
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
   
  // conectar flash
  rtc_gpio_hold_dis(GPIO_NUM_4); // disable Hold. Permite recibir señal para encenderlo
  // desconectar flash
  // rtc_gpio_hold_en(GPIO_NUM_4); // enable Hold. No permite recibir señal para encenderlo

 
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 10;
    config.fb_count = 2;
        Serial.println("UXGA");
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
        Serial.println("SVGA");
  }

  // Init Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
      Serial.printf("Camera init failed with error 0x%x", err);
      digitalWrite(4, HIGH);
      return;
  }   
 
  Serial.println("Starting SD Card");

  if(!SD_MMC.begin()){
    Serial.println("SD Card Mount Failed");
    digitalWrite(4, HIGH);
    return;
  }  else { Serial.println("Tarjeta Montada"); } 

  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD Card attached");
    digitalWrite(4, HIGH);
    return;
  }  else { Serial.println("Tipo detectado"); } 

} 
 
void loop() {

  // parpadeo 
  digitalWrite(ledPin, int(millis()/500)%2);
  digitalWrite(4, int(millis()/500)%2); // debería parpadear el flash.

  pulsado = digitalRead(Button);
  estado = (!estadoAnt)*pulsado + estado*(!pulsado);

  // **********************
  // pulso y tomo una foto
  // **********************
  if (estado!=estadoAnt) {

    estadoAnt=estado; // cambia el estado Anterior

    if (estado==HIGH) { // Solo lo hace en flanco de subida
    
          digitalWrite(ledPin,HIGH);
          digitalWrite(4,LOW); // apaga la cámara
      
          pictureNumber = pictureNumber +1; // Suma 1 al contador de imagenes
      
          delay(1000);     
      
          camera_fb_t * fb = NULL;
            
           // Take Picture with Camera
            fb = esp_camera_fb_get();  
            if(!fb) {
              Serial.println("Camera capture failed");
              return;
            }
      
             // Path where new picture will be saved in SD Card
              String path = "/picture" + String(pictureNumber) +".jpg";
             
              fs::FS &fs = SD_MMC; // cambio a SD_MMC
              Serial.printf("Picture file name: %s\n", path.c_str());
             
              File file = fs.open(path.c_str(), FILE_WRITE);
              if(!file){
                Serial.println("Failed to open file in writing mode");
              }
              else {
                file.write(fb->buf, fb->len); // payload (image), payload length
                Serial.printf("Saved file to path: %s\n", path.c_str());
                // EEPROM.write(0, pictureNumber);
                // EEPROM.commit();
              }
              file.close();
              esp_camera_fb_return(fb);      
              
              // ESP.restart();

              delay(1000); // Espera

    } // Fin de estado solo HIGH. Flanco de subida   
  
  } // Fin del if del botón    
 
} // fin del loop