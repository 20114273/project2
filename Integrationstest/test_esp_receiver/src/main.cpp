#include <Arduino.h>

// Her lader vi som om vi har en fælles besked-struktur i common/lib
// #include "common_defs.h" 

void setup() {
  // Start Serial til computeren (USB)
  Serial.begin(115200);
  
  // Start UART til kommunikation med Mega2560
  // S3 bruger ofte pins 17 (TX) og 18 (RX) som standard for Serial1
  Serial1.begin(9600, SERIAL_8N1, 18, 17); 

  Serial.println("ESP32 Test-modtager klar...");
}

void loop() {
  // Tjek om der er kommet data fra ATmega2560
  if (Serial1.available()) {
    String incoming = Serial1.readStringUntil('\n');
    
    // Print til din computers skærm så du kan se det i VS Code
    Serial.print("Modtaget fra Mega: ");
    Serial.println(incoming);
    
    // Her ville du senere sende det videre til dit Waveshare display
  }
}