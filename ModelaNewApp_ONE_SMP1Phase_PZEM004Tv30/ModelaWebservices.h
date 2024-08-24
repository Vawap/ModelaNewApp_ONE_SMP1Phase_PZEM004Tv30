#ifndef MQTT_MANAGERS_H
#define MQTT_MANAGERS_H

void setupMQTT();
void runMQTT();
void firstConnectHTTP();
void mqttSendValue(String topic, int mainValueNo, float mainValueData, /*int secondValueNo, float secondValueData,*/ int sensorNo, int sensorSelected);
void mqttSendStatusSensorValue(String topic, int statusNo, int statusData);
void mqttSendRelay(String topic, int relayNo, int relayStatus);
void mqttSendControlMode(String topic, int controlModeNo, int controlModeStatus);
void mqttSendNotify(String topic, int type, int valueNo, int valueSelected, int valueData, String status);

#endif
