#ifndef MODELA_CONFIG_MODE_H
#define MODELA_CONFIG_MODE_H

#include "Setting.h"
#include "ModelaVariables.h"
#include "EEPROM_Managers.h"

// Function prototypes
void initEEPROM();
void initBoardPins();
void setupMode(int mode);
void setupBLE();
void setupWiFi();
void runBLE();
void runWiFi();
void debugMode();

#endif // MODELA_CONFIG_MODE_H
