
#include "Setting.h"
#include "ModelaVariables.h"
#include "EEPROM_Managers.h"

// ============================== Start All Function EEPROM ============================== //
// ฟังก์ชันเขียนสตริงลง EEPROM และเพิ่ม newline ที่สิ้นสุด
int writeStrToEEPROM(int addr, const String str) {
  int l = str.length();
  for (int i = 0; i < l; i++) {
    EEPROM.write(addr++, str[i]); // เขียนแต่ละตัวอักษรลง EEPROM
  }
  EEPROM.write(addr, '\n'); // เพิ่มตัวอักษร newline ที่สิ้นสุดสตริง
  EEPROM.commit(); // ยืนยันการเขียนข้อมูล
  return addr + 1; // คืนค่าตำแหน่งที่อยู่ถัดไปใน EEPROM
}

// ฟังก์ชันอ่านเนื้อหาทั้งหมดจาก EEPROM
String readEEPROM() {
  String content = "";
  for (int i = 0; i < EEPROM_SIZE; i++) {
    char ch = EEPROM.read(i); // อ่านตัวอักษรจาก EEPROM
    // if (ch == '\0') break; // หยุดอ่านเมื่อพบ null character
    content += ch; // เพิ่มตัวอักษรลงในสตริง
  }
  return content; // คืนค่าเนื้อหาที่อ่านได้
}

// ฟังก์ชันอ่านเนื้อหาจาก EEPROM โดยเริ่มจากตำแหน่งที่กำหนดและหยุดเมื่อพบ \n
String readEEPROMFromPosition(int startAddr) {
  String content = "";
  char ch;
  for (int i = startAddr; i < EEPROM_SIZE; i++) {
    ch = EEPROM.read(i); // อ่านตัวอักษรจาก EEPROM
    if (ch == '\n') break; // หยุดอ่านเมื่อพบตัวอักษร newline
    content += ch; // เพิ่มตัวอักษรลงในสตริง
  }
  return content; // คืนค่าเนื้อหาที่อ่านได้
}

// ฟังก์ชันลบเนื้อหาจาก EEPROM โดยเริ่มจากตำแหน่งที่กำหนดและหยุดเมื่อพบ \n
void eraseEEPROMFromPosition(int startAddr) {
  for (int i = startAddr; i < EEPROM_SIZE; i++) {
    char ch = EEPROM.read(i); // อ่านตัวอักษรจาก EEPROM
    if (ch == '\n') break; // หยุดเมื่อพบตัวอักษร newline
    EEPROM.write(i, 0); // ลบข้อมูลใน EEPROM ที่ตำแหน่งนี้
  }
}

// เคลียค่า EEPROM ทั้งหมด
void clearEEPROM() {
  for (int i = 0; i < EEPROM_SIZE; i++) {
    EEPROM.write(i, 0); // เขียนค่า 0 ลงในทุกตำแหน่งของ EEPROM
  }
  EEPROM.commit(); // ยืนยันการเขียนข้อมูล
}

void getAllModelaConfig() {
  getModelaConfigMode(); // ดึงค่า Mode จาก EEPROM
  getModelaConfigWifi(); // ดึงค่า SSID และ Password จาก EEPROM
  getModelaToken(); // ดึงค่า tokenKey จาก EEPROM
}

// ดึงค่า Mode จาก EEPROM
void getModelaConfigMode() {
  modeStr = readEEPROMFromPosition(0); // อ่านโหมดการทำงานจาก EEPROM
  mode = modeStr.toInt(); // แปลงสตริงเป็น int
  Serial.println("mode : " + String(mode));
}

// ดึงค่า SSID และ Password จาก EEPROM
void getModelaConfigWifi() {
  ssid = readEEPROMFromPosition(11); // อ่าน SSID
  password = readEEPROMFromPosition(41); // อ่าน Password
  Serial.println("ssid : " + String(ssid));
  Serial.println("password : " + String(password));
}

// ดึงค่า tokenKey จาก EEPROM
void getModelaToken() {
  tokenKey = readEEPROMFromPosition(71); // อ่าน tokenKey
  Serial.println("tokenKey : " + String(tokenKey));
}

// ฟังก์ชันเขียนค่าลง EEPROM
void setModelaConfigMode(int mode) {
  writeStrToEEPROM(0, String(mode)); // เขียนโหมดการทำงานลง EEPROM
}

// ฟังก์ชันเขียนค่า SSID และ Password ลง EEPROM
void setModelaConfigWifi(String ssid, String password) {
  writeStrToEEPROM(11, ssid); // เขียน SSID ลง EEPROM
  writeStrToEEPROM(41, password); // เขียน Password ลง EEPROM
}

// ฟังก์ชันเขียนค่า tokenKey ลง EEPROM
void setModelaToken(String tokenKey) {
  writeStrToEEPROM(71, tokenKey); // เขียน tokenKey ลง EEPROM
}

// ฟังก์ชันเขียนค่า failConnected ลง EEPROM
void setModelaFailConnected(int failConnected) {
  writeStrToEEPROM(101, String(failConnected)); // เขียนค่า failConnected ลง EEPROM
}

// Reset ค่า Config ต่างๆ
void resetModelaConfig() {
  eraseEEPROMFromPosition(11); // ลบ SSID
  eraseEEPROMFromPosition(41); // ลบ Pass
  eraseEEPROMFromPosition(71); // ลบ Key
  writeStrToEEPROM(0, "1"); // กำหนดโหมดเป็น BLE
}
// ============================== End All Function EEPROM ============================== //
