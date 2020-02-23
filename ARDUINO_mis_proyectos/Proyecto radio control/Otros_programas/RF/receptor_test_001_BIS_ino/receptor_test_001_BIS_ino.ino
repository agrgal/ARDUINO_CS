#include <VirtualWire_Config.h>
#include <VirtualWire.h>

long recibidos = 0;

void setup()
{
Serial.begin(9600);
Serial.println("setup");
vw_setup(2000); // Bits per sec
vw_rx_start();
// Start the receiver PLL running
}
void loop()
{
uint8_t buf[VW_MAX_MESSAGE_LEN];
uint8_t buflen = VW_MAX_MESSAGE_LEN;
if (vw_get_message(buf, &buflen)) // Non-blocking
{
  recibidos++;
int i;
// Message with a good checksum received, dump HEX
Serial.print("Got ");
Serial.print(recibidos);
Serial.print(" ");
for (i = 0; i < buflen; i++)
{
Serial.print(char(buf[i]));
Serial.print(" ");
}
Serial.println("");
}
}
