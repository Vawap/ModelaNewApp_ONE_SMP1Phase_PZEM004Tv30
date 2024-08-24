#ifndef MODELA_UTILITIES_FUNCTION_H
#define MODELA_UTILITIES_FUNCTION_H

bool isInArray(int value, int arr[], int arrSize);
void currentTimeSetup();
void printLocalTime();
void blinkLED(int delayTime, String txt = "");
double mapf(double PHval, double in_min, double in_max, double out_min, double out_max);
double Levelmapf(double Levelval, double in_min, double in_max, double out_min, double out_max);
double LevelmapfJ2(double Levelval, double in_min, double in_max, double out_min, double out_max);

#endif
