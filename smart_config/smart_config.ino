
#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(115200);         // Start serial communication
  delay(1000);

  Serial.println("\nStarting Smart Config...");
  WiFi.mode(WIFI_STA);          // Set the ESP8266 as a station (client)

  WiFi.beginSmartConfig();      // Start Smart Config

  // Wait for Smart Config to complete
  while (!WiFi.smartConfigDone()) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nSmart Config received!");
  Serial.println("Connecting to Wi-Fi...");

  // Wait until connected to Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Print the device's IP address
}

void loop() {
  // Keep the program running
}
