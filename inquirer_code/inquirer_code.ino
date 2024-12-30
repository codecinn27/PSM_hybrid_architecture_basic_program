/*
ESPmDNS.h (used for ESP32) cannot directly discover services running 
on ESP8266 devices using ESP8266mDNS.h. 
However, mDNS is a standard protocol, so ESP32 (using ESPmDNS.h) 
can discover ESP8266 services (using ESP8266mDNS.h) 
if both devices are properly set up.


*/

#include <ESPmDNS.h>
#include <WiFi.h>
 
const char* ssid = "1Oz6-2.4G";
const char* password =  "1_Oz6@915";
 
void setup() {
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Connecting...\n\n");
  }
 
  if(mdns_init()!= ESP_OK){
    Serial.println("mDNS failed to start");
    return;
  }

  if (!MDNS.begin("wemos1")) {
    Serial.println("mDNS responder failed");
    return;
  }
  Serial.println("mDNS responder started");
 
  int nrOfServices = MDNS.queryService("http", "tcp");
   
  if (nrOfServices == 0) {
    Serial.println("No services were found.");
  } 
   
  else {
     
    Serial.print("Number of services found: ");
    Serial.println(nrOfServices);
     
    for (int i = 0; i < nrOfServices; i=i+1) {
 
      Serial.println("---------------");
       
      Serial.print("Hostname: ");
      Serial.println(MDNS.hostname(i));
 
      // Resolve IP address by hostname
      IPAddress resolvedIP;
      if (MDNS.queryHost(MDNS.hostname(i), resolvedIP)) {
        Serial.print("IP address: ");
        Serial.println(resolvedIP.toString());
      } else {
        Serial.println("Failed to resolve IP");
      }
 
      Serial.print("Port: ");
      Serial.println(MDNS.port(i));
 
      Serial.println("---------------");
    }
  }
 
}
 
void loop() {}
