#include <ESP8266WiFi.h>
#include <AsyncMqttClient.h>

// Wi-Fi credentials
const char* ssid = "1Oz6-2.4G";
const char* password = "1_Oz6@915";

// MQTT broker details
const char* mqttHost = "20.2.65.144";
const int mqttPort = 1883;
const char* topic = "test/topic";

AsyncMqttClient mqttClient;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  WiFi.onEvent([](WiFiEvent_t event) {
    if (event == WIFI_EVENT_STAMODE_GOT_IP) {
      Serial.println("Connected to Wi-Fi");
      mqttClient.connect(); // Connect to MQTT broker when Wi-Fi is ready
    }
  });

  // Configure MQTT client
  mqttClient.onConnect([](bool sessionPresent) {
    Serial.println("Connected to MQTT broker");
    mqttClient.subscribe(topic, 1); // Subscribe to a topic, 1=  QoS 1 
  });
  
  mqttClient.onMessage([](char* topic, char* payload, AsyncMqttClientMessageProperties properties,
                          size_t len, size_t index, size_t total) {
    Serial.print("Message received on topic: ");
    Serial.println(topic);
    Serial.println("Payload: " + String(payload));
  });

  mqttClient.setServer(mqttHost, mqttPort); // Set MQTT broker details
}

void loop() {
  // Main loop remains free for other tasks
  delay(10); // Allow background processes to run
}
