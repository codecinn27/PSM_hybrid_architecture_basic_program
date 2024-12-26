#include <ESP8266WiFi.h>  // For ESP8266
#include <ESP8266mDNS.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "honor";
const char* password = "hyc12345";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());  // Print the IP address
  
  // Start mDNS responder
  if (MDNS.begin("wemos")) {  // Local address: http://wemos.local
    Serial.println("mDNS responder started");
  }

  // Define web server route
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Hello from Wemos D1 Mini!");
  });

  // Start the server
  server.begin();
}

void loop() {
  // mDNS update (optional for stability)
  MDNS.update();
}