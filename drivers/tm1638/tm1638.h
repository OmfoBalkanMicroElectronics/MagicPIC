/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef TM1638_H
#define TM1638_H

#include <xc.h>
#include <stdint.h>
#include "system_config.h"
#include "mp_gpio.h"

#ifndef _XTAL_FREQ
#error "_XTAL_FREQ must be defined before including tm1638.h"
#endif

typedef struct
{
    mp_gpio_pin_t stb;
    mp_gpio_pin_t clk;
    mp_gpio_pin_t dio;
    uint8_t brightness;
    uint8_t ram[16];   /* TM1638 display/LED RAM mirror */
} tm1638_t;

/* Brightness: 0..7 */
#define TM1638_BRIGHTNESS_MIN  0u
#define TM1638_BRIGHTNESS_MED  4u
#define TM1638_BRIGHTNESS_MAX  7u

void TM1638_Init(tm1638_t *dev, uint8_t brightness);
void TM1638_Refresh(tm1638_t *dev);
void TM1638_ClearDisplay(tm1638_t *dev);
void TM1638_ClearLEDs(tm1638_t *dev);
void TM1638_AllLEDsOn(tm1638_t *dev);
void TM1638_SetBrightness(tm1638_t *dev, uint8_t brightness);

void TM1638_SetDigitRaw(tm1638_t *dev, uint8_t digit, uint8_t segments);
void TM1638_SetCharAt(tm1638_t *dev, uint8_t digit, char c);
void TM1638_SetText(tm1638_t *dev, const char *text);

void TM1638_SetNumber(tm1638_t *dev, int32_t value);
void TM1638_SetNumberZeroPad(tm1638_t *dev, uint32_t value);
void TM1638_SetHex(tm1638_t *dev, uint32_t value);

void TM1638_SetLED(tm1638_t *dev, uint8_t led_num, uint8_t state);
void TM1638_SetLEDs(tm1638_t *dev, uint8_t mask);

uint8_t TM1638_ReadButtons(tm1638_t *dev);

#endif /* TM1638_H */