#include "Setting.h"
#include "ModelaVariables.h"
#include "ModelaConfigMode.h"
#include "ModelaWebservices.h"
#include "ModelaSensors.h"
#include "ModelaController.h"
#include "ModelaUtilityFunction.h"

class Preparation {
public:
  void BeginSetup() {
    Serial.begin(115200);
    Serial.println("Modela New App is Starting...\n");
    Serial.println("Board Firmware Version: " + String(BOARD_FIRMWARE_VERSION));
    Serial.println("Board Hardware Version: " + String(BOARD_HARDWARE_VERSION));
    Serial.println("Device Type ID: " + String(DEVICE_TYPE_ID));
    Serial.println("Device Version: " + String(DEVICE_VERSION));
    Serial.println("\n");

    initBoardPins(); // Initialize pins for switches, relays, and sensors
    initSensor();    // Initialize sensors
    initEEPROM();    // Initialize EEPROM for data storage
    PhysicalButton();
    setupMode(mode); // Set operating mode

    if (mode == 2) {  // If WiFi mode
      setupMQTT(); // Setup MQTT connection
      if (isFirstInit) {
        firstConnectHTTP(); // Set initial values from the server
      }
      currentTimeSetup(); // Setup current time
    }
    // initConfig();
  }

  void run() {
    // Real-time board switches
    PhysicalButton();
    ResetButton();

    switch (mode)
    {
    case 1:
      runBLE();    // BLE mode
      debugMode(); // Debug mode
      break;
    case 2:
      runWiFi();   // WiFi mode
      debugMode(); // Debug mode
      printLocalTime();
      break;
    default:
      Serial.println("Mode is not set.");
      Serial.println("Hard Set to BLE Mode.");
      runBLE(); // Default to BLE mode
      break;
    }
  }
};

Preparation ModelaConfig;
