#include "Setting.h"
#include "ModelaVariables.h"
#include "ModelaController.h"
#include "ModelaOTA.h"

#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// ฟังก์ชันสำหรับรับค่าจาก MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String payloadStr;
  for (unsigned int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }
  Serial.println(payloadStr);
  
  // จัดการกับ JSON
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, payloadStr);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  String controlRelayTopic = tokenKey + "/Control/Relay";
  String controlSensorTopic = tokenKey + "/Control/ValueSensor";
  String controlCalibrateSensorTopic = tokenKey + "/Control/CalibrateSensor";
  String controlLowLimitTopic = tokenKey + "/Control/SensorValueLowLimit";
  String controlHighLimitTopic = tokenKey + "/Control/SensorValueHighLimit";
  String controlAutoModeTopic = tokenKey + "/Control/RelayAutoMode";
  String controlSensorOptionTopic = tokenKey + "/Control/SensorValueOption";
  String controlSensorControlTopic = tokenKey + "/Control/SensorControl";
  String controlStatusSensorTopic = tokenKey + "/Control/StatusSensor";
  String controlStatusSensorOptionTopic = tokenKey + "/Control/StatusSensorOption";
  String controlStatusSensorControlTopic = tokenKey + "/Control/StatusSensorControl";
  String controlUpdateSensorAmountTopic = tokenKey + "/Control/UpdateValueSensorAmount";
  String controlAddTimerSwitchTopic = tokenKey + "/Control/AddTimerSwitchControl";
  String controlEditTimerSwitchTopic = tokenKey + "/Control/EditTimerSwitchControl";
  String controlTimerSwitchStatusTopic = tokenKey + "/Control/TimerSwitchStatus";
  String controlSensorNotifyMethodTopic = tokenKey + "/Control/SensorNotifyMethod";
  String controlSensorNotifyIntervalTopic = tokenKey + "/Control/SensorNotifyInterval";
  String updateOTATopic = tokenKey + "/Update/ota";

  // String updateOTATopic = tokenKey + "/UpdateOTA";
  if(String(topic) == controlRelayTopic){ // MQTT Control Relay (ควบคุมรีเลย์)
    // ตรวจสอบ JSON สำหรับ relaySwitch และ relayStatus
    if (doc.containsKey("switchRelay") && doc.containsKey("statusRelay")) {
      int switchRelay = doc["switchRelay"];
      int statusRelay = doc["statusRelay"];

      Serial.print("Relay Switch: ");
      Serial.println(switchRelay);
      Serial.print("Relay Status: ");
      Serial.println(statusRelay);

      (statusRelay) ? RelayOn(switchRelay) : RelayOff(switchRelay);
      SetAutoMode(switchRelay, 0);
    }
  }
  else if(String(topic) == controlSensorTopic){ // MQTT Select Sensor (เลือกเซ็นเซอร์)
    // ตรวจสอบ JSON สำหรับ sensorNO และ sensorSelect
    if (doc.containsKey("sensorNO") && doc.containsKey("sensorSelect")) {
      int sensorNO = doc["sensorNO"];
      int sensorSelect = doc["sensorSelect"];

      Serial.print("Sensor No: ");
      Serial.println(sensorNO);
      Serial.print("Sensor Selected: ");
      Serial.println(sensorSelect);

      SetSelectValueSensor(sensorNO, sensorSelect);
    }
  }
  else if(String(topic) == controlCalibrateSensorTopic){ // MQTT Calibrate Sensor (ปรับค่าเซ็นเซอร์)
    // ตรวจสอบ JSON สำหรับ sensorNO และ sensorCalibrateValue
    if (doc.containsKey("sensorNO") && doc.containsKey("sensorCalibrateValue")) {
      int sensorNO = doc["sensorNO"];
      float sensorCalibrateValue = doc["sensorCalibrateValue"];

      Serial.print("Sensor No: ");
      Serial.println(sensorNO);
      Serial.print("sensor CalibrateValue: ");
      Serial.println(sensorCalibrateValue);

      SetCalibrateSensor(sensorNO, sensorCalibrateValue);
    }
  }
  else if(String(topic) == controlLowLimitTopic){
    // ตรวจสอบ JSON สำหรับ sensorNO และ valueSensorLowLimit
    if (doc.containsKey("sensorNO") && doc.containsKey("valueSensorLowLimit")) {
      int sensorNO = doc["sensorNO"];
      float valueSensorLowLimit = doc["valueSensorLowLimit"];

      Serial.print("Sensor No: ");
      Serial.println(sensorNO);
      Serial.print("Sensor HighLimit: ");
      Serial.println(valueSensorLowLimit);

      SetLimitValueSensor(sensorNO, 1, valueSensorLowLimit);
    }
  }
  else if(String(topic) == controlHighLimitTopic){
    // ตรวจสอบ JSON สำหรับ sensorNO และ valueSensorHighLimit
    if (doc.containsKey("sensorNO") && doc.containsKey("valueSensorHighLimit")) {
      int sensorNO = doc["sensorNO"];
      float valueSensorHighLimit = doc["valueSensorHighLimit"];

      Serial.print("Sensor No: ");
      Serial.println(sensorNO);
      Serial.print("Sensor HighLimit: ");
      Serial.println(valueSensorHighLimit);

      SetLimitValueSensor(sensorNO, 2, valueSensorHighLimit);
    }
  }
  else if(String(topic) == controlAutoModeTopic){
    // ตรวจสอบ JSON สำหรับ relayNO และ relayAutoMode
    if (doc.containsKey("relayNO") && doc.containsKey("relayAutoMode")) {
      int relayNO = doc["relayNO"];
      int relayAutoMode = doc["relayAutoMode"];

      Serial.print("Relay No: ");
      Serial.println(relayNO);
      Serial.print("Relay AutoMode: ");
      Serial.println(relayAutoMode);

      SetAutoMode(relayNO, relayAutoMode);
    }
  }
  else if(String(topic) == controlSensorOptionTopic){
    // ตรวจสอบ JSON สำหรับ sensorNO และ sensorOption
    if (doc.containsKey("sensorNO") && doc.containsKey("sensorOption")) {
      int sensorNO = doc["sensorNO"];
      int sensorOption = doc["sensorOption"];

      Serial.print("Sensor No: ");
      Serial.println(sensorNO);
      Serial.print("Sensor Value Option: ");
      Serial.println(sensorOption);

      SetSensorValueOption(sensorNO, sensorOption);
    }
  }
  else if(String(topic) == controlSensorControlTopic){
    // ตรวจสอบ JSON สำหรับ sensorNO และ sensorControl
    if (doc.containsKey("sensorNO") && doc.containsKey("sensorControl")) {
      int sensorNO = doc["sensorNO"];
      JsonArray sensorControlArr = doc["sensorControl"];
      int sensorControlCount = sensorControlArr.size();
      int sensorControl[sensorControlCount];
      ClearSensorControl(sensorNO);
      Serial.print("Sensor Control: [");
      for (int i = 0; i < sensorControlCount; i++) {
        sensorControl[i] = sensorControlArr[i];
        Serial.print(sensorControl[i]);
        if (i < sensorControlCount - 1) {
          Serial.print(", ");
        }
        SetSensorControl(sensorNO, sensorControl[i], i);
      }
      Serial.println("]");
    }
  }
  else if(String(topic) == controlStatusSensorTopic){
    // ตรวจสอบ JSON สำหรับ statusSensorNo และ statusSensorSelect
    if (doc.containsKey("statusSensorNo") && doc.containsKey("statusSensorSelect")) {
      int statusSensorNo = doc["statusSensorNo"];
      int statusSensorSelect = doc["statusSensorSelect"];

      Serial.print("Status No: ");
      Serial.println(statusSensorNo);
      Serial.print("Status Selected: ");
      Serial.println(statusSensorSelect);

      SetSelectStatusSensorValue(statusSensorNo, statusSensorSelect);
    }
  }
  else if(String(topic) == controlStatusSensorOptionTopic){
    // ตรวจสอบ JSON สำหรับ statusSensorNo และ statusSensorOption
    if (doc.containsKey("statusSensorNo") && doc.containsKey("statusSensorOption")) {
      int statusSensorNo = doc["statusSensorNo"];
      int statusSensorOption = doc["statusSensorOption"];

      Serial.print("Status No: ");
      Serial.println(statusSensorNo);
      Serial.print("Status Sensor Option: ");
      Serial.println(statusSensorOption);

      SetStatusSensorValueOption(statusSensorNo, statusSensorOption);
    }
  }
  else if(String(topic) == controlStatusSensorControlTopic){
    // ตรวจสอบ JSON สำหรับ statusSensorNo และ statusSensorControl
    if (doc.containsKey("statusSensorNo") && doc.containsKey("statusSensorControl")) {
      int statusSensorNo = doc["statusSensorNo"];
      JsonArray statusSensorControlArr = doc["statusSensorControl"];
      int statusSensorControlCount = statusSensorControlArr.size();
      int statusSensorControl[statusSensorControlCount];
      ClearStatusSensorControl(statusSensorNo);
      Serial.print("Status Sensor Control: [");
      for (int i = 0; i < statusSensorControlCount; i++) {
        statusSensorControl[i] = statusSensorControlArr[i];
        Serial.print(statusSensorControl[i]);
        if (i < statusSensorControlCount - 1) {
          Serial.print(", ");
        }
        SetStatusSensorControl(statusSensorNo, statusSensorControl[i], i);
      }
      Serial.println("]");
    }
  }
  else if(String(topic) == controlUpdateSensorAmountTopic){
    // ตรวจสอบ JSON สำหรับ newSensorAmount
    if (doc.containsKey("newSensorAmount")) {
      int newSensorAmount = doc["newSensorAmount"];

      Serial.print("Add Sensor Value: ");
      Serial.println(newSensorAmount);

      SetSensorAmount(newSensorAmount);
    }
  }
  else if(String(topic) == controlAddTimerSwitchTopic){
    // ตรวจสอบ JSON สำหรับ timerSwitchNo และ timerSwitchStatus
    if (doc.containsKey("timerNo") && doc.containsKey("timerOption")
      && doc.containsKey("timerControl") && doc.containsKey("timerDay")
      && doc.containsKey("timerHour") && doc.containsKey("timerMin")
      && doc.containsKey("timerStatus")) {
      int timerSwitchNo = doc["timerNo"];
      int timerSwitchOption = doc["timerOption"];

      Serial.println("Timer Switch No: " + String(timerSwitchNo));
      Serial.println("Timer Switch Option: " + String(timerSwitchOption));

      JsonArray timerSwitchControlArr = doc["timerControl"];
      int timerSwitchControlCount = timerSwitchControlArr.size();
      int timerSwitchControl[timerSwitchControlCount];
      Serial.print("Timer Switch Control: [");
      for (int i = 0; i < timerSwitchControlCount; i++) {
        timerSwitchControl[i] = timerSwitchControlArr[i];
        Serial.print(timerSwitchControl[i]);
        if (i < timerSwitchControlCount - 1) {
          Serial.print(", ");
        }
      }
      Serial.println("]");

      JsonArray timerSwitchDayArr = doc["timerDay"];
      int timerSwitchDayCount = timerSwitchDayArr.size();
      int timerSwitchDay[timerSwitchDayCount];
      Serial.print("Timer Switch Day: [");
      for (int i = 0; i < timerSwitchDayCount; i++) {
        timerSwitchDay[i] = timerSwitchDayArr[i];
        Serial.print(timerSwitchDay[i]);
        if (i < timerSwitchDayCount - 1) {
          Serial.print(", ");
        }
      }
      Serial.println("]");

      JsonArray timerSwitchHourArr = doc["timerHour"];
      int timerSwitchHourCount = timerSwitchHourArr.size();
      int timerSwitchHour[timerSwitchHourCount];
      Serial.print("Timer Switch Hour: [");
      for (int i = 0; i < timerSwitchHourCount; i++) {
        timerSwitchHour[i] = timerSwitchHourArr[i];
        Serial.print(timerSwitchHour[i]);
        if (i < timerSwitchHourCount - 1) {
          Serial.print(", ");
        }
      }
      Serial.println("]");

      JsonArray timerSwitchMinuteArr = doc["timerMin"];
      int timerSwitchMinuteCount = timerSwitchMinuteArr.size();
      int timerSwitchMinute[timerSwitchMinuteCount];
      Serial.print("Timer Switch Minute: [");
      for (int i = 0; i < timerSwitchMinuteCount; i++) {
        timerSwitchMinute[i] = timerSwitchMinuteArr[i];
        Serial.print(timerSwitchMinute[i]);
        if (i < timerSwitchMinuteCount - 1) {
          Serial.print(", ");
        }
      }
      Serial.println("]");

      int timerStatus = doc["timerStatus"];
      Serial.println("Timer Status: " + String(timerStatus));
      int newTimerAmount = doc["timerAmount"];
      Serial.println("Timer Amount: " + String(newTimerAmount));

      SetTimerSwitch(1, timerSwitchNo, timerSwitchOption,
                    timerSwitchControl, timerSwitchControlCount,
                    timerSwitchDay, timerSwitchDayCount,
                    timerSwitchHour, timerSwitchHourCount,
                    timerSwitchMinute, timerSwitchMinuteCount,
                    timerStatus, newTimerAmount);
    }
  }
  else if(String(topic) == controlEditTimerSwitchTopic){
    // ตรวจสอบ JSON สำหรับ timerSwitchNo และ timerSwitchStatus
    if (doc.containsKey("timerNo") && doc.containsKey("timerOption")
      && doc.containsKey("timerControl") && doc.containsKey("timerDay")
      && doc.containsKey("timerHour") && doc.containsKey("timerMin")) {
      int timerSwitchNo = doc["timerNo"];
      int timerSwitchOption = doc["timerOption"];

      Serial.println("Timer Switch No: " + String(timerSwitchNo));
      Serial.println("Timer Switch Option: " + String(timerSwitchOption));

      JsonArray timerSwitchControlArr = doc["timerControl"];
      int timerSwitchControlCount = timerSwitchControlArr.size();
      int timerSwitchControl[timerSwitchControlCount];
      Serial.print("Timer Switch Control: [");
      for (int i = 0; i < timerSwitchControlCount; i++) {
        timerSwitchControl[i] = timerSwitchControlArr[i];
        Serial.print(timerSwitchControl[i]);
        if (i < timerSwitchControlCount - 1) {
          Serial.print(", ");
        }
      }
      Serial.println("]");

      JsonArray timerSwitchDayArr = doc["timerDay"];
      int timerSwitchDayCount = timerSwitchDayArr.size();
      int timerSwitchDay[timerSwitchDayCount];
      Serial.print("Timer Switch Day: [");
      for (int i = 0; i < timerSwitchDayCount; i++) {
        timerSwitchDay[i] = timerSwitchDayArr[i];
        Serial.print(timerSwitchDay[i]);
        if (i < timerSwitchDayCount - 1) {
          Serial.print(", ");
        }
      }
      Serial.println("]");

      JsonArray timerSwitchHourArr = doc["timerHour"];
      int timerSwitchHourCount = timerSwitchHourArr.size();
      int timerSwitchHour[timerSwitchHourCount];
      Serial.print("Timer Switch Hour: [");
      for (int i = 0; i < timerSwitchHourCount; i++) {
        timerSwitchHour[i] = timerSwitchHourArr[i];
        Serial.print(timerSwitchHour[i]);
        if (i < timerSwitchHourCount - 1) {
          Serial.print(", ");
        }
      }
      Serial.println("]");

      JsonArray timerSwitchMinuteArr = doc["timerMin"];
      int timerSwitchMinuteCount = timerSwitchMinuteArr.size();
      int timerSwitchMinute[timerSwitchMinuteCount];
      Serial.print("Timer Switch Minute: [");
      for (int i = 0; i < timerSwitchMinuteCount; i++) {
        timerSwitchMinute[i] = timerSwitchMinuteArr[i];
        Serial.print(timerSwitchMinute[i]);
        if (i < timerSwitchMinuteCount - 1) {
          Serial.print(", ");
        }
      }
      Serial.println("]");

      int timerStatus = doc["timerStatus"];
      Serial.println("Timer Status: " + String(timerStatus));
      int newTimerAmount = doc["timerAmount"];
      Serial.println("Timer Amount: " + String(newTimerAmount));

      ClearTimerControl(timerSwitchNo);
      SetTimerSwitch(2, timerSwitchNo, timerSwitchOption,
                    timerSwitchControl, timerSwitchControlCount,
                    timerSwitchDay, timerSwitchDayCount,
                    timerSwitchHour, timerSwitchHourCount,
                    timerSwitchMinute, timerSwitchMinuteCount);
    }
  }
  else if(String(topic) == controlTimerSwitchStatusTopic){
    // ตรวจสอบ JSON สำหรับ timerSwitchStatus
    if (doc.containsKey("timerNo") && doc.containsKey("timerStatus")) {
      int timerSwitchNo = doc["timerNo"];
      int timerStatus = doc["timerStatus"];

      Serial.println("Timer Switch No: " + String(timerSwitchNo));
      Serial.println("Timer Switch Status: " + String(timerStatus));

      SetTimerSwitchStatus(timerSwitchNo, timerStatus);
    }
  }
  else if(String(topic) == controlSensorNotifyMethodTopic){
    // ตรวจสอบ JSON สำหรับ sensorNo และ notifyMethod
    if (doc.containsKey("sensorNo") && doc.containsKey("notifyMethod")) {
      int sensorNo = doc["sensorNo"];
      JsonArray notifyMethodArr = doc["notifyMethod"];
      int notifyMethodCount = notifyMethodArr.size();
      int notifyMethod[notifyMethodCount];
      ClearNotifyMethodSensor(sensorNo);
      Serial.print("Sensor Notify Method: [");
      for (int i = 0; i < notifyMethodCount; i++) {
        notifyMethod[i] = notifyMethodArr[i];
        Serial.print(notifyMethod[i]);
        if (i < notifyMethodCount - 1) {
          Serial.print(", ");
        }
        SetNotifyMethodSensor(sensorNo, notifyMethod[i], i);
      }
      Serial.println("]");
    }
  }
  else if(String(topic) == controlSensorNotifyIntervalTopic){
    // ตรวจสอบ JSON สำหรับ timerSwitchStatus
    if (doc.containsKey("sensorNo") && doc.containsKey("notifyInterval")) {
      int sensorNo = doc["sensorNo"];
      int notifyInterval = doc["notifyInterval"];

      Serial.println("Sensor No: " + String(sensorNo));
      Serial.println("Notify Interval: " + String(notifyInterval));

      SetNotifyIntervalSensor(sensorNo, notifyInterval);
    }
  }
  else if(String(topic) == updateOTATopic){
    if (doc.containsKey("process")) {
      String process = doc["process"];
      if (process == "start") {
        updateOTA();
      }
    }
  }
}

// ฟังก์ชันสำหรับเชื่อมต่อ MQTT
void setupMQTT() {
  // ตั้งค่า MQTT
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(callback);
}

// ฟังก์ชันสำหรับส่งสถานะ Device ไปยัง MQTT
void publishStatus(const char* status) {
  String topic = tokenKey + "/Status";
  
  StaticJsonDocument<256> doc;
  doc["key"] = tokenKey.c_str();
  doc["Status"] = status;

  String payload;
  serializeJson(doc, payload);
  if (mqttClient.publish(topic.c_str(), payload.c_str(), true)) {
    Serial.println("MQTT message sent successfully for Status");
  } else {
    Serial.println("Failed to send MQTT message for Status");
  }
}

// ฟังก์ชันสำหรับส่งข้อมูล Device ไปยัง MQTT (เชื่อมต่อ Device)
void matchingDevice() {
  String topic = tokenKey + "/Matching";
  
  StaticJsonDocument<256> doc;
  doc["key"] = tokenKey.c_str();
  doc["deviceTypeID"] = DEVICE_TYPE_ID;
  doc["deviceVersion"] = DEVICE_VERSION;

  String payload;
  serializeJson(doc, payload);
  if (mqttClient.publish(topic.c_str(), payload.c_str(), true)) {
    Serial.println("MQTT message sent successfully for Matching");
  } else {
    Serial.println("Failed to send MQTT message for Matching");
  }

  Serial.println("Matching Device Sent");
  Serial.println(payload);
}

// Topic ที่รับข้อมูล
void subscribeTopics() {
  String topics[] = {
    "/Control/Relay",
    "/Control/ValueSensor",
    "/Control/CalibrateSensor",
    "/Control/SensorValueLowLimit",
    "/Control/SensorValueHighLimit",
    "/Control/RelayAutoMode",
    "/Control/SensorValueOption",
    "/Control/SensorControl",
    "/Control/StatusSensor",
    "/Control/StatusSensorOption",
    "/Control/StatusSensorControl",
    "/Control/UpdateValueSensorAmount",
    "/Control/AddTimerSwitchControl",
    "/Control/EditTimerSwitchControl",
    "/Control/TimerSwitchStatus",
    "/Control/SensorNotifyMethod",
    "/Control/SensorNotifyInterval",
    "/Update/ota"
  };

  for (String topic : topics) {
    String fullTopic = tokenKey + topic;
    mqttClient.subscribe(fullTopic.c_str());
  }
}

// // ฟังก์ชันสำหรับเชื่อมต่อ MQTT ใหม่
// void reconnectMQTT() {
//   while (!mqttClient.connected()) {
//     Serial.println("Connecting to MQTT...");

//     // LWT สำหรับสถานะ offline
//     String topicLWT = tokenKey + "/Status";
//     String payloadLWT = "{\"key\":\"" + tokenKey + "\", \"Status\":\"offline\"}";

//     if (mqttClient.connect(tokenKey.c_str(), MQTT_USER, MQTT_PASSWORD, topicLWT.c_str(), 0, true, payloadLWT.c_str())) {
//       Serial.println("Connected to MQTT Broker");

//       // ประกาศสถานะ online เมื่อเชื่อมต่อสำเร็จ
//       publishStatus("online");
//       matchingDevice();
//       subscribeTopics(); // สมัครรับข้อมูลจาก topic ที่ต้องการ
//     } else {
//       Serial.print("Failed with state ");
//       Serial.print(mqttClient.state());
//       delay(2000);
//     }
//   }

//   while (!mqttClientOTA.connected()) {
//     Serial.println("Connecting to MQTT For OTA...");
//     String clientOTA = tokenKey + "OTA";

//     if (mqttClientOTA.connect(clientOTA.c_str(), MQTT_USER, MQTT_PASSWORD)) {
//       Serial.println("Connected to MQTT Broker OTA");

//       subscribeOTATopics(); // สมัครรับข้อมูลจาก topic ที่ต้องการ
//     } else {
//       Serial.print("Failed with state ");
//       Serial.print(mqttClientOTA.state());
//       delay(2000);
//     }
//   }
// }

void reconnectMQTT() {
  static int attempt = 0;
  const int maxAttempts = 10;
  const unsigned long retryInterval = 2000; // 2 วินาที
  const unsigned long reconnectInterval = 10 * 60 * 1000; // 10 นาที

  if (!mqttClient.connected()) {
    unsigned long now = millis();
    
    if (attempt >= maxAttempts && now - lastReconnectMQTT < reconnectInterval) {
      // ถ้าเกินจำนวนครั้งที่กำหนดแล้วให้รอเวลา
      Serial.println("Max attempts reached. Waiting before retrying...");
      return; // ออกจากฟังก์ชันเพื่อทำงานส่วนอื่น
    }

    if (now - lastReconnectMQTT > retryInterval || attempt == 0) {
      // พยายามเชื่อมต่อใหม่หลังจากผ่านไป 10 นาทีหรือเป็นการพยายามครั้งแรก
      Serial.println("Connecting to MQTT...");

      String topicLWT = tokenKey + "/Status";
      String payloadLWT = "{\"key\":\"" + tokenKey + "\", \"Status\":\"offline\"}";

      if (mqttClient.connect(tokenKey.c_str(), MQTT_USER, MQTT_PASSWORD, topicLWT.c_str(), 0, true, payloadLWT.c_str())) {
        Serial.println("Connected to MQTT Broker");

        // ประกาศสถานะ online เมื่อเชื่อมต่อสำเร็จ
        publishStatus("online");
        matchingDevice();
        subscribeTopics(); // สมัครรับข้อมูลจาก topic ที่ต้องการ
        attempt = 0; // รีเซ็ตจำนวนการพยายามเชื่อมต่อถ้าสำเร็จ
      } else {
        Serial.print("Failed with state ");
        Serial.print(mqttClient.state());
        attempt++;
        lastReconnectMQTT = now; // บันทึกเวลาการพยายามเชื่อมต่อล่าสุด

        // if (attempt >= maxAttempts) {
        //   lastReconnectMQTT = now; // เริ่มนับเวลาหยุดพัก 10 นาทีใหม่
        // }
      }
    }
  }
}

// ============================== Start All Function MQTT ============================== //
void mqttSendValue(String topic, int mainValueNo, float mainValueData, /*int secondValueNo, float secondValueData,*/ int sensorNo, int sensorSelected) {
  String baseTopic = tokenKey + "/";
  char combineTopic[100] = ""; // ต้องเริ่มต้นด้วย string ว่างเพื่อให้ strcat ทำงานได้ถูกต้อง

  // ใช้ strcat เพื่อเพิ่ม baseTopic ไปยัง combineTopic
  strcat(combineTopic, baseTopic.c_str());
  // ต่อจากนั้นใช้ strcat อีกครั้งเพื่อเพิ่ม topic ที่ต้องการ
  strcat(combineTopic, topic.c_str());

  // สร้าง JSON object สำหรับส่งข้อมูล
  StaticJsonDocument<512> jsonDoc;
  jsonDoc["key"] = tokenKey;
  jsonDoc["sensorNo"] = sensorNo;
  jsonDoc["sensorSelected"] = sensorSelected;
  jsonDoc["mainNo"] = mainValueNo;
  String mainValueDataStr = String(mainValueData, 2);
  Serial.println("mainValueDataStr : " + String(mainValueDataStr));
  jsonDoc["mainData"] = mainValueDataStr;
  // jsonDoc["secondNo"] = secondValueNo;
  // jsonDoc["secondData"] = secondValueData;

  // แปลง JSON object เป็น String
  String jsonPayload;
  serializeJson(jsonDoc, jsonPayload);

  // Serial.println(String("JSON Payload : ") + jsonPayload);

  // ส่งข้อมูลผ่าน MQTT
  if (mqttClient.publish(combineTopic, jsonPayload.c_str())) {
    Serial.println("MQTT message sent successfully for " + String(combineTopic));
  } else {
    Serial.println("Failed to send MQTT message for " + String(combineTopic));
  }
}

void mqttSendStatusSensorValue(String topic, int statusNo, int statusData) {
  String baseTopic = tokenKey + "/";
  char combineTopic[100] = ""; // ต้องเริ่มต้นด้วย string ว่างเพื่อให้ strcat ทำงานได้ถูกต้อง

  // ใช้ strcat เพื่อเพิ่ม baseTopic ไปยัง combineTopic
  strcat(combineTopic, baseTopic.c_str());
  // ต่อจากนั้นใช้ strcat อีกครั้งเพื่อเพิ่ม topic ที่ต้องการ
  strcat(combineTopic, topic.c_str());

  // สร้าง JSON object สำหรับส่งข้อมูล
  StaticJsonDocument<512> jsonDoc;
  jsonDoc["key"] = tokenKey;
  jsonDoc["statusNo"] = statusNo;
  jsonDoc["statusData"] = statusData;

  // แปลง JSON object เป็น String
  String jsonPayload;
  serializeJson(jsonDoc, jsonPayload);

  // Serial.println(String("JSON Payload : ") + jsonPayload);

  // ส่งข้อมูลผ่าน MQTT
  if (mqttClient.publish(combineTopic, jsonPayload.c_str())) {
    Serial.println("MQTT message sent successfully for " + String(combineTopic));
  } else {
    Serial.println("Failed to send MQTT message for " + String(combineTopic));
  }
}

void mqttSendRelay(String topic, int relayNo, int relayStatus) {
  String baseTopic = tokenKey + "/";
  char combineTopic[100] = ""; // ต้องเริ่มต้นด้วย string ว่างเพื่อให้ strcat ทำงานได้ถูกต้อง

  // ใช้ strcat เพื่อเพิ่ม baseTopic ไปยัง combineTopic
  strcat(combineTopic, baseTopic.c_str());
  // ต่อจากนั้นใช้ strcat อีกครั้งเพื่อเพิ่ม topic ที่ต้องการ
  strcat(combineTopic, topic.c_str());

  // สร้าง JSON object สำหรับส่งข้อมูล
  StaticJsonDocument<512> jsonDoc;
  jsonDoc["key"] = tokenKey;
  jsonDoc["relayNo"] = relayNo;
  jsonDoc["relayStatus"] = relayStatus;

  // แปลง JSON object เป็น String
  String jsonPayload;
  serializeJson(jsonDoc, jsonPayload);

  // Serial.println(String("JSON Payload : ") + jsonPayload);

  // ส่งข้อมูลผ่าน MQTT
  if (mqttClient.publish(combineTopic, jsonPayload.c_str())) {
    Serial.println("MQTT message sent successfully for " + String(combineTopic));
  } else {
    Serial.println("Failed to send MQTT message for " + String(combineTopic));
  }
}

void mqttSendControlMode(String topic, int controlModeNo, int controlModeStatus) {
  String baseTopic = tokenKey + "/";
  char combineTopic[100] = ""; // ต้องเริ่มต้นด้วย string ว่างเพื่อให้ strcat ทำงานได้ถูกต้อง

  // ใช้ strcat เพื่อเพิ่ม baseTopic ไปยัง combineTopic
  strcat(combineTopic, baseTopic.c_str());
  // ต่อจากนั้นใช้ strcat อีกครั้งเพื่อเพิ่ม topic ที่ต้องการ
  strcat(combineTopic, topic.c_str());

  // สร้าง JSON object สำหรับส่งข้อมูล
  StaticJsonDocument<512> jsonDoc;
  jsonDoc["key"] = tokenKey;
  jsonDoc["controlModeNo"] = controlModeNo;
  jsonDoc["controlModeStatus"] = controlModeStatus;

  // แปลง JSON object เป็น String
  String jsonPayload;
  serializeJson(jsonDoc, jsonPayload);

  // Serial.println(String("JSON Payload : ") + jsonPayload);

  // ส่งข้อมูลผ่าน MQTT
  if (mqttClient.publish(combineTopic, jsonPayload.c_str())) {
    Serial.println("MQTT message sent successfully for " + String(combineTopic));
  } else {
    Serial.println("Failed to send MQTT message for " + String(combineTopic));
  }
}

// type 1 = mobile notify, 2 line notify
void mqttSendNotify(String topic, int type, int valueNo, int valueSelected, int valueData, String status) {
  String baseTopic = tokenKey + "/";
  char combineTopic[100] = ""; // ต้องเริ่มต้นด้วย string ว่างเพื่อให้ strcat ทำงานได้ถูกต้อง

  // ใช้ strcat เพื่อเพิ่ม baseTopic ไปยัง combineTopic
  strcat(combineTopic, baseTopic.c_str());
  // ต่อจากนั้นใช้ strcat อีกครั้งเพื่อเพิ่ม topic ที่ต้องการ
  strcat(combineTopic, topic.c_str());

  // สร้าง JSON object สำหรับส่งข้อมูล
  StaticJsonDocument<1024> jsonDoc;
  jsonDoc["key"] = tokenKey;
  jsonDoc["type"] = type;
  jsonDoc["valueNo"] = valueNo;
  jsonDoc["valueSelected"] = valueSelected;
  jsonDoc["valueData"] = valueData;
  jsonDoc["status"] = status;

  // แปลง JSON object เป็น String
  String jsonPayload;
  serializeJson(jsonDoc, jsonPayload);

  // Serial.println(String("JSON Payload : ") + jsonPayload);

  // ส่งข้อมูลผ่าน MQTT
  if (mqttClient.publish(combineTopic, jsonPayload.c_str())) {
    Serial.println("MQTT message sent successfully for " + String(combineTopic));
  } else {
    Serial.println("Failed to send MQTT message for " + String(combineTopic));
  }
}

void runMQTT() {
  mqttClient.loop();
  reconnectMQTT();
}

void getRelaySetting() {
  HTTPClient http;
  String API_URL = String(API_SERVER) + String(APISettingDevice) + "/" + String(tokenKey) + "/" + String(APIGetRelaySetting);
  http.begin(API_URL);

  // เพิ่ม header Authorization พร้อม API Key
  http.addHeader("Authorization", DASHBOARD_API_KEY);

  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String response = http.getString();

    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }

    Serial.println("HTTP Response code: " + String(httpResponseCode));
    Serial.println("Set Relay Start...");

    JsonObject data = doc.as<JsonObject>();

    // ดึงข้อมูล switchRelay
    if (data.containsKey("switchRelay")) {
      JsonArray switchRelay = data["switchRelay"];
      Serial.println("switchRelay : ");
      for (JsonVariant relay : switchRelay) {
        int relayNo = relay["relayNo"];
        int relayStatus = relay["relayStatus"];
        relayStatus ? RelayOn(relayNo) : RelayOff(relayNo);
        Serial.println("relayNo : " + String(relayNo) + " relayStatus : " + String(relayStatus));
      }
    } else {
      Serial.println("No switchRelay key found");
    }

    // ดึงข้อมูล relayAutoMode
    if (data.containsKey("relayAutoMode")) {
      JsonArray relayAutoMode = data["relayAutoMode"];
      Serial.println("relayAutoMode : ");
      for (JsonVariant relay : relayAutoMode) {
        int relayNo = relay["relayNo"];
        int relayAutoMode = relay["relayAutoMode"];
        SetAutoMode(relayNo, relayAutoMode);
        Serial.println("relayNo : " + String(relayNo) + " relayAutoMode : " + String(relayAutoMode));
      }
    } else {
      Serial.println("No relayAutoMode key found");
    }

    Serial.println("Set Relay Done...");
    Serial.println(response);
  } else {
    Serial.println("Error code: " + String(httpResponseCode));
  }
}

void getValueSensorSetting() {
  HTTPClient http;
  String API_URL = String(API_SERVER) + String(APISettingDevice) + "/" + String(tokenKey) + "/" + String(APIGetSensorSetting);
  http.begin(API_URL);

  // เพิ่ม header Authorization พร้อม API Key
  http.addHeader("Authorization", DASHBOARD_API_KEY);

  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String response = http.getString();

    DynamicJsonDocument doc(10240);
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }

    Serial.println("HTTP Response code: " + String(httpResponseCode));
    Serial.println("Set Value Sensor Start...");

    JsonObject data = doc.as<JsonObject>();
    
    SensorAmount = data["sensorAmount"]; // จำนวน Value Sensor
    Serial.println("Valueu Sensor Amount : " + String(SensorAmount));

    // ดึงข้อมูล sensorSelected
    if (data.containsKey("sensorSelected")) {
      JsonArray sensorSelected = data["sensorSelected"];
      Serial.println("sensorSelected : ");
      for (JsonVariant sensor : sensorSelected) {
        int sensorNo = sensor["sensorNo"];
        int sensorSelect = sensor["sensorSelect"];
        // SensorSelected[sensorNo] = sensorSelect;
        // valueSensor[sensorNo].SensorSelected;
        SetSelectValueSensor(sensorNo, sensorSelect);
        Serial.println("sensorNo : " + String(sensorNo) + " sensorSelect : " + String(sensorSelect));
      }
    } else {
      Serial.println("No sensorSelected key found");
    }

    // ดึงข้อมูล calibrateSensorValue
    if (data.containsKey("sensorCalibrate")) {
      JsonArray sensorCalibrate = data["sensorCalibrate"];
      Serial.println("sensorCalibrate : ");
      for (JsonVariant sensor : sensorCalibrate) {
        int sensorNo = sensor["sensorNo"];
        float sensorCalibrateValue = sensor["sensorCalibrateValue"];
        SetCalibrateSensor(sensorNo, sensorCalibrateValue);
        Serial.println("sensorNo : " + String(sensorNo) + " sensorCalibrateValue : " + String(sensorCalibrateValue));
      }
    } else {
      Serial.println("No sensorCalibrate key found");
    }

    // ดึงข้อมูล Limit Sensor
    if (data.containsKey("sensorValueLimit")) {
      JsonArray sensorValueLimit = data["sensorValueLimit"];
      Serial.println("sensorValueLimit : ");
      for (JsonVariant sensor : sensorValueLimit) {
        int sensorNo = sensor["sensorNo"];
        float sensorValueLowLimit = sensor["sensorValueLowLimit"];
        float sensorValueHighLimit = sensor["sensorValueHighLimit"];
        SetLimitValueSensor(sensorNo, 1, sensorValueLowLimit);
        SetLimitValueSensor(sensorNo, 2, sensorValueHighLimit);
        Serial.println("sensorNo : " + String(sensorNo) + " sensorLowLimit : " + String(sensorValueLowLimit) + " sensorHighLimit : " + String(sensorValueHighLimit));
      }
    } else {
      Serial.println("No sensorValueLimit key found");
    }

    // ดึงข้อมูล sensorValueOption
    if (data.containsKey("sensorValueOption")) {
      JsonArray sensorValueOption = data["sensorValueOption"];
      Serial.println("sensorValueOption : ");
      for (JsonVariant sensor : sensorValueOption) {
        int sensorNo = sensor["sensorNo"];
        int sensorOption = sensor["sensorOption"];
        SetSensorValueOption(sensorNo, sensorOption);
        Serial.println("Sensor No : " + String(sensorNo) + " Sensor Option : " + String(sensorOption));
        
        JsonArray sensorControlArr = sensor["sensorControl"];
        int sensorControlCount = sensorControlArr.size();
        int sensorControl[sensorControlCount];
        Serial.print("Sensor Control : [");
        for (int i = 0; i < sensorControlCount; i++) {
          sensorControl[i] = sensorControlArr[i];
          Serial.print(sensorControl[i]);
          if (i < sensorControlCount - 1) {
            Serial.print(", ");
          }
          SetSensorControl(sensorNo, sensorControl[i], i);
        }
        Serial.println("]");
      }
    } else {
      Serial.println("No sensorValueOption key found");
    }

    // ดึงข้อมูล notifySensor
    if (data.containsKey("notifySensor")) {
      JsonArray notifySensor = data["notifySensor"];
      Serial.println("notifySensor : ");
      for (JsonVariant sensor : notifySensor) {
        int sensorNo = sensor["sensorNo"];
        int notifyInterval = sensor["notifyInterval"];
        SetNotifyIntervalSensor(sensorNo, notifyInterval);
        Serial.println("Sensor No : " + String(sensorNo) + " Notify Interval : " + String(notifyInterval));
        
        JsonArray notifyMethodArr = sensor["notifyMethod"];
        int notifyMethodCount = notifyMethodArr.size();
        int notifyMethod[notifyMethodCount];
        Serial.print("Notify Method : [");
        for (int i = 0; i < notifyMethodCount; i++) {
          notifyMethod[i] = notifyMethodArr[i];
          Serial.print(notifyMethod[i]);
          if (i < notifyMethodCount - 1) {
            Serial.print(", ");
          }
          SetNotifyMethodSensor(sensorNo, notifyMethod[i], i);
        }
        Serial.println("]");
      }
    } else {
      Serial.println("No notifySensor key found");
    }

    Serial.println("Set Value Sensor Done...");
    Serial.println(response);
  } else {
    Serial.println("Error code: " + String(httpResponseCode));
  }
}

void getStatusSensorSetting() {
  HTTPClient http;
  String API_URL = String(API_SERVER) + String(APISettingDevice) + "/" + String(tokenKey) + "/" + String(APIGetStatusSensorSetting);
  http.begin(API_URL);

  // เพิ่ม header Authorization พร้อม API Key
  http.addHeader("Authorization", DASHBOARD_API_KEY);
  
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String response = http.getString();

    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }

    Serial.println("HTTP Response code: " + String(httpResponseCode));
    Serial.println("Set Status Sensor Start...");

    JsonObject data = doc.as<JsonObject>();
    
    StatusSensorAmount = data["statusSensorAmount"]; // จำนวน Status Sensor
    Serial.println("Status Sensor Amount : " + String(StatusSensorAmount));

    // ดึงข้อมูล statusSensorSelected
    if (data.containsKey("statusSensorSelected")) {
      JsonArray statusSensorSelected = data["statusSensorSelected"];
      Serial.println("statusSensorSelected : ");
      for (JsonVariant status : statusSensorSelected) {
        int statusSensorNo = status["statusSensorNo"];
        int statusSensorSelect = status["statusSensorSelect"];
        // StatusSensorSelected[statusSensorNo] = statusSensorSelect;
        SetSelectStatusSensorValue(statusSensorNo, statusSensorSelect);
        Serial.println("statusSensorNo : " + String(statusSensorNo) + " statusSensorSelect : " + String(statusSensorSelect));
      }
    } else {
      Serial.println("No statusSensorSelected key found");
    }

    // ดึงข้อมูล statusSensorOption
    if (data.containsKey("statusSensorOption")) {
      JsonArray statusSensorOption = data["statusSensorOption"];
      for (JsonVariant status : statusSensorOption) {
        int statusSensorNo = status["statusSensorNo"];
        int statusSensorOption = status["statusSensorOption"];
        SetStatusSensorValueOption(statusSensorNo, statusSensorOption);
        Serial.println("statusSensorNo : " + String(statusSensorNo) + " statusSensorOption : " + String(statusSensorOption));
        
        JsonArray statusSensorControlArr = status["statusSensorControl"];
        int statusSensorControlCount = statusSensorControlArr.size();
        int statusSensorControl[statusSensorControlCount];
        Serial.print("Sensor Control : [");
        for (int i = 0; i < statusSensorControlCount; i++) {
          statusSensorControl[i] = statusSensorControlArr[i];
          Serial.print(statusSensorControl[i]);
          if (i < statusSensorControlCount - 1) {
            Serial.print(", ");
          }
          SetStatusSensorControl(statusSensorNo, statusSensorControl[i], i);
        }
        Serial.println("]");
      }
    } else {
      Serial.println("No statusSensorOption key found");
    }

    Serial.println("Set Status Sensor Done...");
    Serial.println(response);
  } else {
    Serial.println("Error code: " + String(httpResponseCode));
  }
}

void getTimerSetting() {
  HTTPClient http;
  String API_URL = String(API_SERVER) + String(APISettingDevice) + "/" + String(tokenKey) + "/" + String(APIGetTimerSetting);
  http.begin(API_URL);

  // เพิ่ม header Authorization พร้อม API Key
  http.addHeader("Authorization", DASHBOARD_API_KEY);
  
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String response = http.getString();

    DynamicJsonDocument doc(8192);
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }

    Serial.println("HTTP Response code: " + String(httpResponseCode));
    Serial.println("Set Status Sensor Start...");

    JsonObject data = doc.as<JsonObject>();
    
    timerAmount = data["timerSwitchAmount"]; // จำนวน Status Sensor
    Serial.println("Timer Switch Amount : " + String(timerAmount));

    JsonArray timerSwitchArray = data["timerSwitch"];
    for (JsonVariant timer : timerSwitchArray) {
      int timerSwitchNo = timer["timerSwitchNo"];
      int timerSwitchOption = timer["timerSwitchOption"];
      int timerStatus = timer["timerSwitchStatus"];
      
      Serial.println("Timer Switch No: " + String(timerSwitchNo));
      Serial.println("Timer Switch Option: " + String(timerSwitchOption));
      Serial.println("Timer Status: " + String(timerStatus));

      JsonArray timerSwitchControlArr = timer["timerSwitchControl"];
      int timerSwitchControlCount = timerSwitchControlArr.size();
      int timerSwitchControl[timerSwitchControlCount];
      Serial.print("Timer Switch Control: [");
      for (int i = 0; i < timerSwitchControlCount; i++) {
        timerSwitchControl[i] = timerSwitchControlArr[i];
        Serial.print(timerSwitchControl[i]);
        if (i < timerSwitchControlCount - 1) {
          Serial.print(", ");
        }
      }
      Serial.println("]");

      JsonArray timerSwitchDayArr = timer["timerSwitchControlDay"];
      int timerSwitchDayCount = timerSwitchDayArr.size();
      int timerSwitchDay[timerSwitchDayCount];
      Serial.print("Timer Switch Day: [");
      for (int i = 0; i < timerSwitchDayCount; i++) {
        timerSwitchDay[i] = timerSwitchDayArr[i];
        Serial.print(timerSwitchDay[i]);
        if (i < timerSwitchDayCount - 1) {
          Serial.print(", ");
        }
      }
      Serial.println("]");

      JsonArray timerSwitchHourArr = timer["timerSwitchControlHour"];
      int timerSwitchHourCount = timerSwitchHourArr.size();
      int timerSwitchHour[timerSwitchHourCount];
      Serial.print("Timer Switch Hour: [");
      for (int i = 0; i < timerSwitchHourCount; i++) {
        timerSwitchHour[i] = timerSwitchHourArr[i];
        Serial.print(timerSwitchHour[i]);
        if (i < timerSwitchHourCount - 1) {
          Serial.print(", ");
        }
      }
      Serial.println("]");

      JsonArray timerSwitchMinuteArr = timer["timerSwitchControlMinute"];
      int timerSwitchMinuteCount = timerSwitchMinuteArr.size();
      int timerSwitchMinute[timerSwitchMinuteCount];
      Serial.print("Timer Switch Minute: [");
      for (int i = 0; i < timerSwitchMinuteCount; i++) {
        timerSwitchMinute[i] = timerSwitchMinuteArr[i];
        Serial.print(timerSwitchMinute[i]);
        if (i < timerSwitchMinuteCount - 1) {
          Serial.print(", ");
        }
      }
      Serial.println("]");

      SetTimerSwitch(1, timerSwitchNo, timerSwitchOption,
                    timerSwitchControl, timerSwitchControlCount,
                    timerSwitchDay, timerSwitchDayCount,
                    timerSwitchHour, timerSwitchHourCount,
                    timerSwitchMinute, timerSwitchMinuteCount,
                    timerStatus, timerAmount);
    }

    Serial.println("Set Timer Switch Done...");
    Serial.println(response);
  } else {
    Serial.println("Error code: " + String(httpResponseCode));
  }
}

// ฟังก์ชันสำหรับเชื่อมต่อ HTTP ครั้งแรก (ดึงค่าล่าสุดจาก Server)
void firstConnectHTTP() {
  // ดึงข้อมูลส่วนของ Relay & Auto Mode
  getRelaySetting();

  // ดึงข้อมูลส่วนของ Value Sensors
  getValueSensorSetting();

  // ดึงข้อมูลส่วนของ Status Sensors
  getStatusSensorSetting();

  // ดึงข้อมูลส่วนของ Timer Setting
  getTimerSetting();

  isFirstInit = false;
}
