#include <CayenneEthernet.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <BlynkWidgets.h>
#include <BlynkSimpleWildFire.h>
#include <BlynkSimpleTI_TivaC_Connected.h>
#include <BlynkSimpleShieldEsp8266_HardSer.h>
#include <BlynkApiArduino.h>
#include <WidgetLED.h>
#include <CayenneYun.h>
#include <BlynkSimpleParticle.h>
#include <WidgetLCD.h>
#include <CayenneWiFi101.h>
#include <BlynkSimpleEthernetV2_0.h>
#include <BlynkParticle.h>
#include <CayenneClient.h>
#include <BlynkSimpleTI_CC3200_LaunchXL.h>
#include <BlynkSimpleLinkItONE.h>
#include <CayenneDefines.h>
#include <CayenneWiFiClient.h>
#include <BlynkSimpleEsp8266_SSL.h>
#include <BlynkSimpleTinyDuino.h>
#include <BlynkSimpleUIPEthernet.h>
#include <BlynkSimpleWifi.h>
#include <BlynkSimpleWiFiShield101_SSL.h>
#include <CayenneEthernetW5500.h>
#include <BlynkSimpleWiFly.h>
#include <CayenneTMP102.h>
#include <WidgetTerminal.h>
#include <BlynkSimpleRedBear_Duo.h>
#include <BlynkSimpleEnergiaWiFi.h>
#include <BlynkSimpleRBL_CC3200.h>
#include <BlynkSimpleCC3000.h>
#include <BlynkApiParticle.h>
#include <BlynkSimpleUserDefined.h>
#include <BlynkSimpleEsp8266.h>
#include <CayenneEthernetClient.h>
#include <WidgetBridge.h>
#include <CayenneVCNL4000.h>
#include <CayenneEthernetW5200.h>
#include <BlynkSimpleEthernet2.h>
#include <CayenneWiFi.h>
#include <CayenneTypes.h>
#include <BlynkSimpleEthernet.h>
#include <WidgetSD.h>
#include <BlynkSimpleYun.h>
#include <BlynkSimpleIntelEdisonWiFi.h>
#include <BlynkSimpleRBL_WiFi_Mini.h>
#include <BlynkSimpleSerial.h>
#include <BlynkSimpleWiFiShield101.h>
#include <WidgetRTC.h>
#include <CayenneSerial.h>
#include <CayenneTemperature.h>
#include <BlynkSimpleShieldEsp8266_SoftSer.h>

/*
Cayenne Ethernet Manual IP Example

This sketch connects to the Cayenne server using an Arduino Ethernet Shield W5100 and a 
manually specified MAC address, IP and gateway.

The Cayenne Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Set the token variable to match the Arduino token from the Dashboard.
2. Compile and upload this sketch.

For Cayenne Dashboard widgets using digital or analog pins this sketch will automatically
send data on those pins to the Cayenne server. If the widgets use Virtual Pins, data
should be sent to those pins using virtualWrites. Examples for sending and receiving
Virtual Pin data are under the Basics folder.
*/

//#define CAYENNE_DEBUG             // Uncomment to show debug messages
#define CAYENNE_PRINT Serial        // Comment this out to disable prints and save space
#include <CayenneEthernet.h>        // Comment this out if you uncomment a different Ethernet device below.
//#include <CayenneEthernetW5500.h> // Uncomment this and comment out CayenneEthernet.h to use an Ethernet 2 shield or other Ethernet W5500 shield.
                                    // You will need the Ethernet2 library installed. See the ArduinoEthernetW5500 example sketch for more info.
//#include <CayenneEthernetW5200.h> // Uncomment this and comment out CayenneEthernet.h to use an Ethernet W5200 shield.
                                    // You will need the EthernetW5200 library installed. See the ArduinoEthernetW5200 example sketch for more info.

// Cayenne authentication token. This should be obtained from the Cayenne Dashboard.
char token[] = "s9nw3hzsot";

// Mac address should be different for each device in your LAN
byte arduino_mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress arduino_ip(10, 0, 0, 20);
IPAddress dns_ip(8, 8, 8, 8);
IPAddress gateway_ip(10, 0, 0, 1);
IPAddress subnet_mask(255, 255, 255, 0);

void setup()
{
  Serial.begin(9600);
  Cayenne.begin(token, arduino_ip, dns_ip, gateway_ip, subnet_mask, arduino_mac);
}

void loop()
{
  Cayenne.run();
}
