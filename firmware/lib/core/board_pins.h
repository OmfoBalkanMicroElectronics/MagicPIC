#ifndef BOARD_PINS_H
#define BOARD_PINS_H

#include "gpio_hal.h"

/* =========================================================
 *  PIC16F886 28-pin pin butcesi  (HS, 20 MHz harici kristal)
 *  - RA7/OSC1, RA6/OSC2 : KRISTAL -> I/O olarak KULLANILAMAZ
 *  - RE3/MCLR : reset (MCLRE = ON), 10k pull-up + 100n
 *  - RB3/PGM  : LVP = ON -> REZERVE
 *  Serbest I/O: RA0..RA5, RB0..RB2, RB4..RB7, RC0..RC7
 * ========================================================= */

#define BOARD_RB3_RESERVED_FOR_LVP   1
#define BOARD_RA6_RA7_USED_BY_XTAL   1

#if defined(BOARD_USE_RA6) || defined(BOARD_USE_RA7)
#  error "RA6/RA7 pinleri 20 MHz HS kristali (OSC1/OSC2) tarafindan kullaniliyor."
#endif

/* ---- Debug LED: RC0, aktif-HIGH, 470R seri direnc ---- */
#define DBG_LED_Init()        GPIO_SetOutput(C, 0)
#define DBG_LED_On()          GPIO_SetHigh(C, 0)
#define DBG_LED_Off()         GPIO_SetLow(C, 0)
#define DBG_LED_Toggle()      GPIO_Toggle(C, 0)

/* ---- Test butonu: RB0, dahili weak pull-up, aktif-LOW ---- */
#define DBG_BTN_Init()        do { GPIO_SetInput(B, 0);      \
                                   GPIO_PullupEnableB(0);    \
                                   GPIO_PortBPullupsEnable();\
                              } while (0)
#define DBG_BTN_IsPressed()   (GPIO_Read(B, 0) == 0U)

/* LVP/PGM korumasi */
#define BOARD_LVP_PGM_Guard() do { GPIO_SetInput(B, 3);   \
                                   GPIO_PullupDisableB(3);\
                              } while (0)

/* Kart pinlerinin toplu baslatilmasi (makro: kopya/ROM israfi yok) */
#define BOARD_PinsInitialize() do { DBG_LED_Init(); \
                                    DBG_LED_Off();  \
                                    DBG_BTN_Init(); \
                               } while (0)

/* =========================================================
 *  74HC595 seri kaydirma register'i (LCD 3-pin backend)
 *  DS   = RB1  (seri veri)
 *  SHCP = RB2  (kaydirma saati)
 *  STCP = RA0  (latch)
 *  Not: GPIO backend'inden gelen mevcut kablolama korunur,
 *       RA1..RA3 serbest kalir.
 * ========================================================= */
#define HC595_DS_Init()       GPIO_SetOutput(B, 1)
#define HC595_DS_SetHigh()    GPIO_SetHigh(B, 1)
#define HC595_DS_SetLow()     GPIO_SetLow(B, 1)

#define HC595_SHCP_Init()     GPIO_SetOutput(B, 2)
#define HC595_SHCP_SetHigh()  GPIO_SetHigh(B, 2)
#define HC595_SHCP_SetLow()   GPIO_SetLow(B, 2)

#define HC595_STCP_Init()     GPIO_SetOutput(A, 0)
#define HC595_STCP_SetHigh()  GPIO_SetHigh(A, 0)
#define HC595_STCP_SetLow()   GPIO_SetLow(A, 0)

#endif /* BOARD_PINS_H */