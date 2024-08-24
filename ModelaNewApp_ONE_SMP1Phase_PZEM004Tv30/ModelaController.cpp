#include "Setting.h"

#include "ModelaController.h"
#include "ModelaVariables.h"
#include "EEPROM_Managers.h"
#include "ModelaWebservices.h"
#include "ModelaSensors.h"
#include "ModelaUtilityFunction.h"

// ============================== Start Function สำหรับ Set ค่าตัวแปร ============================== //

// ตัวเลือกการเลือกดูค่า Sensor แต่ละ Value
void SetSelectValueSensor(int SelectValue, int SelectSensor)
{
  int SensorSelectedIndex = SelectValue;
  if (SelectValue) {
    valueSensor[SensorSelectedIndex].SensorSelected = SelectSensor;
  }
}

// Calibrate ค่าเซนเซอร์
void SetCalibrateSensor(int SelectValue, float CalibrateSensor)
{
  int CalibrateSensorIndex = SelectValue;
  if (CalibrateSensorIndex) {
    valueSensor[CalibrateSensorIndex].CalibrateSensorValue = CalibrateSensor;
  }
}

// Set Limit Sensor ค่าควคุมด้านสูงและต่ำ
void SetLimitValueSensor(int SelectValue, int typeValue, float LimitValue)
{
  int LimitValueIndex = SelectValue;
  if (SelectValue) {
    if (typeValue == 1) { // LowLimit
      valueSensor[LimitValueIndex].LowLimitSensorValue = LimitValue;
    }
    else if (typeValue == 2) { // HighLimit
      valueSensor[LimitValueIndex].HighLimitSensorValue = LimitValue;
    }
  }
}

// Set ค่า AutoMode ของ Relay ที่ต้องการ 0 = Manual, 1 = Auto
void SetAutoMode(int SelectRelay, int AutoMode)
{
  int AutoModeIndex = SelectRelay;
  if (AutoModeIndex)
  {
    AutoModeRelay[AutoModeIndex] = AutoMode;
    mqttSendControlMode("ControlRelayMode", AutoModeIndex, AutoMode);
  }
}

// Set ค่า Option ของ Sensor ที่ต้องการ
void SetSensorValueOption(int SelectValue, int SensorOption)
{
  int SensorValueOptionIndex = SelectValue;
  if (SensorValueOptionIndex) {
    valueSensor[SensorValueOptionIndex].SensorValueOption = SensorOption;
  }
}

// ฟังก์ชันในการล้างค่า SensorValueControl
void ClearSensorControl(int sensorNO) {
  for (int i = 0; i < 5; i++) {
    valueSensor[sensorNO].SensorValueControl[i] = 0; // หรือค่าเริ่มต้นที่คุณต้องการ
  }
}

// Set Relay ที่ต้องการควบคุม
void SetSensorControl(int SelectValue, int SensorControl, int SensorControlIndex)
{
  int SensorValueControlIndex = SelectValue;
  if (SensorValueControlIndex) {
    valueSensor[SensorValueControlIndex].SensorValueControl[SensorControlIndex] = SensorControl;
  }
}

// Set ค่า Notify Interval ของ Sensor ที่ต้องการ
void SetNotifyIntervalSensor(int SelectValue, int NotifyInterval)
{
  int SensorNotifyIntervalIndex = SelectValue;
  if (SensorNotifyIntervalIndex) {
    valueSensor[SensorNotifyIntervalIndex].NotifyIntervalValue = NotifyInterval;
  }
}

// ฟังก์ชันในการล้างค่า NotifyMethod
void ClearNotifyMethodSensor(int sensorNO) {
  for (int i = 0; i < 2; i++) {
    valueSensor[sensorNO].NotifyMethodValue[i] = 0; // หรือค่าเริ่มต้นที่คุณต้องการ
  }
}

// Set NotifyMethod ที่ต้องการ
void SetNotifyMethodSensor(int SelectValue, int NotifyMethod, int SensorNotifyIndex)
{
  int SensorNotifyMethodIndex = SelectValue;
  if (SensorNotifyMethodIndex) {
    valueSensor[SensorNotifyMethodIndex].NotifyMethodValue[SensorNotifyIndex] = NotifyMethod;
  }
}

void SetSelectStatusSensorValue(int SelectValue, int StatusSensor)
{
  int StatusSensorValueIndex = SelectValue;
  if (StatusSensorValueIndex) {
    statusSensor[StatusSensorValueIndex].StatusSensorSelected = StatusSensor;
  }
}

// Set ค่า Option ของ Status Sensor ที่ต้องการ
void SetStatusSensorValueOption(int SelectValue, int StatusSensorOption)
{
  int StatusSensorValueOptionIndex = SelectValue;
  if (StatusSensorValueOptionIndex) {
    statusSensor[StatusSensorValueOptionIndex].StatusSensorValueOption = StatusSensorOption;
  }
}

// ฟังก์ชันในการล้างค่า StatusSensorValueControl
void ClearStatusSensorControl(int sensorNO) {
  for (int i = 0; i < 5; i++) {
    statusSensor[sensorNO].StatusSensorValueControl[i] = 0;
  }
}

// Set Relay ที่ต้องการควบคุม
void SetStatusSensorControl(int SelectValue, int StatusSensorControl, int StatusSensorControlIndex)
{
  int StatusSensorValueControlIndex = SelectValue;
  if (StatusSensorValueControlIndex) {
    statusSensor[StatusSensorValueControlIndex].StatusSensorValueControl[StatusSensorControlIndex] = StatusSensorControl;
  }
}

void SetSensorAmount(int newSensorAmount) {
  SensorAmount = newSensorAmount;
}


// Relay
// ==================== Start Relay ==================== //
void RelayOn(int switchRelay)
{
  if (switchRelay == 1) {
    (BOARD_FIRMWARE_TYPE == 3) ? digitalWrite(RL1, OFF) : digitalWrite(RL1, ON);
  } else if (switchRelay == 2) {
    (BOARD_FIRMWARE_TYPE == 3) ? digitalWrite(RL2, OFF) : digitalWrite(RL2, ON);
  } else if (switchRelay == 3) {
    (BOARD_FIRMWARE_TYPE == 3) ? digitalWrite(RL3, OFF) : digitalWrite(RL3, ON);
  } else if (switchRelay == 4) {
    (BOARD_FIRMWARE_TYPE == 3) ? digitalWrite(RL4, OFF) : digitalWrite(RL4, ON);
  } else if (switchRelay == 5) {
    (BOARD_FIRMWARE_TYPE == 3) ? digitalWrite(RL5, OFF) : digitalWrite(RL5, ON);
  }

  mqttSendRelay("ControlRelay", switchRelay, 1);

  RelayStatus[switchRelay] = 1;
  RelaySwitch[switchRelay] = 1;
}

void RelayOff(int switchRelay)
{
  if (switchRelay == 1) {
    (BOARD_FIRMWARE_TYPE == 3) ? digitalWrite(RL1, ON) : digitalWrite(RL1, OFF);
  } else if (switchRelay == 2) {
    (BOARD_FIRMWARE_TYPE == 3) ? digitalWrite(RL2, ON) : digitalWrite(RL2, OFF);
  } else if (switchRelay == 3) {
    (BOARD_FIRMWARE_TYPE == 3) ? digitalWrite(RL3, ON) : digitalWrite(RL3, OFF);
  } else if (switchRelay == 4) {
    (BOARD_FIRMWARE_TYPE == 3) ? digitalWrite(RL4, ON) : digitalWrite(RL4, OFF);
  } else if (switchRelay == 5) {
    (BOARD_FIRMWARE_TYPE == 3) ? digitalWrite(RL5, ON) : digitalWrite(RL5, OFF);
  }

  mqttSendRelay("ControlRelay", switchRelay, 0);

  RelayStatus[switchRelay] = 0;
  RelaySwitch[switchRelay] = 0;
}

// ฟังก์ชันสำหรับปรับปรุงสถานะของรีเลย์ตามปุ่มกด
void updateRelay(int switchRelay) {
  if (val[switchRelay] == 1 && RelayStatus[switchRelay] == 0 && Toggle[switchRelay] == 0) {
    // Serial.println("Val [" + String(switchRelay) + "] : " + String(val[switchRelay]));
    // Serial.println("RelayStatus [" + String(switchRelay) + "] : " + String(RelayStatus[switchRelay]));
    RelayOn(switchRelay);
    Toggle[switchRelay] = 1;
    SetAutoMode(switchRelay, 0); // ตั้งค่าโหมดอัตโนมัติเป็น 0 ทุกครั้งหลังจากดำเนินการ
  } else if (val[switchRelay] == 0 && RelayStatus[switchRelay] == 1 && Toggle[switchRelay] == 1) {
    // Serial.println("Val [" + String(switchRelay) + "] : " + String(val[switchRelay]));
    // Serial.println("RelayStatus [" + String(switchRelay) + "] : " + String(RelayStatus[switchRelay]));
    RelayOn(switchRelay);
    Toggle[switchRelay] = 0;
    SetAutoMode(switchRelay, 0); // ตั้งค่าโหมดอัตโนมัติเป็น 0 ทุกครั้งหลังจากดำเนินการ
  } else if (val[switchRelay] == 1 && RelayStatus[switchRelay] == 1 && Toggle[switchRelay] == 0) {
    // Serial.println("Val [" + String(switchRelay) + "] : " + String(val[switchRelay]));
    // Serial.println("RelayStatus [" + String(switchRelay) + "] : " + String(RelayStatus[switchRelay]));
    RelayOff(switchRelay);
    Toggle[switchRelay] = 1;
    SetAutoMode(switchRelay, 0); // ตั้งค่าโหมดอัตโนมัติเป็น 0 ทุกครั้งหลังจากดำเนินการ
  } else if (val[switchRelay] == 0 && RelayStatus[switchRelay] == 0 && Toggle[switchRelay] == 1) {
    // Serial.println("Val [" + String(switchRelay) + "] : " + String(val[switchRelay]));
    // Serial.println("RelayStatus [" + String(switchRelay) + "] : " + String(RelayStatus[switchRelay]));
    RelayOff(switchRelay);
    Toggle[switchRelay] = 0;
    SetAutoMode(switchRelay, 0); // ตั้งค่าโหมดอัตโนมัติเป็น 0 ทุกครั้งหลังจากดำเนินการ
  }
}
// ==================== End Relay ==================== //


// ==================== Start Physical Button ==================== //
// กดปุ่มเปิด-ปิดรีเลย์
void PhysicalButton() {
  // อ่านสถานะของปุ่มกดและอัปเดตตัวแปร val1 และ val2
  val[1] = digitalRead(SW1) == ON ? 1 : 0;
  val[2] = digitalRead(SW2) == ON ? 1 : 0;

  // ประมวลผลการตอบสนองของรีเลย์ตามสถานะของปุ่มกดและสถานะปัจจุบันของรีเลย์
  updateRelay(1);
  updateRelay(2);
}

// กดปุ่ม Reset
void ResetButton() {
  int buttonState = digitalRead(BOARD_BUTTON_PIN);

  if (buttonState == LOW) {
    // หากปุ่มถูกกดลง
    ResetButtonPressTime = millis();
    while (digitalRead(BOARD_BUTTON_PIN) == LOW) {
      // รอจนกว่าปุ่มจะถูกปล่อยปล่อย
      if (millis() - ResetButtonPressTime >= BUTTON_HOLD_TIME_ACTION) {
        // หากปุ่มถูกกดค้างไว้เกินเวลาที่กำหนด
        // ทำการกระพริบ LED
        while (digitalRead(BOARD_BUTTON_PIN) == LOW) {
          blinkLED(100);
        }

        resetModelaConfig();
        // รีสตาร์ท ESP32
        delay(1000);
        ESP.restart();
      }

      // // เข้า Mode Debug
      // if(millis() - ResetButtonPressTime >= 1000){
      //   // หากปุ่มถูกกดค้างไว้เกินเวลาที่กำหนด
      //   // ทำการปิด LED
      //   while (digitalRead(BOARD_BUTTON_PIN) == LOW) {
      //     digitalWrite(BOARD_LED_PIN, HIGH);
      //   }

      //   digitalWrite(BOARD_LED_PIN, LOW);
      //   runDebugMode = !runDebugMode;
      // }
    }
  }
}
// ==================== End Physical Button ==================== //

// ฟังก์ชันในการล้างค่า SensorValueControl
void ClearTimerControl(int TimerNo) {
  for (int i = 0; i < 5; i++) {
    timerSwitch[TimerNo].timerControl[i] = 0; // หรือค่าเริ่มต้นที่คุณต้องการ
  }
}

// SetType 1 = Add Timer, 2 = Edit Timer, 3 = Delete Timer
void SetTimerSwitch(int SetType, int TimerNo, int TimerOption,
                    int *TimerControl, int TimerControlCount,
                    int *TimerDay, int TimerDayCount,
                    int *TimerHour, int TimerHourCount,
                    int *TimerMin, int TimerMinCount,
                    int TimerStatus, int newTimerAmount) {
  int TimerSwitchIndex = TimerNo;
  if (SetType == 1) {
    timerAmount = newTimerAmount;
    timerSwitch[TimerSwitchIndex].timerNo = TimerNo;
    timerSwitch[TimerSwitchIndex].timerOption = TimerOption;
    for (int i = 0; i < TimerControlCount; i++) {
      timerSwitch[TimerSwitchIndex].timerControl[i] = TimerControl[i];
    }
    for (int i = 0; i < TimerDayCount; i++) {
      timerSwitch[TimerSwitchIndex].controlDay[i] = TimerDay[i];
    }
    for (int i = 0; i < TimerHourCount; i++) {
      timerSwitch[TimerSwitchIndex].controlHour[i] = TimerHour[i];
    }
    for (int i = 0; i < TimerMinCount; i++) {
      timerSwitch[TimerSwitchIndex].controlMin[i] = TimerMin[i];
    }
    timerSwitch[TimerSwitchIndex].timerStatus = TimerStatus;
  }
  else if (SetType == 2) {
    timerSwitch[TimerSwitchIndex].timerOption = TimerOption;
    for (int i = 0; i < TimerControlCount; i++) {
      timerSwitch[TimerSwitchIndex].timerControl[i] = TimerControl[i];
    }
    for (int i = 0; i < TimerDayCount; i++) {
      timerSwitch[TimerSwitchIndex].controlDay[i] = TimerDay[i];
    }
    for (int i = 0; i < TimerHourCount; i++) {
      timerSwitch[TimerSwitchIndex].controlHour[i] = TimerHour[i];
    }
    for (int i = 0; i < TimerMinCount; i++) {
      timerSwitch[TimerSwitchIndex].controlMin[i] = TimerMin[i];
    }
  }
  // else if (SetType == 3) {
  //   timerSwitch[TimerSwitchIndex].timerNo = 0;
  //   timerSwitch[TimerSwitchIndex].timerOption = 0;
  //   for (int i = 0; i < TimerControlCount; i++) {
  //     timerSwitch[TimerSwitchIndex].timerControl[i] = 0;
  //   }
  //   for (int i = 0; i < TimerDayCount; i++) {
  //     timerSwitch[TimerSwitchIndex].controlDay[i] = 0;
  //   }
  //   for (int i = 0; i < TimerHourCount; i++) {
  //     timerSwitch[TimerSwitchIndex].controlHour[i] = 0;
  //   }
  // }
}

void SetTimerSwitchStatus(int TimerNo, int TimerStatus) {
  int TimerSwitchIndex = TimerNo;
  timerSwitch[TimerSwitchIndex].timerStatus = TimerStatus;
}

void ValueSensorControl(int numSensor) {
  // Serial.println("Value Sensor Action : " + String(numSensor) + " Sensor");
  // numSensor = numSensor - 1;
  for (int i = 1; i <= numSensor; i++) {
    switch (valueSensor[i].SensorSelected) {
      case 0:
        MainSensorValue[i] = 0;
        SecondSensorValue[i] = 0;
        break;

      case 1:
        PZEM004Tv30_6(i, 1);
        break;
      case 2:
        PZEM004Tv30_6(i, 2);
        break;
      case 3:
        PZEM004Tv30_6(i, 3);
        break;
      case 4:
        PZEM004Tv30_6(i, 4);
        break;
      case 5:
        PZEM004Tv30_6(i, 5);
        break;
      case 6:
        PZEM004Tv30_6(i, 6);
        break;
      default:
        MainSensorValue[i] = 0;
        SecondSensorValue[i] = 0;
        break;
    }
    delay(200);
  }
}

void ValueStatusSensorControl(int numStatusSensor) {
  // Serial.println("Value Sensor Action : " + String(numStatusSensor) + " Sensor");
  // numStatusSensor = numStatusSensor - 1;
  for (int i = 1; i <= numStatusSensor; i++) {
    switch (statusSensor[i].StatusSensorSelected) {
      case 0:
        StatusSensorValue[i] = 0;
        break;
      case 1:
        FloatingValueFunction(i, 1);
        break;
      case 2:
        FloatingValueFunction(i, 2);
        break;
      case 3:
        FloatingValueFunction(i, 3);
        break;
      case 4:
        ElectricFenceFunction(i, 1);
        break;
      case 5:
        ElectricFenceFunction(i, 2);
        break;
      case 6:
        AlarmFenceFunction(i, 1);
        break;
      case 7:
        AlarmFenceFunction(i, 2);
        break;
      case 8:
        PIRstatusFunction(i, 1);
        break;
      case 9:
        PIRstatusFunction(i, 2);
        break;
      case 10:
        PIRstatusFunction(i, 3);
        break;
      case 11:
        GeneralHighLevelFunction(i, 1);
        break;
      case 12:
        GeneralHighLevelFunction(i, 2);
        break;
      case 13:
        GeneralHighLevelFunction(i, 3);
        break;
      case 14:
        GeneralLowLevelFunction(i, 1);
        break;
      case 15:
        GeneralLowLevelFunction(i, 2);
        break;
      case 16:
        GeneralLowLevelFunction(i, 3);
        break;
      case 17:
        PIRPowerstatusFunction(i, 1, 1);
        break;
      case 18:
        PIRPowerstatusFunction(i, 2, 2);
        break;
      case 19:
        PIRPowerstatusFunction(i, 3, 3);
        break;
      case 20:
        PIRPowerstatusFunction(i, 1, 4);
        break;
      case 21:
        PIRPowerstatusFunction(i, 2, 5);
        break;
      case 22:
        PIRPowerstatusFunction(i, 3, 6);
        break;
      default:
        StatusSensorValue[i] = 0;
        break;
    }
    // Serial.println("Sensor Selected : " + String(i) + " : " + String(SensorSelected[i]));
  }
}

void ControlRelay(int relayStatus, int relayControl) {
  // relayStatus : 1 = ON, 0 = OFF
  // Serial.println("AutoModeRelay : " + String(AutoModeRelay[relayControl]) + " Index : " + String(relayControl));
  if (AutoModeRelay[relayControl]) {
    // Serial.println("relayStatus : " + String(relayStatus) + ", relayControl : " + String(relayControl));
    (relayStatus) ? RelayOn(relayControl) : RelayOff(relayControl);
  }
}

void valueSensorDataSendMQTT(int numSensor) {
  for (int i = 1; i <= numSensor; i++) {
    if (valueSensor[i].CalibrateSensorValue) {
      MainSensorValue[i] = MainSensorValue[i] + valueSensor[i].CalibrateSensorValue;
      // Serial.println("Main Value Sensor " + String(i) + " : " + String(MainSensorValue[i]));
      // Serial.println("Calibrate Sensor " + String(i) + " : " + String(valueSensor[i].CalibrateSensorValue));
    }
    mqttSendValue("ValueSensor", i, MainSensorValue[i], i, valueSensor[i].SensorSelected);
  }
}

void valueStatusSensorDataSendMQTT(int numSensor) {
  for (int i = 1; i <= numSensor; i++) {
    mqttSendStatusSensorValue("ValueStatusSensor", i, StatusSensorValue[i]);
  }
}

unsigned long getNotifyInterval(int interval) {
  switch (interval) {
    case 1: return 0;            // เตือนครั้งเดียว
    case 2: return 2 * 60 * 1000; // เตือนทุก 2 นาที
    case 3: return 10 * 60 * 1000; // เตือนทุก 10 นาที
    case 4: return 15 * 60 * 1000; // เตือนทุก 15 นาที
    case 5: return 30 * 60 * 1000; // เตือนทุก 30 นาที
    case 6: return 60 * 60 * 1000; // เตือนทุก 1 ชม
    default: return 0;
  }
}

void ActionRelaySensor(int numSensor) {
  unsigned long currentTime = millis();

  for (int i = 1; i <= numSensor; i++) {
    int Option = valueSensor[i].SensorValueOption;
    float HighLimit = valueSensor[i].HighLimitSensorValue;
    float LowLimit = valueSensor[i].LowLimitSensorValue;
    int valueSensorControlSize = sizeof(valueSensor[i].SensorValueControl) / sizeof(valueSensor[i].SensorValueControl[0]);
    int valueNotifyMethodSize = sizeof(valueSensor[i].NotifyMethodValue) / sizeof(valueSensor[i].NotifyMethodValue[0]);
    valueSensor[i].interval = getNotifyInterval(valueSensor[i].NotifyIntervalValue);

    for (int j = 1; j <= RELAY_AMOUNT && Option != 0; j++) {
      if (isInArray(j, valueSensor[i].SensorValueControl, valueSensorControlSize)) {
        bool conditionMet = false;

        if ((Option == 1 || Option == 3) && MainSensorValue[i] > HighLimit) {
          ControlRelay(1, j);
          conditionMet = true;
        } else if ((Option == 1 || Option == 4) && MainSensorValue[i] < LowLimit) {
          ControlRelay(0, j);
          conditionMet = true;
        } else if ((Option == 2 || Option == 5) && MainSensorValue[i] > HighLimit) {
          ControlRelay(0, j);
          conditionMet = true;
        } else if ((Option == 2 || Option == 6) && MainSensorValue[i] < LowLimit) {
          ControlRelay(1, j);
          conditionMet = true;
        } else {
          // รีเซ็ตค่า lastNotifyTime และสถานะการแจ้งเตือนเมื่อหลุดจากเงื่อนไข
          valueSensor[i].lastNotifyTime = currentTime;
          valueSensor[i].notifySent = false;
        }

        if (conditionMet) {
          if (!valueSensor[i].notifySent || ((currentTime - valueSensor[i].lastNotifyTime >= valueSensor[i].interval) && valueSensor[i].interval > 0)) {
            const char* status = (MainSensorValue[i] > HighLimit) ? "high" : "low";
            if (isInArray(1, valueSensor[i].NotifyMethodValue, valueNotifyMethodSize)) {
              mqttSendNotify("ValueSensorNotify", 1, i, valueSensor[i].SensorSelected, MainSensorValue[i], status);
              delay(250);
            }
            if (isInArray(2, valueSensor[i].NotifyMethodValue, valueNotifyMethodSize)) {
              mqttSendNotify("ValueSensorNotify", 2, i, valueSensor[i].SensorSelected, MainSensorValue[i], status);
              delay(250);
            }
            valueSensor[i].lastNotifyTime = currentTime;
            valueSensor[i].notifySent = true; // ตั้งค่าสถานะการแจ้งเตือน
          }
        }
    
    // for (int j = 1; j <= RELAY_AMOUNT && Option != 0; j++) {
    //   if (isInArray(j, valueSensor[i].SensorValueControl, valueSensorControlSize)) {
    //     bool conditionMet = false;
    //     // Serial.println("In Array Relay " + String(j) + " Option : " + String(Option) + " HighLimit : " + String(HighLimit) + " LowLimit : " + String(LowLimit));
    //     if ((Option == 1 || Option == 3) && MainSensorValue[i] > HighLimit) {
    //       ControlRelay(1, j);
    //       conditionMet = true;
    //       if (!valueSensor[i].notifySent || ((currentTime - valueSensor[i].lastNotifyTime >= valueSensor[i].interval) && valueSensor[i].interval > 0)) {
    //         if (isInArray(1, valueSensor[i].NotifyMethodValue, valueNotifyMethodSize)) {
    //           mqttSendNotify("ValueSensorNotify", 1, i, valueSensor[i].SensorSelected, MainSensorValue[i], "high");
    //         }
    //         else if(isInArray(2, valueSensor[i].NotifyMethodValue, valueNotifyMethodSize)) {
    //           mqttSendNotify("ValueSensorNotify", 2, i, valueSensor[i].SensorSelected, MainSensorValue[i], "high");
    //         }
    //         valueSensor[i].lastNotifyTime = currentTime;
    //         valueSensor[i].notifySent = true; // ตั้งค่าสถานะการแจ้งเตือน
    //       }
    //       // Serial.println("OPTION 1 | 3 AND VAL > HIGH");
    //     }
    //     else if ((Option == 1 || Option == 4) && MainSensorValue[i] < LowLimit) {
    //       ControlRelay(0, j);
    //       conditionMet = true;
    //       if (!valueSensor[i].notifySent || ((currentTime - valueSensor[i].lastNotifyTime >= valueSensor[i].interval) && valueSensor[i].interval > 0)) {
    //         if (isInArray(1, valueSensor[i].NotifyMethodValue, valueNotifyMethodSize)) {
    //           mqttSendNotify("ValueSensorNotify", 1, i, valueSensor[i].SensorSelected, MainSensorValue[i], "low");
    //         }
    //         else if(isInArray(2, valueSensor[i].NotifyMethodValue, valueNotifyMethodSize)) {
    //           mqttSendNotify("ValueSensorNotify", 2, i, valueSensor[i].SensorSelected, MainSensorValue[i], "low");
    //         }
    //         valueSensor[i].lastNotifyTime = currentTime;
    //         valueSensor[i].notifySent = true; // ตั้งค่าสถานะการแจ้งเตือน
    //       }
    //       // Serial.println("OPTION 1 | 4 AND VAL < LOW");
    //     }
    //     else if ((Option == 2 || Option == 5) && MainSensorValue[i] > HighLimit) {
    //       ControlRelay(0, j);
    //       conditionMet = true;
    //       if (!valueSensor[i].notifySent || ((currentTime - valueSensor[i].lastNotifyTime >= valueSensor[i].interval) && valueSensor[i].interval > 0)) {
    //         if (isInArray(1, valueSensor[i].NotifyMethodValue, valueNotifyMethodSize)) {
    //           mqttSendNotify("ValueSensorNotify", 1, i, valueSensor[i].SensorSelected, MainSensorValue[i], "high");
    //         }
    //         else if(isInArray(2, valueSensor[i].NotifyMethodValue, valueNotifyMethodSize)) {
    //           mqttSendNotify("ValueSensorNotify", 2, i, valueSensor[i].SensorSelected, MainSensorValue[i], "high");
    //         }
    //         valueSensor[i].lastNotifyTime = currentTime;
    //         valueSensor[i].notifySent = true; // ตั้งค่าสถานะการแจ้งเตือน
    //       }
    //       // Serial.println("OPTION 2 | 5 AND VAL > HIGH");
    //     }
    //     else if ((Option == 2 || Option == 6) && MainSensorValue[i] < LowLimit) {
    //       ControlRelay(1, j);
    //       conditionMet = true;
    //       if (!valueSensor[i].notifySent || ((currentTime - valueSensor[i].lastNotifyTime >= valueSensor[i].interval) && valueSensor[i].interval > 0)) {
    //         if (isInArray(1, valueSensor[i].NotifyMethodValue, valueNotifyMethodSize)) {
    //           mqttSendNotify("ValueSensorNotify", 1, i, valueSensor[i].SensorSelected, MainSensorValue[i], "low");
    //         }
    //         else if(isInArray(2, valueSensor[i].NotifyMethodValue, valueNotifyMethodSize)) {
    //           mqttSendNotify("ValueSensorNotify", 2, i, valueSensor[i].SensorSelected, MainSensorValue[i], "low");
    //         }
    //         valueSensor[i].lastNotifyTime = currentTime;
    //         valueSensor[i].notifySent = true; // ตั้งค่าสถานะการแจ้งเตือน
    //       }
    //       // Serial.println("OPTION 2 | 6 AND VAL < LOW");
    //     } else {
    //       // รีเซ็ตค่า lastNotifyTime เมื่อหลุดจากเงื่อนไข
    //       valueSensor[i].lastNotifyTime = currentTime;
    //       valueSensor[i].notifySent = false; // รีเซ็ตสถานะการแจ้งเตือน
    //     }
    //     Serial.println("lastNotifyTime : " + String(valueSensor[i].lastNotifyTime));
    //   }
    // }
      }
    }
  }
}

void ActionRelayStatusSensor(int numStatusSensor) {
  for (int i = 1; i <= numStatusSensor; i++) {
    int Option = statusSensor[i].StatusSensorValueOption;
    int statusSensorControlSize = sizeof(statusSensor[i].StatusSensorValueControl) / sizeof(statusSensor[i].StatusSensorValueControl[0]);
    for (int j = 1; j <= RELAY_AMOUNT && Option != 0; j++) {
      if (isInArray(j, statusSensor[i].StatusSensorValueControl, statusSensorControlSize)) {
        if ((Option == 1 || Option == 3) && StatusSensorValue[i]) {
          ControlRelay(1, j);
        }
        else if ((Option == 1 || Option == 4) && !StatusSensorValue[i]) {
          ControlRelay(0, j);
        }
        else if ((Option == 2 || Option == 5) && StatusSensorValue[i]) {
          ControlRelay(0, j);
        }
        else if ((Option == 2 || Option == 6) && !StatusSensorValue[i]) {
          ControlRelay(1, j);
        }
      }
    }
  }
}
