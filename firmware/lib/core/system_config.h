#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

#include <xc.h>
#include <stdint.h>

/* Harici HS kristal: 20 MHz  */
#define _XTAL_FREQ      20000000UL
#define SYS_FCY         (_XTAL_FREQ / 4UL)   /* 5 MHz komut çevrimi = 200 ns/komut */

#define SYS_GlobalInterruptEnable()   (INTCONbits.GIE = 1U)
#define SYS_GlobalInterruptDisable()  (INTCONbits.GIE = 0U)

/* Uzun beklemeler için: tek __delay_ms ça?r?s?n? büyütmek yerine döngü kullan */
void SYS_DelayMs(uint16_t ms);

void SYSTEM_Initialize(void);

#endif /* SYSTEM_CONFIG_H */