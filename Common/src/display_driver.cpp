/*
 * display_driver.cpp
 *
 * Created: 06/05/2026 16:46:38
 *  Author: Emil
 */ 

#include "display_driver.h"
#include "ArduinoCompat.h"
#include "LiquidCrystal.h"
#include "string.h"
#include "uart.h"

extern LiquidCrystal lcd; //Bruger objekt lcd defineret i en anden fil (i main.cpp) derfor extern

void opstartBesked(){
	lcd.clear();
	lcd.begin(16,2);
	lcd.home();
	lcd.print("Starter system..");
	
}

void visBesked(){
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Batteri: ");
	lcd.setCursor(0,1);
	lcd.print("Gen.eff: ");
}

void opdaterBesked(const char* batt_niveau, const char* gen_eff){
	// For at undgå unødvendige opdateringer på LCD-displayet, sammenlignes de nye værdier med de tidligere viste værdier. Static gemmer sin værdi på tværs af iterationer.
	static char forrige_batt_niveau[10] = "";
	static char forrige_gen_eff[5] = "";

	//Opdater kun hvis værdierne er forskellige.
	if (strcmp(batt_niveau, forrige_batt_niveau) != 0) { // Sammenlign indholdet
        lcd.setCursor(9,0);
        lcd.print("    "); // Sletter tidligere værdi
        lcd.setCursor(9,0);
        lcd.print(batt_niveau);
        lcd.print("%");
        strcpy(forrige_batt_niveau, batt_niveau); // Opdaterer forrige værdi med nuværende værdi.
    }

	if (strcmp(gen_eff, forrige_gen_eff) != 0) { // Sammenlign indholdet
        lcd.setCursor(9,1);
        lcd.print("     "); // Sletter tidligere værdi
        lcd.setCursor(9,1);
        lcd.print(gen_eff);
        lcd.print("W");
        strcpy(forrige_gen_eff, gen_eff); // Kopier strengen, ikke pointeren!
    }
	
}

uint8_t hentSensorStatus(){
	while(ReadChar() != "F" || "L" || "T" || "O" ){
		;
	}
	char modtagetStatus = ReadChar();
	
	if (modtagetStatus == "F") {
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Lys: Fejl");
		lcd.setCursor(0,1);
		lcd.print("Temp: Fejl: ");
		return 1;
	}

	if (modtagetStatus == "L") {
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Lys: Fejl");
		lcd.setCursor(0,1);
		lcd.print("Temp: OK");
		return 1;
	}

	if (modtagetStatus == "T") {
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Lys: OK");
		lcd.setCursor(0,1);
		lcd.print("Temp: Fejl");
		return 1;

	if (modtagetStatus == "O") {
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Lys: OK");
		lcd.setCursor(0,1);
		lcd.print("Temp: OK");
		return 0;
		}
	}
}
