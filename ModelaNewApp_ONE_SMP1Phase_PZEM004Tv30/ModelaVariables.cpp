#include "Setting.h"
#include "ModelaVariables.h"

// ประกาศตัวแปร Global
// mode 1 = BLE, mode 2 = WiFi
// tokenKey คือ token ที่ใช้สำหรับเชื่อมต่อกับ Server
String modeStr, failConnectedStr, ssid, password, tokenKey;
int mode, failConnected;
bool runDebugMode = false;

bool deviceConnected = false; // สภานะการเชื่อมต่อ BLE

float MainSensorValue[99], SecondSensorValue[99], CalibrateSensorValue[99], HighLimitSensorValue[99], LowLimitSensorValue[99];
uint8_t StatusSensorValue[20];
int SensorSelected[99], SensorValueOption[99], SensorValueControl[99];
int StatusSensorSelected[20], StatusSensorValueOption[20], StatusSensorValueControl[20];

uint8_t val[5], Toggle[5], button[5], AutoModeRelay[5], RelayStatus[5], RelaySwitch[5];

bool isFirstInit = true;

unsigned long lastSendDebugTime = 0;
unsigned long ResetButtonPressTime = 0;
unsigned long lastPrintLocalTime = 0;
unsigned long lastSendSensorMQTTTime = 0;
unsigned long lastSendStatusSensorMQTTTime = 0;
unsigned long lastActionRelay = 0;
unsigned long lastReconnectMQTT = 0;

struct tm timeinfo;

int SensorAmount, StatusSensorAmount;
struct ValueSensorOption valueSensor[99];
struct StatusSensorOption statusSensor[20];

int timerAmount;
struct TimerSwitch timerSwitch[20];

// ตัวแปรสำหรับรับค่าจาก Serial
float Serialval1, Serialval2, Serialval3, Serialval4, Serialval5;
float Serialval6, Serialval7, Serialval8, Serialval9, Serialval10;

struct DhtRS485Value dhtRS485Value[20];
int dhtRS485Count = 0;
bool startCalAvgDhtRS485 = false;
