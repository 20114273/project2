
#ifndef DISPLAY_DRIVER_H_
#define DISPLAY_DRIVER_H_

#include <stdint.h>



// globale værdier som main kan bruge
extern float temp;
extern char sidsteKommando;
extern int ldr;

// funktion
void opdaterSystemFraUART(void);
void visBesked();
void opstartBesked();
void opdaterBesked(const char*, const char*);
uint8_t hentSensorStatus();


#endif /* DISPLAY_DRIVER_H_ */