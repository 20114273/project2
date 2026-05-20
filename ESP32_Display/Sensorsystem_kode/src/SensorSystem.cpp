#include "Sensorsystem.h"


int v1 = 0;
int v2 = 0;
int v3 = 0;
double voltage = 0;
double T = 0;
char check;
char retning = 0;
int i = 0;
int avgV1 = 0;
int avgV2 = 0;
int avgT = 0;



void setupSensors() {
    // TX = 10, RX = 11 (Vælg pins der er ledige i dit hvide stik)
    Serial1.begin(9600, SERIAL_8N1, 11, 3);

    // Sæt opløsning og gør indgangen mere følsom over for dagslys-spændinger
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);

    Serial.println("--- system klar ---");
}
void checkSensors() {
    double sumV1=0;
    double sumV2=0;
    double sumT=0;
    int malinger = 10;


    for (int i = 0; i < malinger; i++) {
        double tempV1= analogRead(LDR_1);
        double tempV2= analogRead(LDR_2);
        double tempV3= analogRead(Temp);
        double tempVoltage = (tempV3 / 4095.0) * 3.3;
        double tempT = (tempVoltage-0.5)/0.01;

        sumV1 += tempV1;
        sumV2 += tempV2;
        sumT += tempT;
        delay(10);
    }
    avgV1 = sumV1 / malinger;
    avgV2 = sumV2 / malinger;
    avgT = sumT / malinger;

    bool ldrOK = (avgV1 < 4095 && avgV1 >= 15 && avgV2 < 4095 && avgV2 >= 15);
    bool tempOK = (avgT > -20 && avgT < 50);


    if (ldrOK && tempOK) {
            check = 'O'; // Alt er OK
            Serial.print("\t Sensor: OK");
        } else if (!ldrOK && !tempOK) {
            check = 'F'; // Begge fejler
            Serial.print("\t Sensor: Total Fejl (F)");
        } else if (!ldrOK) {
            check = 'L'; // Kun LDR fejler
            Serial.print("\t Sensor: LDR fejl");
        }  else if (!tempOK) {
            check = 'T'; // Kun Temp fejler
            Serial.print("\t Sensor: Temp fejl");
        }
    }


void sensorData() {
    v1 = analogRead(LDR_1) * 1.047;
    v2 = analogRead(LDR_2);
    v3 = analogRead(    Temp);
}

void updateSensors() {
    if (v1 < v2 - 200) {
        retning = 'U'; // UP
        Serial.print("\t retning: U");
    }
    else if (v1 > v2 + 200) {
        retning = 'D'; // DOWN
        Serial.print("\t retning: D");
    }
    else {
        retning = 'S'; // NOTHING
        Serial.print("\t retning: S");
    }
}
void printData() {
    // Print resultatet til terminalen i CLion
    Serial.print("\t LDR 1: "); Serial.print(v1);
    Serial.print("\t LDR 2: "); Serial.println(v2);
}

void updateTempSensor() {
    Serial.print("\t Temp: "); Serial.print(T);
    double voltage = (v3 / 4095.0) * 3.3;
    T = (voltage-0.5)/0.01;
}

void sendData() {
    Serial1.print(check);
    Serial1.print(",");
    Serial1.print(retning); // Sender retning
    Serial1.print("_");
    Serial1.print(avgT); // Sender Temp
    Serial1.print("-");
    Serial1.print(avgV1);
    Serial1.print("'");
    Serial1.println(avgV2);
}