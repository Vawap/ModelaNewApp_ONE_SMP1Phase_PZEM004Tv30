#include <Arduino.h>

// Board Version
#define BOARD_FIRMWARE_VERSION "1.0.1"
#define BOARD_HARDWARE_VERSION "1.0.0"
#define BOARD_FIRMWARE_TYPE_SMART 1
#define BOARD_FIRMWARE_TYPE_FARM 2
#define BOARD_FIRMWARE_TYPE_ONE 3

#define BOARD_FIRMWARE_TYPE BOARD_FIRMWARE_TYPE_ONE

// Device Version
#define DEVICE_TYPE_ID "107001"
#define DEVICE_VERSION "1.0.0"
#define DEVICE_BT_NAME "Modela Eco Meter ESP32"

// ตั้งค่า Device แยกตามประเภท
#if BOARD_FIRMWARE_TYPE == BOARD_FIRMWARE_TYPE_ONE
  #define RELAY_AMOUNT 1
  #define SW1 34
  #define SW2 999
  #define SW3 999
  #define SW4 999
  #define SW5 999
  #define RL1 14
  #define RL2 999
  #define RL3 999
  #define RL4 999
  #define RL5 999
#elif BOARD_FIRMWARE_TYPE == BOARD_FIRMWARE_TYPE_SMART
  #define RELAY_AMOUNT 2
  #define SW1 34
  #define SW2 5
  #define SW3 999
  #define SW4 999
  #define SW5 999
  #define RL1 14
  #define RL2 26
  #define RL3 999
  #define RL4 999
  #define RL5 999
#elif BOARD_FIRMWARE_TYPE == BOARD_FIRMWARE_TYPE_FARM
  #define RELAY_AMOUNT 5
  #define SW1 5
  #define SW2 15
  #define SW3 39
  #define SW4 35
  #define SW5 34
  #define RL1 19
  #define RL2 23
  #define RL3 32
  #define RL4 26
  #define RL5 14
#else
  #define RELAY_AMOUNT 2
  #define SW1 34
  #define SW2 5
  #define RL1 14
  #define RL2 26
#endif

// Board Pin
#define BOARD_LED_PIN 2
#define BOARD_BUTTON_PIN 0

// Action
#define BUTTON_HOLD_TIME_ACTION 15000
#define BUTTON_HOLD_TIME_DEBUG_MODE 1000

// Switch
#define EXP_ADDRESS 0x38
#define ON 0
#define OFF 1

// Device Pin
#define D5 32
#define D6 33
#define D7 25
#define D0 27
#define TXA 13
#define RXA 27
#define Dex1 4 // digital extra 1
#define A1 39  // analog place AIN2
#define A2 35  // analog place AIN3
#define A3 36  // analog place AIN3

// EEPROM Setting
#define EEPROM_SIZE 512

// Screen Setting
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1 // ที่อยู่ของจอ OLED ในโหมด I2C

// BLE Setting
#define SERVICE_UUID "6e400001-b5a3-f393-e0a9-e50e24dcca9e"        // UUID สำหรับบริการใน ESP32 // ต้องลองดูว่าใช้หลายตัว UDID เดียวกันได้ไหม
#define CHARACTERISTIC_UUID "6e400002-b5a3-f393-e0a9-e50e24dcca9e" // UUID สำหรับคุณสมบัติใน ESP32 // ต้องลองดูว่าใช้หลายตัว UDID เดียวกันได้ไหม


// MQTT Setting
// #define MQTT_SERVER "52.230.96.1681"
#define MQTT_SERVER "mqtt.modelaaiiot.com"
#define MQTT_PORT 1883
#define MQTT_USER "modelaaiiotmqtt"
#define MQTT_PASSWORD "v0Xu91Ksmodela@!ufi"

// HTTP Setting
// #define API_SERVER "http://192.168.100.244:8080/api/v2/"
#define API_SERVER "https://www.modelaaiiot.com/api/v2/"
#define APISettingDevice "deviceSetting"
#define APIGetRelaySetting "relay"
#define APIGetSensorSetting "valuesensor"
#define APIGetStatusSensorSetting "statusSensor"
#define APIGetTimerSetting "timer"

#define DASHBOARD_API_KEY "wzkJHModelaMteVgIOTHkpJc"

// Time Setting
#define TIME_ZONE 25200 // 7 * 60 * 60 (GMT+7)
#define DAYLIGHT_OFFSET 0
#define NTP_SERVER_1 "pool.ntp.org"
#define NTP_SERVER_2 "time.nist.gov"

#define OTA_UPDATE_URL "http://updatego.modela.co.th/Bk7mg10/mdli535/Modela_SMC_FarmSeries_ESP32_app.ino.bin"
