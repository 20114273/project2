#include <Arduino.h>
#include "Sensorsystem.h"

void setup() {
    Serial.begin(115200);
    delay(1000);
    //kald setup af sensore
    setupSensors();

}
void loop() {
    checkSensors();
    sensorData();
    updateSensors();
    printData();
    updateTempSensor();
    sendData();
    delay(1000);
}