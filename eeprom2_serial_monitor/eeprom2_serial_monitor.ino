#include <EEPROM.h>

// Define EEPROM size and address for storing message
const int EEPROM_SIZE = 512;
const int messageAddress = 0; // Starting address in EEPROM

void setup() {
  Serial.begin(115200);

  // Initialize EEPROM
  EEPROM.begin(EEPROM_SIZE);

  // Retrieve and display the stored message on startup
  String storedMessage = readStringFromEEPROM(messageAddress);
  if (storedMessage.length() > 0) {
    Serial.println("Message stored in EEPROM:");
    Serial.println(storedMessage);
  } else {
    Serial.println("No message stored in EEPROM.");
  }

  Serial.println("\nEnter a new message to store in EEPROM:");
}

void loop() {
  // Check if there's any input in the Serial Monitor
  if (Serial.available() > 0) {
    // Read the input message
    String newMessage = Serial.readStringUntil('\n'); // Read until a newline character
    newMessage.trim(); // Remove leading and trailing whitespace or newline characters

    // Store the message in EEPROM
    writeStringToEEPROM(messageAddress, newMessage);
    EEPROM.commit();

    // Display confirmation
    Serial.println("New message stored in EEPROM:");
    Serial.println(newMessage);
  }
}

// Function to write a string to EEPROM
void writeStringToEEPROM(int addr, const String &data) {
  for (int i = 0; i < data.length(); i++) {
    EEPROM.write(addr + i, data[i]);
  }
  EEPROM.write(addr + data.length(), '\0'); // Add null terminator
}

// Function to read a string from EEPROM
String readStringFromEEPROM(int addr) {
  String data = "";
  char ch;
  while ((ch = EEPROM.read(addr++)) != '\0' && data.length() < EEPROM_SIZE) {
    data += ch;
  }
  return data;
}
