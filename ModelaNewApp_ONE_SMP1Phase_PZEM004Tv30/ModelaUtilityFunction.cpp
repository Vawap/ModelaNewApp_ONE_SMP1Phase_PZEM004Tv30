
#include "Setting.h"
#include "ModelaVariables.h"
#include "ModelaController.h"

// ตั้งค่าเวลา
void currentTimeSetup() {
  configTime(TIME_ZONE, DAYLIGHT_OFFSET, NTP_SERVER_1, NTP_SERVER_2);
}

// ตรวจสอบว่ามีค่าใน Array หรือไม่
bool isInArray(int value, int arr[], int arrSize) {
  for (int i = 0; i < arrSize; i++) {
    if (arr[i] == value) {
      // Serial.println("Found in Array");
      return true;
    }
  }
  // Serial.println("Not Found in Array");
  return false;
}

// คำสั่งทำงานตามเวลาที่ตั้งไว้
void ActionTimer() {
  for (int i = 1; i <= timerAmount; i++) {
    if (timerSwitch[i].timerStatus == 1) {
      int Option = timerSwitch[i].timerOption; // ตัวเลือกการควบคุม Timer 1 = On, 2 = Off, 3 = On & Off
      int timerDaySize = sizeof(timerSwitch[i].controlDay) / sizeof(timerSwitch[i].controlDay[0]);
      
      if (isInArray(timeinfo.tm_wday, timerSwitch[i].controlDay, timerDaySize)) {
        for (int j = 0; j < 2; j++) {
          if ((Option == 1 && j == 0) || (Option == 2 && j == 0) || Option == 3) {
            if (timerSwitch[i].controlHour[j] == timeinfo.tm_hour && timerSwitch[i].controlMin[j] == timeinfo.tm_min) {
              for (int k = 0; k < 5; k++) {
                if (timerSwitch[i].timerControl[k] != 0) {
                  if (Option == 1 || (Option == 3 && j == 0)) {
                    RelayOn(timerSwitch[i].timerControl[k]);
                  } else if (Option == 2 || (Option == 3 && j == 1)) {
                    RelayOff(timerSwitch[i].timerControl[k]);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// แสดงเวลาปัจจุบัน
void printLocalTime() {
  // struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  unsigned long currentPrintLocalTime = millis();
  unsigned long printLocalTimeInterval = 1000; // 1 วินาที

  if (currentPrintLocalTime - lastPrintLocalTime >= printLocalTimeInterval) {
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    ActionTimer();

    lastPrintLocalTime = currentPrintLocalTime;
  }
}

void blinkLED(int delayTime, String txt = "") {
  digitalWrite(BOARD_LED_PIN, HIGH);
  (txt) ? Serial.print(txt) : NULL;
  delay(delayTime);
  digitalWrite(BOARD_LED_PIN, LOW);
  (txt) ? Serial.print(txt) : NULL;
  delay(delayTime);
}

double mapf(double PHval, double in_min, double in_max, double out_min, double out_max) {
  return (PHval - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  delay(100);
}

// map pressure transducer float
double Levelmapf(double Levelval, double in_min, double in_max, double out_min, double out_max) {
  return (Levelval - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double LevelmapfJ2(double Levelval, double in_min, double in_max, double out_min, double out_max) {
  return (Levelval - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
