#include "Setting.h"
#include "ModelaVariables.h"
#include "ModelaSensors.h"
#include "ModelaUtilityFunction.h"

#include <DHT.h>
#include <Wire.h>          //I2C
#include <ModbusMaster.h>  // RS485
#include <SoftwareSerial.h>



#include <PZEM004Tv30.h>


#define RXD2 16  
#define TXD2 17

//#define PZEM004Tv30
//#define pzem

PZEM004Tv30 pzem(Serial2, RXD2, TXD2);


//Set OLED
#include <Adafruit_GFX.h>  // OLED
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

#define OLED_RESET -1  // ที่อยู่ของจอ OLED ในโหมด I2C
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// Setup Sensor ก่อนเริ่มใช้งาน
void initSensor() {
  analogReadResolution(10);
  Wire.begin();  // สำหรับเซ็นเซอร์ที่ต้องการ I2C

  Serial.begin(115200);
 // pzem.begin();
  Serial2.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // เริ่มต้นการสื่อสารกับ PZEM-004T
  ///PZEM004Tv30();

  // OLED Text
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  OLED.clearDisplay();
}

// ======================================== Start All Value Sensor ======================================== //

void PZEM004Tv30_6(int sensorNumber, int readType) {

    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();
    float frequency = pzem.frequency();
    float pf = pzem.pf();

  if (readType == 1) {
    MainSensorValue[sensorNumber] = voltage;
  }
  else if (readType == 2) {
   MainSensorValue[sensorNumber] = current;
  }
  else if (readType == 3) {
    MainSensorValue[sensorNumber] = power;
  }
  else if (readType == 4) {
   MainSensorValue[sensorNumber] = energy;
  }
  else if (readType == 5) {
   MainSensorValue[sensorNumber] = pf;
  }
  else if (readType == 6) {
   MainSensorValue[sensorNumber] = frequency;
  }

}

void checkDisplayAndUnit(int sensorNumber, int selectSelected) {

  Serial.println("sensorNumber : " + String(sensorNumber) + " selectSelected : " + String(selectSelected));
  if (selectSelected >= 1 && selectSelected <= 3) {
    //Serial.println("เข้า 1 - 3");
    valueSensor[sensorNumber].SensorDisplay = "Temp";
    valueSensor[sensorNumber].SensorUnit = "C";
  }
  else if (selectSelected == 4) {
    //Serial.println("เข้า 4");
    valueSensor[sensorNumber].SensorDisplay = "Avg Temp";
    valueSensor[sensorNumber].SensorUnit = "C";
  }
  else if (selectSelected >= 5 && selectSelected <= 7) {
    //Serial.println("เข้า 5 - 7");
    valueSensor[sensorNumber].SensorDisplay = "Humidity";
    valueSensor[sensorNumber].SensorUnit = "C";
  }
  else if (selectSelected == 8) {
    valueSensor[sensorNumber].SensorDisplay = "Avg Humi";
    valueSensor[sensorNumber].SensorUnit = "%";
  }
  else if (selectSelected == 9) {
    valueSensor[sensorNumber].SensorDisplay = "SoilTemp";
    valueSensor[sensorNumber].SensorUnit = "C";
  }
  else if (selectSelected == 10) {
    valueSensor[sensorNumber].SensorDisplay = "SoilHumi";
    valueSensor[sensorNumber].SensorUnit = "%";
  }
  else if (selectSelected == 11) {
    valueSensor[sensorNumber].SensorDisplay = "SoilPH";
    valueSensor[sensorNumber].SensorUnit = "";
  }
  else if (selectSelected == 12) {
    valueSensor[sensorNumber].SensorDisplay = "SoilEC";
    valueSensor[sensorNumber].SensorUnit = "dS/m";
  }
  else if (selectSelected == 13) {
    valueSensor[sensorNumber].SensorDisplay = "SoilN";
    valueSensor[sensorNumber].SensorUnit = "mg/kg";
  }
  else if (selectSelected == 14) {
    valueSensor[sensorNumber].SensorDisplay = "SoilP";
    valueSensor[sensorNumber].SensorUnit = "mg/kg";
  }
  else if (selectSelected == 15) {
    valueSensor[sensorNumber].SensorDisplay = "SoilK";
    valueSensor[sensorNumber].SensorUnit = "mg/kg";
  }
  else if (selectSelected == 16) {
    valueSensor[sensorNumber].SensorDisplay = "SoilSal";
    valueSensor[sensorNumber].SensorUnit = "dS/m";
  }
  else if (selectSelected == 17) {
    valueSensor[sensorNumber].SensorDisplay = "Temp";
    valueSensor[sensorNumber].SensorUnit = "C";
  }
  else if (selectSelected == 18) {
    valueSensor[sensorNumber].SensorDisplay = "Humidity";
    valueSensor[sensorNumber].SensorUnit = "%";
  }
  else if (selectSelected == 19) {
    valueSensor[sensorNumber].SensorDisplay = "VPD";
    valueSensor[sensorNumber].SensorUnit = "kPa";
  }
  else if (selectSelected == 20) {
    valueSensor[sensorNumber].SensorDisplay = "HeatInd.";
    valueSensor[sensorNumber].SensorUnit = "C";
  }
}

void additionOLED() {

  OLED.clearDisplay();
  for (int i = 1; i <= SensorAmount; i++) {  // วนลูปสำหรับแต่ละเซ็นเซอร์

    checkDisplayAndUnit(i, valueSensor[i].SensorSelected);

    int cursorPosition = (i - 1) * 16;

    OLED.setTextSize(1);
    OLED.setTextColor(WHITE);
    OLED.setCursor(0, cursorPosition);
    OLED.print(valueSensor[i].SensorDisplay);
    //OLED.print(" ");
    OLED.setCursor(55, cursorPosition);
    OLED.print(MainSensorValue[i]);
    //OLED.print(" ");
    OLED.setCursor(95, cursorPosition);
    OLED.println(valueSensor[i].SensorUnit);

  }
  OLED.display();  // แสดงผลลัพธ์บนหน้
}

// ======================================== End All Value Sensor ======================================== //




// ======================================== Start All Status Sensor ======================================== //

void FloatingValueFunction(int sensorNumber, int inputType) {
  uint8_t bufferStatus = 0;
  if (inputType == 1) {  // D5
    bufferStatus = digitalRead(D5);
  } else if (inputType == 2) {  // D6
    bufferStatus = digitalRead(D6);
  } else if (inputType == 3) {  // D7
    bufferStatus = digitalRead(D7);
  }
  StatusSensorValue[sensorNumber] = map(bufferStatus, 0, 1, 1, 0);
  //Serial.print("Status Sensor FloatingValue : " + String(StatusSensorValue[sensorNumber]));
}

void ElectricFenceFunction(int sensorNumber, int inputType) {

  uint8_t bufferStatus = 0;
  uint8_t loopalarmJ1, loopalarmJ2;
  uint8_t StatusSensor;

  if (inputType == 1) {  // D5
    bufferStatus = digitalRead(D5);

    if (bufferStatus == 1) {
      loopalarmJ1 = loopalarmJ1 + 1;
    }

    if (loopalarmJ1 > 3) {
      bufferStatus = 1;
    }
    if (bufferStatus == 0) {
      StatusSensor = 0;
      loopalarmJ1 = 0;
    }

  } else if (inputType == 2) {  // D6
    bufferStatus = digitalRead(D6);

    // ถ้าค่าของขา D6 เป็น 1
    if (bufferStatus == 1) {
      // เพิ่มค่า loopalarmJ2 ขึ้น 1
      loopalarmJ2++;
    }

    // ถ้าค่าของ loopalarmJ2 มากกว่า 3
    if (loopalarmJ2 > 3) {
      // กำหนดค่า StatusSensorAX เป็น 1
      StatusSensor = 1;
    }

    // ถ้าค่าของขา D6 เป็น 0
    if (bufferStatus == 0) {
      // กำหนดค่า StatusSensorAX เป็น 0
      StatusSensor = 0;
      // รีเซ็ตค่าของ loopalarmJ2 เป็น 0
      loopalarmJ2 = 0;
    }
  }

  StatusSensorValue[sensorNumber] = StatusSensor;
  //Serial.print("Status Sensor ElectricFence : " + String(StatusSensorValue[sensorNumber]));
}

void AlarmFenceFunction(int sensorNumber, int inputType) {

  uint8_t bufferStatus = 0;
  uint8_t loopalarmJ3, loopalarmJ4;
  uint8_t StatusSensor;

  if (inputType == 1) {  // D7
    bufferStatus = digitalRead(D7);

    if (bufferStatus == 1) {
      loopalarmJ3 = loopalarmJ3 + 1;
    }

    if (loopalarmJ3 > 3) {
      StatusSensor = 1;
    }
    if (bufferStatus == 0) {
      StatusSensor = 0;
      loopalarmJ3 = 0;
    }

  } else if (inputType == 2) {  // D6
    bufferStatus = digitalRead(A0);

    if (bufferStatus >= 930) {
      loopalarmJ4 = loopalarmJ4 + 1;
    }

    if (loopalarmJ4 > 3) {
      StatusSensor = 1;
    }
    if (bufferStatus <= 929) {
      StatusSensor = 0;
      loopalarmJ4 = 0;
    }
  }

  StatusSensorValue[sensorNumber] = StatusSensor;
  //Serial.print("Status Sensor AlarmFence : " + String(StatusSensorValue[sensorNumber]));
}

void PIRstatusFunction(int sensorNumber, int inputType) {

  uint8_t bufferStatus = 0;
  if (inputType == 1) {  // D5
    bufferStatus = digitalRead(D5);
  } else if (inputType == 2) {  // D6
    bufferStatus = digitalRead(D6);
  } else if (inputType == 3) {  // D7
    bufferStatus = digitalRead(D7);
  }

  StatusSensorValue[sensorNumber] = bufferStatus;
  //Serial.print("Status Sensor PIRstatus : " + String(StatusSensorValue[sensorNumber]));
}

void GeneralHighLevelFunction(int sensorNumber, int inputType) {
  uint8_t bufferStatus = 0;

  if (inputType == 1) {  // D5
    bufferStatus = digitalRead(D5);
  } else if (inputType == 2) {  // D6
    bufferStatus = digitalRead(D6);
  } else if (inputType == 3) {  // D7
    bufferStatus = digitalRead(D7);
  }

  StatusSensorValue[sensorNumber] = map(bufferStatus, 0, 1, 1, 0);
  //Serial.print("Status Sensor GeneralHighLevel : " + String(StatusSensorValue[sensorNumber]));
}

void GeneralLowLevelFunction(int sensorNumber, int inputType) {
  uint8_t bufferStatus = 0;

  if (inputType == 1) {  // D5
    bufferStatus = digitalRead(D5);
  } else if (inputType == 2) {  // D6
    bufferStatus = digitalRead(D6);
  } else if (inputType == 3) {  // D7
    bufferStatus = digitalRead(D7);
  }

  StatusSensorValue[sensorNumber] = bufferStatus;
  //Serial.print("Status Sensor GeneralLowLevel : " + String(StatusSensorValue[sensorNumber]));
}

void PIRPowerstatusFunction(int sensorNumber, int inputType, int readType) {

  uint8_t bufferStatus = 0;
  uint8_t PowerON, PowerOFF;
  int powerCut;

  if (inputType == 1) {  // D5
    bufferStatus = digitalRead(D5);
  } else if (inputType == 2) {  // D6
    bufferStatus = digitalRead(D6);
  } else if (inputType == 3) {  // D7
    bufferStatus = digitalRead(D7);
  }

  // แปลง bufferStatus เป็น powerCut
  powerCut = map(bufferStatus, 0, 1, 1, 0);

  // ตรวจสอบและกำหนดค่า StatusSensor
  if (powerCut == 1) {
    PowerON = 0;
  } else if (powerCut == 0) {
    PowerOFF = 1;
  }

  if (readType == 1) {
    StatusSensorValue[sensorNumber] = PowerON;
  } else if (readType == 2) {
    StatusSensorValue[sensorNumber] = PowerON;
  } else if (readType == 3) {
    StatusSensorValue[sensorNumber] = PowerON;
  } else if (readType == 4) {
    StatusSensorValue[sensorNumber] = PowerOFF;
  } else if (readType == 5) {
    StatusSensorValue[sensorNumber] = PowerOFF;
  } else if (readType == 6) {
    StatusSensorValue[sensorNumber] = PowerOFF;
  }

  Serial.print("Status Sensor PowerStatus : " + String(StatusSensorValue[sensorNumber]));
}

// ======================================== End All Status Sensor ======================================== //
