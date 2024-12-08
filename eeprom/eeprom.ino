#include <EEPROM.h>
#include <WiFi.h>

const char* ssid = "";
const char* password = "";
int ssidAddress = 0;
int passwordAddress = sizeof(ssid);

void setup() {
  EEPROM.begin(512);
  EEPROM.writeString(ssidAddress, ssid);
  EEPROM.writeString(passwordAddress, password);
  EEPROM.commit();
  
  // Rest of the initialization code, WiFi connection, etc.
}