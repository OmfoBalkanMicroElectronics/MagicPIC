#ifndef GPIO_HAL_H
#define GPIO_HAL_H

#include <xc.h>
#include <stdint.h>

/*
 * PIC16F886'da LATx register'? YOKTUR (bu PIC16F1xxx ailesine özgüdür).
 * Bu yüzden ç?k?? yaz?m? PORTx üzerinden yap?l?r. Bit-field atamas? XC8
 * taraf?ndan tek komutluk BSF/BCF'ye derlenir -> read-modify-write riski
 * yoktur ve 0 byte RAM harcar.
 */

/* Yön kontrolü */
#define GPIO_SetOutput(P, B)     do { TRIS##P##bits.TRIS##P##B = 0U; } while (0)
#define GPIO_SetInput(P, B)      do { TRIS##P##bits.TRIS##P##B = 1U; } while (0)

/* Ç?k?? yazma */
#define GPIO_SetHigh(P, B)       do { PORT##P##bits.R##P##B = 1U; } while (0)
#define GPIO_SetLow(P, B)        do { PORT##P##bits.R##P##B = 0U; } while (0)
#define GPIO_Toggle(P, B)        do { PORT##P##bits.R##P##B ^= 1U; } while (0)
#define GPIO_Write(P, B, V)      do { PORT##P##bits.R##P##B = ((V) ? 1U : 0U); } while (0)

/* Okuma (uint8_t: 0 veya 1) */
#define GPIO_Read(P, B)          ((uint8_t)(PORT##P##bits.R##P##B))

/* Aç?k-drain benzeri hat sürümü (I2C için haz?r altyap?) */
#define GPIO_ReleaseOpenDrain(P, B)  do { TRIS##P##bits.TRIS##P##B = 1U; } while (0)
#define GPIO_DriveLowOpenDrain(P, B) do { PORT##P##bits.R##P##B = 0U; \
                                          TRIS##P##bits.TRIS##P##B = 0U; } while (0)

/* Weak pull-up SADECE PORTB'de mevcuttur (WPUB + OPTION_REG.nRBPU) */
#define GPIO_PortBPullupsEnable()    do { OPTION_REGbits.nRBPU = 0U; } while (0)
#define GPIO_PortBPullupsDisable()   do { OPTION_REGbits.nRBPU = 1U; } while (0)
#define GPIO_PullupEnableB(B)        do { WPUBbits.WPUB##B = 1U; } while (0)
#define GPIO_PullupDisableB(B)       do { WPUBbits.WPUB##B = 0U; } while (0)

#endif /* GPIO_HAL_H */