#include "Setting.h"
#include "ModelaVariables.h"
#include "EEPROM_Managers.h"
#include "ModelaConfigMode.h"
#include "ModelaWebservices.h"
#include "ModelaUtilityFunction.h"
#include "ModelaController.h"
#include "ModelaSensors.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <WiFi.h>

BLEServer *pServer;
BLECharacteristic *pCharacteristic;

void initEEPROM() {
  // เริ่มต้นใช้งาน EEPROM ในการเก็บข้อมูล
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("Failed to initialise EEPROM");
    return;
  }

  getAllModelaConfig();
}

void initBoardPins() {
  const int inputPins[] = {SW1, SW2, A0, D5, D6, D7, A1, A2}; // Input Pins
  const int outputPins[] = {RL1, RL2, BOARD_LED_PIN}; // Output Pins
  const int digitalWritePins[] = {RL1, RL2}; // DigitalWrite Pins

  for (int inPin : inputPins) {
    pinMode(inPin, INPUT);
  }

  for (int outPin : outputPins) {
    pinMode(outPin, OUTPUT);
  }

  for (int digiPin : digitalWritePins) {
    digitalWrite(digiPin, HIGH);
  }

  pinMode(BOARD_BUTTON_PIN, INPUT_PULLUP); // ตั้งค่าให้ปุ่มกดเป็น INPUT_PULLUP (สวิทซ์ Reset)
}

void setupMode(int mode) {
  // ตรวจสอบโหมดการทำงานและกำหนดการตั้งค่าตามโหมดนั้น
  switch (mode) {
    case 1:
      setupBLE(); // โหมด BLE
      break;
    case 2:
      setupWiFi(); // โหมด WiFi
      break;
    default:
      Serial.println("Invalid mode. Setting to default BLE mode.");
      setupBLE(); // โหมด BLE
      break;
  }
}

// ============================== Start All Function BLE ============================== //
class MyServerCallbacks : public BLEServerCallbacks
{ // ตรวจสอบว่า connected อยู่หรือไม่ if connected can send datd
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
    Serial.println("Device connected.");
  }

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
    Serial.println("Device disconnected.");
  }
};

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue(); // รับข้อมูลที่เขียนมาจากอุปกรณ์
    if (!value.empty()) { // ตรวจสอบว่ามีข้อมูลหรือไม่
      String receivedValue = String(value.c_str()); // แปลง std::string เป็น String
      Serial.println("Received data from App: " + receivedValue);

      // ประมวลผลคำสั่งต่างๆ ที่ได้รับ
      processCommand(receivedValue);
    }
  }

  void processCommand(const String& command) {
    if (command.startsWith("SSID:")) { // ตรวจสอบว่าคำสั่งเริ่มต้นด้วย "SSID:"
      // หาตำแหน่งของจุลภาคแรก เพื่อแยก SSID และ Password
      int splitIndex1 = command.indexOf(",Password:");
      int splitIndex2 = command.indexOf(",Key:");
      if (splitIndex1 != -1 && splitIndex2 != -1) { // ตรวจสอบว่าพบจุลภาคหรือไม่
        String ssidPart = command.substring(0, splitIndex1); // ส่วนของ SSID
        String passPart = command.substring(splitIndex1 + 10, splitIndex2); // ส่วนของ Password
        String tokenPart = command.substring(splitIndex2 + 5); // ส่วนของ Token

        // แยกค่า SSID และ Password และ Token จากสตริง
        String ssid = ssidPart.substring(5); // ตัด "SSID:" ทิ้ง
        String password = passPart;
        String token = tokenPart;

        Serial.print("Received SSID: ");
        Serial.println(ssid);
        Serial.print("Received Password: ");
        Serial.println(password);
        Serial.print("Received Token: ");
        Serial.println(token);

        setModelaConfigWifi(ssid, password);
        setModelaToken(token);
        setModelaConfigMode(2);
        ESP.restart(); // รีสตาร์ทเพื่อใช้การตั้งค่าใหม่
      }
    } else if (command == "on") {
      // RelayOn(1); // เปิดรีเลย์ 1
      Serial.println("Relay 1 is ON");
    } else if (command == "off") {
      // RelayOff(1); // ปิดรีเลย์ 1
      Serial.println("Relay 1 is OFF");
    } else if (command == "on2") {
      // RelayOn(2); // เปิดรีเลย์ 2
      Serial.println("Relay 2 is ON");
    } else if (command == "off2") {
      // RelayOff(2); // ปิดรีเลย์ 2
      Serial.println("Relay 2 is OFF");
    } //else if (command == "on3") {
    //   RelayOn(3); // เปิดรีเลย์ 3
    //   Serial.println("Relay 3 is ON");
    // } else if (command == "off3") {
    //   RelayOff(3); // ปิดรีเลย์ 3
    //   Serial.println("Relay 3 is OFF");
    // } else if (command == "on4") {
    //   RelayOn(4); // เปิดรีเลย์ 4
    //   Serial.println("Relay 4 is ON");
    // } else if (command == "off4") {
    //   RelayOff(4); // ปิดรีเลย์ 4
    //   Serial.println("Relay 4 is OFF");
    // } else if (command == "on5") {
    //   RelayOn(5); // เปิดรีเลย์ 5
    //   Serial.println("Relay 5 is ON");
    // } else if (command == "off5") {
    //   RelayOff(5); // ปิดรีเลย์ 5
    //   Serial.println("Relay 5 is OFF");
    // }
  }
};
// ============================== End All Function BLE ============================== //

// ===== Start Set BLE ===== //
void setupBLE() {
  // ตั้งค่า Bluetooth Low Energy (BLE)
  BLEDevice::init(DEVICE_BT_NAME);
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID));
  pCharacteristic = pService->createCharacteristic(
      BLEUUID(CHARACTERISTIC_UUID),
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  Serial.println("BLE server started. Waiting for connections...");
}
// ===== End Set BLE ===== //

// ===== Start Set Wifi ===== //
void setupWiFi() {
  Serial.print("Connecting to WiFi");
  
  WiFi.begin(ssid.c_str(), password.c_str());
  failConnectedStr = readEEPROMFromPosition(101);
  failConnected = failConnectedStr.toInt();
  Serial.println("failConnected : " + String(failConnected));

  unsigned long startAttemptTime = millis(); // จับเวลาเริ่มต้นการพยายามเชื่อมต่อ
  int wifi_connect_timeout = (failConnected > 2) ? 10000 : 5000; // ตั้งค่าเวลาเชื่อมต่อ Wifi ไม่เกิน 5 วินาที ในครั้งที่ 1 และ 2 และ 10 วินาที ในครั้งที่ 3 ขึ้นไป

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < wifi_connect_timeout) {
    blinkLED(250, ".");
  }

  if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Failed to connect to WiFi. Please try again.");
      failConnected = (failConnected >= 6) ? 0 : failConnected + 1; // ถ้าเชื่อมต่อ Wifi ไม่สำเร็จ ให้เพิ่มค่า failConnected ขึ้น 1
      // setModelaConfigMode((failConnected >= 6) ? 1 : mode); // ถ้าเชื่อมต่อ Wifi ไม่สำเร็จ มากกว่า 6 ครั้ง ให้เปลี่ยนเป็น Mode BLE
      setModelaFailConnected(failConnected);
      delay(500);
      if (!failConnected){
        setupBLE(); // ถ้าเชื่อมต่อ Wifi ไม่สำเร็จ มากกว่า 6 ครั้ง ให้เปลี่ยนเป็น Mode BLE
        mode = 1;
      }
      else {
        ESP.restart();
      }
  } else {
      Serial.println();
      Serial.print("Connected to WiFi. IP address: ");
      Serial.println(WiFi.localIP());
      setModelaFailConnected(0); // reset fail count on success
  }
}
// ===== End Set Wifi ===== //

void runBLE() {
  blinkLED(1000, ".");
  // // รันโหมด BLE
  // if (deviceConnected) {
  //   // ส่งข้อมูลไปยังอุปกรณ์ที่เชื่อมต่อ
  //   pCharacteristic->setValue("Hello from ModelaSmartControl-BT");
  //   pCharacteristic->notify();
  //   delay(1000); // หน่วงเวลา 1 วินาที
  // }
}

void runSensor() {
  unsigned long sendSensorMQTTInterval = 5000;

  // อัพเดทค่า Sensor ทุก 5 วินาที
  if (millis() - lastSendSensorMQTTTime >= sendSensorMQTTInterval) {
    if (WiFi.status() == WL_CONNECTED) {
      ValueSensorControl(SensorAmount);
      // Serial.println("ValueSensorControl : Sensor Amount : " + String(SensorAmount));
      valueSensorDataSendMQTT(SensorAmount);
      // Serial.println("valueSensorDataSendMQTT : Sensor Amount : " + String(SensorAmount));
    }
    lastSendSensorMQTTTime = millis(); // อัพเดตเวลาล่าสุดที่ส่งข้อมูล
    additionOLED();
  }
}

void runStatusSensor() {
  unsigned long sendStatusSensorMQTTInterval = 2000;

  // อัพเดทค่า Sensor ทุก 5 วินาที
  if (millis() - lastSendStatusSensorMQTTTime >= sendStatusSensorMQTTInterval) {
    if (WiFi.status() == WL_CONNECTED) {
      ValueStatusSensorControl(StatusSensorAmount);
      valueStatusSensorDataSendMQTT(StatusSensorAmount);
    }
    lastSendStatusSensorMQTTTime = millis(); // อัพเดตเวลาล่าสุดที่ส่งข้อมูล
  }
}

void runActionRelay() {
  unsigned long intervalActionRelay = 2000;

  // อัพเดทสถานะของรีเลย์ทุก 0.5 วินาที
  if (millis() - lastActionRelay >= intervalActionRelay) {
    if (WiFi.status() == WL_CONNECTED) {
      ActionRelaySensor(SensorAmount); // ควบคุมรีเลย์ตามค่าเซ็นเซอร์
      ActionRelayStatusSensor(StatusSensorAmount); // ควบคุมรีเลย์ตามค่าเซ็นเซอร์
    }
    lastActionRelay = millis();
  }
}

void runWiFi() {
  // รันโหมด WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Disconnected");
    setupWiFi(); // ถ้าหลุดจาก WiFi ให้เชื่อมต่อใหม่
  }
  else {
    if (isFirstInit) {
      // Set ค่าเริ่มต้นจาก Server
      firstConnectHTTP();
    }
    runMQTT(); // รัน MQTT
    runSensor(); // รัน Sensors
    runStatusSensor(); // รัน Status Sensor
    runActionRelay(); // รัน Action Relay
  }
}

// ========================= Start Debug Mode ========================= //
void valueSensorDebug(int numSensor) {
  // แสดงค่า Sensor แต่ละ Value
  for (int i = 1; i <= numSensor; i++) {
    // Serial.println("Selected Sensor " + String(i) + " : " + String(SensorSelected[i]) + " MainValue " + String(i) + " : " + String(MainSensorValue[i]) + " SecondValue " + String(i) + " : " + String(SecondSensorValue[i]));
    Serial.println("Selected Sensor " + String(i) + " : " + String(SensorSelected[i]) + " MainValue " + String(i) + " : " + String(MainSensorValue[i]));
  }
}

void statusSensorDebug(int numStatusSensor) {
  // แสดงค่า Status Sensor แต่ละ Value
  for (int i = 1; i <= numStatusSensor; i++) {
    Serial.println("Selected Status Sensor " + String(i) + " : " + String(StatusSensorSelected[i]) + " StatusValue " + String(i) + " : " + String(StatusSensorValue[i]));
  }
}

void sensorSelectedDebug(int numSensor) {
  for (int i = 1; i <= numSensor; i++) {
    Serial.println("Selected Sensor " + String(i) + " : " + String(valueSensor[i].SensorSelected));
  }
}

void debugMode() {
  if(runDebugMode) {
    unsigned long currentSendDebugTime = millis();
    unsigned long sendDebugInterval = 5000; // 5 วินาที

    if (currentSendDebugTime - lastSendDebugTime >= sendDebugInterval) {
      // โหมด Debug
      // Serial.println("______________________________\n");
      // Serial.println("Start Debug Mode...\n");
      // Serial.println("Mode : " + String(mode));
      // if (mode == 2) {
      //   Serial.println("WiFi SSID : " + String(ssid) + " | WiFi Password : " + String(password));
      //   Serial.println("Token : " + String(tokenKey));
      // }
      // Serial.println("______________________________\n");
      // Serial.println("Fail Connected : " + String(failConnected) + " | Device Connected : " + String(deviceConnected));
      // Serial.println("isFirstInit : " + String(isFirstInit));
      // Serial.println("______________________________\n");
      // valueSensorDebug(SensorAmount);
      // sensorSelectedDebug(SensorAmount);
      // Serial.println("______________________________\n");
      // statusSensorDebug(StatusSensorAmount);
      // Serial.println("______________________________\n");
      // Serial.println("Last Send Debug Time : " + String(lastSendDebugTime));
      // Serial.println("End Debug Mode...");
      // Serial.println("______________________________\n");

      // // แสดงจำนวน SensorAmount
      // Serial.print("SensorAmount: ");
      // Serial.println(SensorAmount);

      // // แสดงข้อมูลของ valueSensor
      // for (int i = 1; i <= SensorAmount; i++) {
      //   Serial.print("Sensor ");
      //   Serial.print(i);
      //   Serial.println(":");
      //   Serial.print("  SensorNO: ");
      //   Serial.println(valueSensor[i].SensorNO);
      //   Serial.print("  SensorSelected: ");
      //   Serial.println(valueSensor[i].SensorSelected);
      //   Serial.print("  SensorNotification: ");
      //   Serial.println(valueSensor[i].SensorNotification);
      //   Serial.print("  SensorValueOption: ");
      //   Serial.println(valueSensor[i].SensorValueOption);
      //   for (int j = 0; j < 5; j++) {
      //     Serial.print("  SensorValueControl[");
      //     Serial.print(j);
      //     Serial.print("]: ");
      //     Serial.println(valueSensor[i].SensorValueControl[j]);
      //   }
      //   Serial.print("  HighLimitSensorValue: ");
      //   Serial.println(valueSensor[i].HighLimitSensorValue);
      //   Serial.print("  LowLimitSensorValue: ");
      //   Serial.println(valueSensor[i].LowLimitSensorValue);
      //   Serial.print("  CalibrateSensorValue: ");
      //   Serial.println(valueSensor[i].CalibrateSensorValue);
      //   Serial.print("  TimerStatus: ");
      //   Serial.println(valueSensor[i].TimerStatus);
      //   for (int j = 0; j < 10; j++) {
      //     Serial.print("  TimerNo[");
      //     Serial.print(j);
      //     Serial.print("]: ");
      //     Serial.println(valueSensor[i].TimerNo[j]);
      //     Serial.print("  TimerHourBegin[");
      //     Serial.print(j);
      //     Serial.print("]: ");
      //     Serial.println(valueSensor[i].TimerHourBegin[j]);
      //     Serial.print("  TimerMinBegin[");
      //     Serial.print(j);
      //     Serial.print("]: ");
      //     Serial.println(valueSensor[i].TimerMinBegin[j]);
      //     Serial.print("  TimerHourEnd[");
      //     Serial.print(j);
      //     Serial.print("]: ");
      //     Serial.println(valueSensor[i].TimerHourEnd[j]);
      //     Serial.print("  TimerMinEnd[");
      //     Serial.print(j);
      //     Serial.print("]: ");
      //     Serial.println(valueSensor[i].TimerMinEnd[j]);
      //   }
      // }

      // // แสดงจำนวน StatusSensorAmount
      // Serial.print("StatusSensorAmount: ");
      // Serial.println(StatusSensorAmount);

      // // แสดงข้อมูลของ statusSensor
      // for (int i = 1; i <= StatusSensorAmount; i++) {
      //   Serial.print("Status Sensor ");
      //   Serial.print(i);
      //   Serial.println(":");
      //   Serial.print("  StatusSensorSelected: ");
      //   Serial.println(statusSensor[i].StatusSensorSelected);
      //   Serial.print("  StatusSensorNotification: ");
      //   Serial.println(statusSensor[i].StatusSensorNotification);
      //   Serial.print("  StatusSensorValueOption: ");
      //   Serial.println(statusSensor[i].StatusSensorValueOption);
      //   for (int j = 0; j < 5; j++) {
      //     Serial.print("  StatusSensorValueControl[");
      //     Serial.print(j);
      //     Serial.print("]: ");
      //     Serial.println(statusSensor[i].StatusSensorValueControl[j]);
      //   }
      //   Serial.print("  TimerStatus: ");
      //   Serial.println(statusSensor[i].TimerStatus);
      //   for (int j = 0; j < 10; j++) {
      //     Serial.print("  TimerNo[");
      //     Serial.print(j);
      //     Serial.print("]: ");
      //     Serial.println(statusSensor[i].TimerNo[j]);
      //     Serial.print("  TimerHourBegin[");
      //     Serial.print(j);
      //     Serial.print("]: ");
      //     Serial.println(statusSensor[i].TimerHourBegin[j]);
      //     Serial.print("  TimerMinBegin[");
      //     Serial.print(j);
      //     Serial.print("]: ");
      //     Serial.println(statusSensor[i].TimerMinBegin[j]);
      //     Serial.print("  TimerHourEnd[");
      //     Serial.print(j);
      //     Serial.print("]: ");
      //     Serial.println(statusSensor[i].TimerHourEnd[j]);
      //     Serial.print("  TimerMinEnd[");
      //     Serial.print(j);
      //     Serial.print("]: ");
      //     Serial.println(statusSensor[i].TimerMinEnd[j]);
      //   }
      // }

      // แสดงจำนวน timerAmount
      Serial.print("timerAmount: ");
      Serial.println(timerAmount);

      // แสดงข้อมูลของ timerSwitch
      for (int i = 1; i <= timerAmount; i++) {
        Serial.print("Timer Switch ");
        Serial.print(i);
        Serial.println(":");
        Serial.print("  timerNo: ");
        Serial.println(timerSwitch[i].timerNo);
        Serial.print("  timerOption: ");
        Serial.println(timerSwitch[i].timerOption);
        for (int j = 0; j < 5; j++) {
          Serial.print("  timerControl[");
          Serial.print(j);
          Serial.print("]: ");
          Serial.println(timerSwitch[i].timerControl[j]);
        }
        for (int j = 0; j < 7; j++) {
          Serial.print("  controlDay[");
          Serial.print(j);
          Serial.print("]: ");
          Serial.println(timerSwitch[i].controlDay[j]);
        }
        for (int j = 0; j < 2; j++) {
          Serial.print("  controlHour[");
          Serial.print(j);
          Serial.print("]: ");
          Serial.println(timerSwitch[i].controlHour[j]);
          Serial.print("  controlMin[");
          Serial.print(j);
          Serial.print("]: ");
          Serial.println(timerSwitch[i].controlMin[j]);
        }
        Serial.print("  timerStatus: ");
        Serial.println(timerSwitch[i].timerStatus);
      }

      lastSendDebugTime = currentSendDebugTime;
    }
  }
}
// ========================= End Debug Mode ========================= //
