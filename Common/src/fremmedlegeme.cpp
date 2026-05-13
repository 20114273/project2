#include "fremmedlegeme.h"
#include "uart.h"
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


float hentTemp() {
	char buffer[32];
	uint8_t i = 0;
	char c;

	while (1) {
		if (CharReady()) {
			c = ReadChar();

			// Gem alle tegn i buffer
			if (i < sizeof(buffer) - 1) {
				buffer[i++] = c;
			}

			// Når vi rammer slut-tegn -> parse
			if (c == '-' || c == '!') {
				buffer[i] = '\0';

				// Find start og slut på temperatur
				char *start = strchr(buffer, '_');
				char *end   = strchr(buffer, '-');

				if (end == NULL) {
					end = strchr(buffer, '!');
				}

				if (start != NULL && end != NULL && end > start) {
					char tempStr[10];
					uint8_t len = end - start - 1;

					if (len < sizeof(tempStr)) {
						strncpy(tempStr, start + 1, len);
						tempStr[len] = '\0';

						return atof(tempStr);
					}
				}

				// Reset buffer hvis parsing fejler
				i = 0;
			}
		}
	}
}

int hentLDR() {
	char buffer[40];
	uint8_t i = 0;
	char c;

	int LDR1 = 0;
	int LDR2 = 0;

	while (1) {
		if (CharReady()) {
			c = ReadChar();

			if (i < sizeof(buffer) - 1) {
				buffer[i++] = c;
			}

			if (c == '\n' || c == '\r') {
				buffer[i] = '\0';

				char *start = strchr(buffer, '-');
				char *mid   = strchr(buffer, '\'');

				if (start != NULL && mid != NULL && mid > start) {

					// direkte parsing (simpelt)
					LDR1 = atoi(start + 1);
					LDR2 = atoi(mid + 1);

					// ? returnér gennemsnit
					return (LDR1 + LDR2) / 2;
				}

				i = 0;
			}
		}
	}
}



void fjernFremmedlegeme(){
	
}
