#include <VirtualWire.h>
void setup()
{
vw_setup(2000); // Bits per sec
}

void loop()
{
const char *msg = "hola, primer mensaje";
vw_send((uint8_t *)msg, strlen(msg));
delay(400);
}
