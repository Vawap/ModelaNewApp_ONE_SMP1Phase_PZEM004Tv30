
// #include "Setting.h"
// #include "ModelaVariables.h"
// #include <Preferences.h>

// Preferences preferences;

// // ============================== Start All Function Preference ============================== //
// // ฟังก์ชันเขียนสตริงลงใน Preference
// void writeStrToPreference(const char* key, const String& str) {
//   preferences.putString(key, str); // เขียนค่าลงใน Preference
//   preferences.end(); // สิ้นสุดการใช้งาน preferences
// }

// // ฟังก์ชันอ่านสตริงจาก Preference
// String readPreference(const char* key) {
//   String value = preferences.getString(key, ""); // อ่านค่าจาก Preference
//   preferences.end(); // สิ้นสุดการใช้งาน preferences
//   return value;
// }

// void getAllModelaConfig() {
//   getModelaConfigMode(); // ดึงค่า Mode จาก Preference
//   getModelaConfigWifi(); // ดึงค่า SSID และ Password จาก Preference
//   getModelaToken(); // ดึงค่า tokenKey จาก Preference
// }

// // ดึงค่า Mode จาก Preference
// void getModelaConfigMode() {
//   modeStr = readPreference("mode"); // อ่านโหมดการทำงานจาก Preference
//   mode = modeStr.toInt(); // แปลงสตริงเป็น int
//   Serial.println("mode : " + String(mode));
// }

// // ดึงค่า SSID และ Password จาก Preference
// void getModelaConfigWifi() {
//   ssid = readPreference("ssid"); // อ่าน SSID จาก Preference
//   password = readPreference("password"); // อ่าน Password จาก Preference
//   Serial.println("ssid : " + String(ssid));
//   Serial.println("password : " + String(password));
// }

// // ดึงค่า tokenKey จาก Preference
// void getModelaToken() {
//   tokenKey = readPreference("tokenKey"); // อ่าน tokenKey จาก Preference
//   Serial.println("tokenKey : " + String(tokenKey));
// }

// // ฟังก์ชันเขียนค่าลงใน Preference
// void setModelaConfigMode(int mode) {
//   writeStrToPreference("mode", String(mode)); // เขียนโหมดการทำงานลงใน Preference
// }

// // ฟังก์ชันเขียนค่า SSID และ Password ลงใน Preference
// void setModelaConfigWifi(String ssid, String password) {
//   writeStrToPreference("ssid", ssid); // เขียน SSID ลงใน Preference
//   writeStrToPreference("password", password); // เขียน Password ลงใน Preference
// }

// // ฟังก์ชันเขียนค่า tokenKey ลงใน Preference
// void setModelaToken(String tokenKey) {
//   writeStrToPreference("tokenKey", tokenKey); // เขียน tokenKey ลงใน Preference
// }

// // Reset ค่า Config ต่างๆ
// void resetModelaConfig() {
//   preferences.remove("mode"); // ลบโหมดการทำงาน
//   preferences.remove("ssid"); // ลบ SSID
//   preferences.remove("password"); // ลบ Password
//   preferences.remove("tokenKey"); // ลบ tokenKey
//   setModelaConfigMode(1); // กำหนดโหมดเป็น BLE
// }
// // ============================== End All Function Preference ============================== //
