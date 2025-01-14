// clearing eeprom and clearing wifi crendential in NVS (Non-Volatile Storage)

#include <EEPROM.h>
// #include <WiFi.h> //esp32
#include <ESP8266WiFi.h> //esp8266

void setup() {
  Serial.begin(115200);

  // Start EEPROM with size 512
  EEPROM.begin(512);

  // Loop through and clear all EEPROM addresses
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);  // Set each byte to 0
  }

  // Commit changes to EEPROM
  EEPROM.commit();
  Serial.println("EEPROM has been cleared!");

  // Clear Wi-Fi credentials from NVS
  WiFi.disconnect(true); // Disconnect from Wi-Fi and clear stored credentials
  Serial.println("Wi-Fi credentials have been cleared!");

  // Optionally, reboot to ensure everything is reset and to start fresh
  // This will make the device go through the Wi-Fi setup process again
  ESP.restart(); 
}

void loop() {
  // Do nothing in loop
}
