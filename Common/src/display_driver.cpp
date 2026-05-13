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
#include <stdlib.h>

// ===== globale variabler =====
char uartBuffer[50];
uint8_t uartIndex = 0;
float temp = 0;
char sidsteKommando = 'S';
int ldr = 0;

extern LiquidCrystal lcd; //Bruger objekt lcd defineret i en anden fil (i main.cpp) derfor extern

void opstartBesked(){
	lcd.begin(16,2);
	lcd.clear();
	lcd.home();
	lcd.print("Starter system..");
	_delay_ms(500);
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
	char modtagetStatus = 'K';
	
	// Vent på at vi modtager et af de gyldige tegn
	for (int i=0; i<21; i++)
	{
		// 1. Tjek om der er data klar på UART-linjen
		if (CharReady())
		{
			char midlertidig = ReadChar();
			
			// 2. Hvis tegnet er et af de gyldige, så gem det og afbryd loopet
			if (midlertidig == 'F' || midlertidig == 'L' || midlertidig == 'T' || midlertidig == 'O')
			{
				modtagetStatus = midlertidig;
				break; 
			}
		}
		_delay_ms(50);
	}
	
	if (modtagetStatus == 'F') {
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Lys: Fejl");
		lcd.setCursor(0,1);
		lcd.print("Temp: Fejl");
		return 1;
	}

	if (modtagetStatus == 'L') {
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Lys: Fejl");
		lcd.setCursor(0,1);
		lcd.print("Temp: OK");
		return 1;
	}

	if (modtagetStatus == 'T') {
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Lys: OK");
		lcd.setCursor(0,1);
		lcd.print("Temp: Fejl");
		return 1;
	}

	if (modtagetStatus == 'O') {
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Lys: OK");
		lcd.setCursor(0,1);
		lcd.print("Temp: OK");
		return 0;
	}
		
	if (modtagetStatus == 'K') {
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Komm.-fejl!");
		return 1;
	}
		
	return 1;
	}

void opdaterSystemFraUART()
{
	while (CharReady())
	{
		char c = ReadChar();

		// 1. Gem i buffer
		if (uartIndex < sizeof(uartBuffer) - 1) {
			uartBuffer[uartIndex++] = c;
			uartBuffer[uartIndex] = '\0'; // Hold altid strengen termineret
			} else {
			uartIndex = 0; // Buffer overflow beskyttelse
		}

		// 2. Tjek for kommandoer øjeblikkeligt (U, D, S)
		if (c == 'U' || c == 'D' || c == 'S') {
			sidsteKommando = c;
		}

		// 3. Parser når hele pakken er modtaget (vi leder efter slut-tegnet ')
		if (c == '\'')
		{
			char *startT = strchr(uartBuffer, '_');
			char *endT   = strchr(uartBuffer, '-');
			char *split  = strchr(uartBuffer, '\'');

			if (startT && endT && split && endT > startT)
			{
				// Parse Temperatur
				char tempStr[10];
				uint8_t len = endT - startT - 1;
				if (len < sizeof(tempStr)) {
					strncpy(tempStr, startT + 1, len);
					tempStr[len] = '\0';
					temp = atof(tempStr);
				}

				// Parse LDR (Vi antager formatet -LDR1'LDR2)
				// Finder tallet efter '-' og tallet efter den forrige LDR værdi
				char *midL = strchr(endT, '-');
				if(midL) {
					int ldr1 = atoi(midL + 1);
					// Her skal du være sikker på hvordan din ESP sender LDR2.
					// Hvis den sender -LDR1'LDR2, så:
					int ldr2 = atoi(split + 1);
					ldr = (ldr1 + ldr2) / 2;
				}
			}
			// Reset buffer efter hver pakke eller forsøg
			uartIndex = 0;
			memset(uartBuffer, 0, sizeof(uartBuffer));
		}
	}
}