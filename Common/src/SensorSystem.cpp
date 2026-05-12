#include "Sensorsystem.h"


int v1 = 0;
int v2 = 0;
int v3 = 0;
double voltage = 0;
double T = 0;
int check = 0;
char retning = 0;
int i = 0;



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
    int i = 0;

    for (i; i < malinger; i++) {
        int tempV1= analogRead(LDR_1);
        int tempV2= analogRead(LDR_2);
        double tempV3= analogRead(Temp);
        double tempVoltage = (tempV3 / 4095.0) * 3.3;
        double tempT = (tempVoltage-0.5)/0.01;

        sumV1 += tempV1;
        sumV2 += tempV2;
        sumT += tempT;
        delay(10);
    }
        double avgV1 = sumV1 / malinger;
        double avgV2 = sumV2 / malinger;
        double avgT = sumT / malinger;

            if (avgV1 < 4095 && avgV2 < 4095 && avgV1 > 0 && avgV2 > 0 && avgT > -20 && avgT < 50 ) {
                check = 3;
                Serial.print("\t Sensor: OK");
            }else if ( avgV1 < 4095 && avgV2 < 4095 && avgV1 > 0 && avgV2 > 0 && avgT < -20 && avgT > 50 ) {
                check = 2;
                Serial.print("\t Sensor: Temp fejl");
            } else if (avgV1 == 4095 || avgV2 == 4095 || avgV1 == 0 || avgV2 == 0 &&avgT > -20 && avgT < 50 ) {
            check = 1;
                Serial.print("\t Sensor: LDR fejl");
            }
            else {
                check = 0;
                Serial.print("\t Sersor: Fejl");
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
    Serial1.print(',');
    Serial1.print(retning); // Sender retning
    Serial1.print(",");
    Serial1.println(T); // Sender Temp
}