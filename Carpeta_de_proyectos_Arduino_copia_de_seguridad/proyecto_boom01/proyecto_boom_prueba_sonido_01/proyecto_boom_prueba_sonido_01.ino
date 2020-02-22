#include <DFMiniMp3.h>
#include <SoftwareSerial.h>


SoftwareSerial DFPlayerSerial(0, 1); // RX, TX

/*
mp3_play();    //start play
mp3_play(5);  //play "mp3/0005.mp3"
mp3_pause();
mp3_stop();
mp3_next();   
mp3_prev();

mp3_set_volume(uint16_t volume);  //0~30
mp3_set_EQ(); //0~5
void mp3_single_loop(boolean state);  //set single loop
void mp3_random_play();
*/

void setup()
{
  Serial.begin(9600);
  DFPlayerSerial.begin(9600);
  mp3_set_serial(DFPlayerSerial);
  mp3_set_volume(15);
}

void loop()
{
  mp3_play(1);
  delay(6000);
  mp3_next();
  delay(6000);
  mp3_prev();
  delay(6000);
  mp3_play(4);
  delay(6000);
}
