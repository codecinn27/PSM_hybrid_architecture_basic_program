#include <EEPROM.h>
#include <ESP8266WiFi.h>

#define EEPROM_SIZE 96
#define SSID_ADDR 0   // Starting address for SSID
#define PASS_ADDR 32  // Starting address for Password
#define FLAG_ADDR 64  // Address to store connection flag

void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  // Check if credentials exist in EEPROM
  if (EEPROM.read(FLAG_ADDR) == 1) {
    char ssid[32], pass[32];
    readEEPROM(SSID_ADDR, ssid, 32);
    readEEPROM(PASS_ADDR, pass, 32);

    Serial.println("Connecting to saved Wi-Fi credentials...");
    WiFi.begin(ssid, pass);

    if (waitForWiFiConnection(10000)) {
      Serial.println("Connected!");
    } else {
      Serial.println("Failed to connect. Entering Smart Config mode...");
      startSmartConfig();
    }
  } else {
    Serial.println("No Wi-Fi credentials found. Entering Smart Config mode...");
    startSmartConfig();
  }
}

void loop() {
  // Place your main logic here
}

void startSmartConfig() {
  WiFi.mode(WIFI_STA);
  WiFi.beginSmartConfig();

  while (!WiFi.smartConfigDone()) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nSmart Config received.");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("Password: ");
  Serial.println(WiFi.psk());

  // Save credentials in EEPROM
  writeEEPROM(SSID_ADDR, WiFi.SSID().c_str());
  writeEEPROM(PASS_ADDR, WiFi.psk().c_str());
  EEPROM.write(FLAG_ADDR, 1);
  EEPROM.commit();

  Serial.println("Credentials saved in EEPROM.");
}

bool waitForWiFiConnection(unsigned long timeout) {
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - start >= timeout) {
      return false;
    }
    delay(500);
    Serial.print(".");
  }
  return true;
}

void writeEEPROM(int startAddress, const char* data) {
  for (int i = 0; i < 32; i++) {
    if (i < strlen(data)) {
      EEPROM.write(startAddress + i, data[i]);
    } else {
      EEPROM.write(startAddress + i, 0); // Write null character for padding
    }
  }
}

void readEEPROM(int startAddress, char* data, int len) {
  for (int i = 0; i < len; i++) {
    data[i] = EEPROM.read(startAddress + i);
  }
  data[len - 1] = '\0'; // Ensure null-terminated string
}
