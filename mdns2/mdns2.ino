#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <ESPmDNS.h>
#else
#error "Board not found"
#endif

#include <ESPAsyncWebServer.h>

AsyncWebServer server(80); // server port 80

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Page Not found");
}

void setup(void)
{
  
  Serial.begin(115200);
  
  WiFi.softAP("techiesms", "");  // Wifi.softAP("ssid wifi name","no password")
  Serial.println("softap");
  Serial.println("");
  Serial.println(WiFi.softAPIP());

  // MDNS.addService("_http", "_tcp", 80);

  if (MDNS.begin("ESP")) { //esp.local/
    Serial.println("MDNS responder started");
  }


  server.on("/", [](AsyncWebServerRequest * request)
  { 
   String message = "hello world"; 
  request->send(200, "text/plain", message);
  });

  server.on("/page1", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
   String message = "Welcome to page1"; 
  request->send(200, "text/plain", message);
  });

  server.onNotFound(notFound);

  server.begin();  // it will start webserver
}


void loop(void)
{
}

/*
https://youtu.be/cWZP7Y8qP6E?si=9AFv781QkHo8SiVT

lv 1: create a wifi hotspot using the mcu then those device that connect to the same hotspot
can access to the webpage through the ipaddress

lv 2: uncomment the line 31, to run the mdns so you can just type the domain name
instead of the ipaddress to the machine
*/