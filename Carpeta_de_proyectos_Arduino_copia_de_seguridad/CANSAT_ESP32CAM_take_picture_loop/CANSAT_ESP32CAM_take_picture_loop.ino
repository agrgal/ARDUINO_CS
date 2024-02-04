/* Take picture when requested and sends via serial to Master ESP32*/

#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"

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

int cuenta = 0;
long tiempo = 0;
long segundos = 0;
int cada = 8000; // tiempo en ms

// ======
// SETUP
// ======

void setup() {
  
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

  // Turn off the flash
  pinMode(4,INPUT);
  digitalWrite(4,LOW); // ¿por qué cuando ocurre esto funciona? 
  
  //rtc_gpio_hold_dis(GPIO_NUM_4);
 
  Serial.begin(115200);                      // Define and start serial monitor
  
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
  
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  
  // Init Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Init Tarjeta
  init_microSD_card();
  
}

// ======
// Loop
// ======

void loop() {

  tiempo = millis();

  segundos = long(tiempo/cada); 

  if (segundos>long((tiempo-500)/cada)) { // cada 10 segundos
     cuenta++; // aumento el contador de la cuenta
     
      // ===============================
      // Guardar la foto en la tarjeta 
      // ===============================
      take_picture();
      
      // ===============================

      delay(cada/4); // algo de espera
  }
  
  // digitalWrite(4, LOW); // mantengo el flash apagado
}

// ==============
// Funciones
// ==============

// Tomar foto 
void take_picture( void ){
    
     // Tomo fotografía
     camera_fb_t * fb = NULL;
     
     // Take Picture with Camera
     fb = esp_camera_fb_get(); 
     if(!fb) { // Si no ha tomado la imagen
            Serial.println("Camera capture failed");
            return;
     }
     
     String path = "/imagen" + String(cuenta) +".jpg";
      
     fs::FS &fs = SD_MMC; 
      
     
     File file = fs.open(path.c_str(),FILE_WRITE);
      
     if(!file){
            Serial.printf(" Failed to open file in writing mode: %s\n", path.c_str());
            cuenta = cuenta + 0;
      } 
      else {
            file.write(fb->buf, fb->len); // payload (image), payload length
            Serial.printf(" Saved: %s\n", path.c_str());
      }
     
     file.close();
     
     esp_camera_fb_return(fb);   
     
     // Turns off the ESP32-CAM white on-board LED (flash) connected to GPIO 4
     digitalWrite(4,LOW);
}

// --> 'init_microSD_card()' function
void init_microSD_card( void ){
      if(!SD_MMC.begin()){
            Serial.println("SD Card Mount Failed");
            return;
      }
      uint8_t cardType = SD_MMC.cardType();
      if(cardType == CARD_NONE){
            Serial.println("No SD Card attached");
            return;
      }
      Serial.println("Ok!");
}
