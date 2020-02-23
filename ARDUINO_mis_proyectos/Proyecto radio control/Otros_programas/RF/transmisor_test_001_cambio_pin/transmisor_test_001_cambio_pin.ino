#include <VirtualWire.h>

const int pinData=12;

void setup()
{
vw_setup(2000); // Bits per sec
vw_set_tx_pin(pinData);
}

void loop()
{
const char *msg = "hola, primer mensaje";
vw_send((uint8_t *)msg, strlen(msg));
delay(400);
}
