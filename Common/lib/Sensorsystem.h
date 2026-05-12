#pragma once
#include <Arduino.h>
// Definer de pins der sidder i det hvide stik på dit Waveshare board
// ESP32-S3 pins: 18, 17, 16 og 3
const int LDR_1 = 18;
const int LDR_2 = 17;
const int Temp = 16;


extern int v1;
extern int v2;
extern int v3;
extern double T;
extern double voltage;
extern int check;
extern char retning;
extern int i;

void checkSensors();
void setupSensors();
void sensorData();
void updateSensors();
void updateTempSensor();
void printData();
void sendData();