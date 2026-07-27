#ifndef DEBUG_TEST_H
#define DEBUG_TEST_H

#include <stdint.h>

void DEBUG_HeartBeat(uint8_t blinks);      /* Sistem ayakta m?? */
uint8_t DEBUG_GpioSelfTest(void);          /* 1 = geçti, 0 = kald? */
void DEBUG_ButtonEchoLoop(void);           /* Buton -> LED canl? test */
void DEBUG_LcdSelfTest(void);      /* Statik gorsel dogrulama  */
void DEBUG_LcdCounterLoop(void);   /* Canli/surekli dogrulama  */
void DEBUG_Hc595WalkTest(void);   /* LCD'den BAGIMSIZ shift-register testi */
void DEBUG_LcdScrollTest(void);        /* Iki modun gorsel dogrulamasi */
void DEBUG_LcdMarqueeAppLoop(void);    /* Non-blocking gercek uygulama ornegi */
#endif /* DEBUG_TEST_H */