#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <Hash.h>

#define potentialMeter A0
#define WIFI_SSID "1Oz6-2.4G"
#define WIFI_PASSWORD "1_Oz6@915"

String old_value, value;
ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// HTML template for displaying potentiometer value
char html_template[] PROGMEM = R"=====( 
<html lang="en">
   <head>
      <meta charset="utf-8">
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <title>MrDIY Socket Example</title>
      <script>
        socket = new WebSocket("ws:/" + "/" + location.host + ":81");
        socket.onopen = function(e) { console.log("[socket] socket.onopen "); };
        socket.onerror = function(e) { console.log("[socket] socket.onerror "); };
        socket.onmessage = function(e) { 
            console.log("[socket] " + e.data);
            document.getElementById("mrdiy_value").innerHTML = e.data;
        };
      </script>
   </head>
   <body style="max-width:400px;margin: auto;font-family:Arial, Helvetica, sans-serif;text-align:center">
      <div><h1><br />My Value</h1></div>
      <div><p id="mrdiy_value" style="font-size:100px;margin:0"></p></div>
   </body>
</html>
)=====";

// HTML template for ON/OFF buttons
char html_buttons[] PROGMEM = R"=====( 
<html lang="en">
   <head>
      <meta charset="utf-8">
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <title>Control Buttons</title>
      <script>
        socket = new WebSocket("ws:/" + "/" + location.host + ":81");
        function sendMessage(msg) {
            if (socket.readyState === WebSocket.OPEN) {
                socket.send(msg);
            } else {
                console.log("Socket is not open.");
            }
        }
      </script>
   </head>
   <body style="text-align:center; font-family:Arial, Helvetica, sans-serif; margin-top:50px;">
      <h1>Control Panel</h1>
      <button style="font-size:20px; padding:10px 20px;" onclick="sendMessage('ON')">ON</button>
      <button style="font-size:20px; padding:10px 20px;" onclick="sendMessage('OFF')">OFF</button>
   </body>
</html>
)=====";

// Handle WebSocket events
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        webSocket.sendTXT(num, "0");
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] Received message: %s\n", num, payload);
      break;

      /*
      let binaryData = new Uint8Array([0x01, 0x02, 0x03, 0x04]); // Example binary data
      socket.send(binaryData.buffer);
      */ 
    case WStype_BIN:
      Serial.printf("[%u] Received binary data length: %u\n", num, length);
      break;

  }
}

// Handle main page request
void handleMain() {
  server.send_P(200, "text/html", html_template);
}

// Handle control buttons page request
void handleButtons() {
  server.send_P(200, "text/html", html_buttons);
}

// Handle 404 errors
void handleNotFound() {
  server.send(404, "text/html", "<html><body><p>404 Error</p></body></html>");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  // Print IP address after successful connection
  Serial.println("Connected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  server.on("/", handleMain);         // Serve potentiometer page
  server.on("/control", handleButtons); // Serve control buttons page
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  webSocket.loop();
  server.handleClient();
  value = String(analogRead(potentialMeter));
  webSocket.broadcastTXT(value);
  delay(50);
}
