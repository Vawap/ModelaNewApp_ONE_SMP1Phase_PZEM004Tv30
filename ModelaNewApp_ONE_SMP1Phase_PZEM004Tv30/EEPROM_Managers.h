#ifndef EEPROM_MANAGERS_H
#define EEPROM_MANAGERS_H

#include <EEPROM.h>

int writeStrToEEPROM(int addr, const String str); // เขียนสตริงลง EEPROM และเพิ่ม newline ที่สิ้นสุด
String readEEPROM(); // อ่านเนื้อหาทั้งหมดจาก EEPROM
String readEEPROMFromPosition(int startAddr); // อ่านเนื้อหาจาก EEPROM โดยเริ่มจากตำแหน่งที่กำหนดและหยุดเมื่อพบ \n
void eraseEEPROMFromPosition(int startAddr); // ลบเนื้อหาจาก EEPROM โดยเริ่มจากตำแหน่งที่กำหนดและหยุดเมื่อพบ \n
void clearEEPROM(); // เคลียค่า EEPROM ทั้งหมด

void getAllModelaConfig(); // ดึงค่าทั้งหมดจาก EEPROM
void getModelaConfigMode(); // ดึงค่า Mode จาก EEPROM
void getModelaConfigWifi(); // ดึงค่า SSID และ Password จาก EEPROM
void getModelaToken(); // ดึงค่า tokenKey จาก EEPROM

void setModelaConfigMode(int mode); // ตั้งค่า Mode ลงใน EEPROM
void setModelaConfigWifi(String ssid, String password); // ตั้งค่า SSID และ Password ลงใน EEPROM
void setModelaToken(String tokenKey); // ตั้งค่า tokenKey ลงใน EEPROM
void setModelaFailConnected(int failConnected); // ตั้งค่า failConnected ลงใน EEPROM

void resetModelaConfig(); // Reset ค่า Config ต่างๆ

#endif
