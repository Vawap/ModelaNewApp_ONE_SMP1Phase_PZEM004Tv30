#ifndef ALL_SENSOR_MANAGERS_H
#define ALL_SENSOR_MANAGERS_H

// Setup Sensor ก่อนเริ่มใช้งาน
void initSensor();

// function สำหรับอ่านค่า Sensor ต่างๆ
void dhtTempFunction(int sensorNumber, int inputType); // อ่านค่าอุณหภูมิจาก DHT
void dhtHumiFunction(int sensorNumber, int inputType); // อ่านค่าความชื้นจาก DHT

// function สำหรับอ่านค่า Status Sensor ต่างๆ
void FloatingValueFunction(int sensorNumber, int inputType); // อ่านค่าลูกลอยไฟฟ้า
void ElectricFenceFunction(int sensorNumber, int inputType); // อ่านค่าการจ่ายไฟเข้ารั้วไฟฟ้า
void AlarmFenceFunction(int sensorNumber, int inputType); // อ่านค่าการแจ้งเตือนจากรั้วไฟฟ้า
void PIRstatusFunction(int sensorNumber, int inputType); // อ่านค่าการตรวจจับผู้บุกรุก
void GeneralHighLevelFunction(int sensorNumber, int inputType); // อ่านค่าการตรวจจับสถานะ Active High
void GeneralLowLevelFunction(int sensorNumber, int inputType); // อ่านค่าการตรวจจับสถานะ Active Low
void PIRPowerstatusFunction(int sensorNumber, int inputType, int readType); // อ่านค่าตรวจสอบกระแสไฟดับ

void readPower_1PhaseRS485(int sensorNumber, uint8_t id, int readType);
void readEnergy_1PhaseRS485(int sensorNumber, uint8_t id, int readType);

void PZEM004Tv30_6(int sensorNumber, int readType);

void additionOLED();

#endif
