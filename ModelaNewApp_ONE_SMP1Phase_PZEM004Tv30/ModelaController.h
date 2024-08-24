#ifndef MODELA_CONTROLLER_H
#define MODELA_CONTROLLER_H

// function สำหรับการควบคุม Value Sensor
void SetSelectValueSensor(int SelectValue, int SelectSensor); // เลือกดูค่า Sensor แต่ละ Value
void SetCalibrateSensor(int SelectValue, float CalibrateSensor); // Calibrate ค่าเซนเซอร์
void SetLimitValueSensor(int SelectValue, int typeValue, float LimitValue); // Set Limit Sensor ค่าควคุมด้านสูงและต่ำ
void SetAutoMode(int SelectRelay, int AutoMode); // Set ค่า AutoMode ของ Relay ที่ต้องการ 0 = Manual, 1 = Auto
void SetSensorValueOption(int SelectValue, int SensorOption); // Set ค่า Option ของ Sensor ที่ต้องการ
void ClearSensorControl(int sensorNO); // ฟังก์ชันในการล้างค่า SensorValueControl
void SetSensorControl(int SelectValue, int SensorControl, int SensorControlIndex); // Set Relay ที่ต้องการควบคุม
void SetSensorAmount(int SensorAmount); // Update จำนวน Sensor ที่ต้องการใช้งาน

// function สำหรับการควบคุม Status Sensor
void SetSelectStatusSensorValue(int SelectValue, int SensorValue); // เลือกดูค่า Status Sensor แต่ละ Value
void SetStatusSensorValueOption(int SelectValue, int SensorOption); // Set ค่า Option ของ Status Sensor ที่ต้องการ
void ClearStatusSensorControl(int sensorNO); // ฟังก์ชันในการล้างค่า StatusSensorValueControl
void SetStatusSensorControl(int SelectValue, int SensorControl, int StatusSensorControlIndex); // Set Relay ที่ต้องการควบคุม Status Sensor

// function สำหรับการควบคุม Relay
void RelayOn(int switchRelay);
void RelayOff(int switchRelay);
void updateRelay(int switchRelay);

// function สำหรับการควบคุม Button บน Board
void PhysicalButton();
void ResetButton();

void ClearTimerControl(int TimerNo);
void SetTimerSwitch(int SetType, int TimerNo, int TimerOption,
                    int *TimerControl, int TimerControlCount,
                    int *TimerDay, int TimerDayCount,
                    int *TimerHour, int TimerHourCount,
                    int *TimerMin, int TimerMinCount,
                    int TimerStatus = 1, int newTimerAmount = 1);
void SetTimerSwitchStatus(int TimerNo, int TimerStatus);

void ValueSensorControl(int numSensor);
void ValueStatusSensorControl(int numStatusSensor);
void valueSensorDataSendMQTT(int numSensor);
void valueStatusSensorDataSendMQTT(int numSensor);

void ActionRelaySensor(int numSensor);
void ActionRelayStatusSensor(int numStatusSensor);
void ControlRelay(int typeControl, int relayStatus, int sensorControl);

void SetNotifyIntervalSensor(int SelectValue, int NotifyInterval);
void ClearNotifyMethodSensor(int sensorNO);
void SetNotifyMethodSensor(int SelectValue, int NotifyMethod, int SensorNotifyIndex);

#endif
