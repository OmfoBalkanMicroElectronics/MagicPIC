#ifndef LCD0802_H
#define LCD0802_H

#include <xc.h>
#include <stdint.h>
#include "system_config.h"
#include "mp_gpio.h"

#ifndef _XTAL_FREQ
#error "_XTAL_FREQ must be defined before including lcd0802.h"
#endif

typedef struct
{
    mp_gpio_pin_t rs;
    mp_gpio_pin_t en;
    mp_gpio_pin_t d4;
    mp_gpio_pin_t d5;
    mp_gpio_pin_t d6;
    mp_gpio_pin_t d7;
    uint8_t cols;
    uint8_t rows;
} lcd0802_t;

void LCD0802_Init(lcd0802_t *lcd);
void LCD0802_Clear(lcd0802_t *lcd);
void LCD0802_Home(lcd0802_t *lcd);
void LCD0802_SetCursor(lcd0802_t *lcd, uint8_t row, uint8_t col);
void LCD0802_WriteChar(lcd0802_t *lcd, char c);
void LCD0802_WriteString(lcd0802_t *lcd, const char *str);
void LCD0802_WriteStringAt(lcd0802_t *lcd, uint8_t row, uint8_t col, const char *str);
void LCD0802_ScrollText(lcd0802_t *lcd, uint8_t row, const char *text, uint16_t speed_ms);

#endif