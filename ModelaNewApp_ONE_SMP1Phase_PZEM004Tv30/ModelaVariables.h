#ifndef ModelaVariables_H
#define ModelaVariables_H

#include "Arduino.h"
#include "time.h"

extern String modeStr, failConnectedStr, ssid, password, tokenKey;
extern int mode, failConnected;

extern bool runDebugMode;

extern bool deviceConnected;

extern float MainSensorValue[99], SecondSensorValue[99], CalibrateSensorValue[99], HighLimitSensorValue[99], LowLimitSensorValue[99];
extern uint8_t StatusSensorValue[20];
extern int SensorSelected[99], SensorValueOption[99], SensorValueControl[99];
extern int StatusSensorSelected[20], StatusSensorValueOption[20], StatusSensorValueControl[20];
// extern int SensorAmount, StatusSensorAmount;

extern uint8_t val[5], Toggle[5], button[5], AutoModeRelay[5], RelayStatus[5], RelaySwitch[5];

extern bool isFirstInit;

extern unsigned long lastSendDebugTime, ResetButtonPressTime, lastPrintLocalTime, lastSendSensorMQTTTime;
extern unsigned long lastSendStatusSensorMQTTTime, lastActionRelay, lastReconnectMQTT;

extern struct tm timeinfo; // เวลาปัจจุบัน

extern int SensorAmount;
extern struct ValueSensorOption {
  int SensorNO;
  int SensorSelected;
  String SensorDisplay;
  String SensorUnit;
  int SensorNotification;
  int SensorValueOption;
  int SensorValueControl[5];
  float HighLimitSensorValue;
  float LowLimitSensorValue;
  float CalibrateSensorValue;
  int NotifyIntervalValue;
  int NotifyMethodValue[2];
  unsigned long lastNotifyTime;
  unsigned long interval; // ตัวแปรใหม่สำหรับเก็บ interval ของแต่ละ sensor
  bool notifySent; // ตัวแปรใหม่สำหรับเก็บสถานะการแจ้งเตือน
  int TimerStatus;
  int TimerNo[10];
  int TimerHourBegin[10];
  int TimerMinBegin[10];
  int TimerHourEnd[10];
  int TimerMinEnd[10];
} valueSensor[99];

extern int StatusSensorAmount;
extern struct StatusSensorOption {
  int StatusSensorSelected;
  int StatusSensorNotification;
  int StatusSensorValueOption;
  int StatusSensorValueControl[5];
  int TimerStatus;
  int TimerNo[10];
  int TimerHourBegin[10];
  int TimerMinBegin[10];
  int TimerHourEnd[10];
  int TimerMinEnd[10];
} statusSensor[20];

extern int timerAmount;
extern struct TimerSwitch {
  int timerNo;
  int timerOption; // ตัวเลือก Timer
  int timerControl[5]; // Switch ที่ควบคุม
  int controlDay[7]; // วันที่ต้องการควบคุม
  int controlHour[2]; // ชั่วโมงที่ต้องการควบคุม
  int controlMin[2]; // นาทีที่ต้องการควบคุม
  int timerStatus;
} timerSwitch[20];

// ตัวแปรสำหรับรับค่าจาก Serial
extern float Serialval1, Serialval2, Serialval3, Serialval4, Serialval5;
extern float Serialval6, Serialval7, Serialval8, Serialval9, Serialval10;

extern struct DhtRS485Value {
  int dhtID;
  float dhtHumi;
  float dhtTemp;
} dhtRS485Value[20];

extern int dhtRS485Count;
extern bool startCalAvgDhtRS485;

#endif
