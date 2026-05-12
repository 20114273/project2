#include <Arduino.h>

void setup() {
  // Kommunikation til computeren (USB)
  Serial.begin(115200); 
  
  // Kommunikation til Arduino Mega (Pins fra dit billede)
  // Vi bruger GPIO17 som TX. RX (GPIO18) er ikke strengt nødvendig her,
  // men vi definerer den for god ordens skyld.
  Serial1.begin(9600, SERIAL_8N1, 18, 17); 
  
  Serial.println("ESP32-S3 Sender er startet...");
}

void loop() {
  // Send en besked til Arduino Mega
  Serial1.println("Besked fra ESP32 LCD-modul!");
  
  // Print det også til VS Code Serial Monitor så du ved den kører
  Serial.println("Sender data til Mega...");
  
  delay(2000); // Send hvert 2. sekund
}