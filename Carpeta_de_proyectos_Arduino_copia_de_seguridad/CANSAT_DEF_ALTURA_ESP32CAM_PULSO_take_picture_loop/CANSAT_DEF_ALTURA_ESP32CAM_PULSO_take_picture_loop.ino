/* Take picture when requested and sends via serial to Master ESP32*/

#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"


// Serial comunnication with Arduino
#define RXp2 3 // Rx de ESP 32 CAM
#define TXp2 1 // Tx de ESP 32 CAM

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
int valor = 0;
int valorAnt = 0;

byte upperByte, lowerByte;

// ======
// SETUP
// ======

void setup() {
  
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

  // Turn off the flash
  pinMode(4,INPUT);
  digitalWrite(4,LOW); // ¿por qué cuando ocurre esto funciona? 

  // Turn on built-in LED GPIO 33
  pinMode(33,INPUT);
  digitalWrite(33,LOW);
  
  //rtc_gpio_hold_dis(GPIO_NUM_4);
 
  // Serial.begin(115200);                      // Define and start serial monitor

  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2); // Segunda comunicación serie
  
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
    config.jpeg_quality = 8; // toma esta opción. tamaño fotos 1600x1200
                             // Valor estandard 10. Si lo rebajo voy aumentando calidad, pero... 
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 10;
    config.fb_count = 1;
  }
  
  // Init Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // parámetros de la cámara: repasarlos
  // https://randomnerdtutorials.com/esp32-cam-ov2640-camera-settings/
  sensor_t * s = esp_camera_sensor_get();
  s->set_brightness(s, 0);     // -2 to 2
  s->set_contrast(s, 0);       // -2 to 2
  s->set_saturation(s, 0);     // -2 to 2
  s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
  s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
  s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
  s->set_wb_mode(s, 0);        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
  s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
  s->set_aec2(s, 0);           // 0 = disable , 1 = enable
  s->set_ae_level(s, 0);       // -2 to 2
  s->set_aec_value(s, 300);    // 0 to 1200
  s->set_gain_ctrl(s, 1);      // 0 = disable , 1 = enable
  s->set_agc_gain(s, 0);       // 0 to 30
  s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
  s->set_bpc(s, 0);            // 0 = disable , 1 = enable
  s->set_wpc(s, 1);            // 0 = disable , 1 = enable
  s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
  s->set_lenc(s, 1);           // 0 = disable , 1 = enable
  s->set_hmirror(s, 0);        // 0 = disable , 1 = enable
  s->set_vflip(s, 0);          // 0 = disable , 1 = enable
  s->set_dcw(s, 1);            // 0 = disable , 1 = enable
  s->set_colorbar(s, 0);       // 0 = disable , 1 = enable

  // Init Tarjeta
  init_microSD_card();
  
}

// ======
// Loop
// ======

void loop() {

  tiempo = millis();
  segundos = long(tiempo/cada); 

  if (Serial2.available() > 0) { // recepcion de dos bytes
    // read the incoming byte:
    valor = (int) Serial2.parseInt();
    Serial2.flush(); // limpia el buffer??
  }
  
  if (valor>valorAnt) { // ¿habría que poner valor != valorAnt?
      cuenta++; // suma uno al valor de la cuenta
      take_picture(valor); 
      digitalWrite(33,HIGH); // enciende el LED
      delay(500); // espera al menos 500 segundos
      digitalWrite(33,LOW); // Lo apaga
   }

  valorAnt=valor;
}

// ==============
// Funciones
// ==============

// Tomar foto 
void take_picture( int altura ){
    
     // Tomo fotografía
     camera_fb_t * fb = NULL;
     
     // Take Picture with Camera
     fb = esp_camera_fb_get(); 
     if(!fb) { // Si no ha tomado la imagen
            // Serial.println("Camera capture failed");
            return;
     }
     
     String path = "/imagen" + String(cuenta) +"_"+String(altura)+".jpg";
      
     fs::FS &fs = SD_MMC; 
      
     
     File file = fs.open(path.c_str(),FILE_WRITE);
      
     if(!file){
            // Serial.printf(" Failed to open file in writing mode: %s\n", path.c_str());
            cuenta = cuenta + 0;
      } 
      else {
            file.write(fb->buf, fb->len); // payload (image), payload length
            // Serial.printf(" Saved: %s\n", path.c_str());
      }
     
     file.close();
     
     esp_camera_fb_return(fb);   
     
     // Turns off the ESP32-CAM white on-board LED (flash) connected to GPIO 4
     digitalWrite(4,LOW);
}

// --> 'init_microSD_card()' function
void init_microSD_card( void ){
      if(!SD_MMC.begin()){
            // Serial.println("SD Card Mount Failed");
            return;
      }
      uint8_t cardType = SD_MMC.cardType();
      if(cardType == CARD_NONE){
            // Serial.println("No SD Card attached");
            return;
      }
      // Serial.println("Ok!");
}
