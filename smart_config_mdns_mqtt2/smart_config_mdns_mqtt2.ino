#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

IPAddress mqttHost;
uint16_t mqttPort = 1883;
WiFiClient espClient;
PubSubClient mqttClient(espClient);
String topic = "test/topic";  // Store the topic received from Serial
bool mqttDiscovered = false;  // Flag to track if MQTT broker has been discovered via mDNS

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);  // Set ESP8266 as a WiFi station
  WiFi.beginSmartConfig();

  Serial.println("Waiting for SmartConfig...");

  while (!WiFi.smartConfigDone()) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nSmartConfig received.");
  Serial.print("Connecting to: ");
  Serial.println(WiFi.SSID());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("mDNS responder started");
  }

  mqttDiscovery();
}

void mqttDiscovery() {
  if (mqttDiscovered) {
    return;  // Skip discovery if already done
  }
  Serial.println("Looking for MQTT service via mDNS...");
  int n = MDNS.queryService("mqtt", "tcp");

  if (n > 0) {

    mqttHost = MDNS.IP(0);
    mqttPort = MDNS.port(0);

    Serial.print("Discovered MQTT Broker: ");
    Serial.print(mqttHost);
    Serial.print(":");
    Serial.println(mqttPort);

    mqttClient.setServer(mqttHost, mqttPort);
    mqttClient.setCallback(messageCallback);
    mqttDiscovered = true;  // Set discovery flag
    if (mqttClient.connect("ESP8266Client")) {
      Serial.println("Connected to MQTT broker via mDNS");
      mqttClient.subscribe(topic.c_str());
      Serial.print("Subscribed to topic: ");
      Serial.println(topic);
    } else {
      Serial.println("MQTT connection failed");
    }
  } else {
    Serial.println("No MQTT service found via mDNS");
  }
}

void reconnect() {

  if (!mqttDiscovered) {
    Serial.println("MQTT not discovered yet. Retrying mDNS...");
    mqttDiscovery();  // Attempt discovery again if not done
    if (!mqttDiscovered) {
      delay(5000);  // Wait before retrying discovery
      return;
    }
  }

  while (!mqttClient.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (mqttClient.connect("ESP8266Client")) {
      Serial.println("Reconnected to MQTT broker");
      mqttClient.subscribe(topic.c_str());
    } else {
      Serial.print("Failed to connect. Retry in 5 seconds.");
      delay(5000);
    }
  }
}

void loop() {
  // If MQTT is not connected, attempt to reconnect
  if (!mqttClient.connected()) {
    reconnect();  // Attempt to reconnect only if MQTT is not connected
  } else {
    mqttClient.loop();  // Handle MQTT messages
  }

  // Check for Serial input
  if (Serial.available() > 0) {
    topic = Serial.readStringUntil('\n');  // Read topic from Serial
    topic.trim();  // Remove any trailing newline or spaces

    if (topic.length() > 0 && mqttClient.connected()) {
      mqttClient.subscribe(topic.c_str());
      Serial.print("Subscribed to topic: ");
      Serial.println(topic);
      mqttClient.publish(topic.c_str(), "Test message");  // Publish a test message
    } else {
      Serial.println("Failed to subscribe. MQTT not connected or empty topic.");
    }
  }
}

