/*
 * File:   lcd0802.c
 * Author: cfylmz
 *
 * Created on 25 Temmuz 2026 Cumartesi, 11:13
 */


#include "lcd0802.h"
#include "system_config.h"

/* =========================
   Internal helper functions
   ========================= */

static void lcd0802_pulse_enable(lcd0802_t *lcd)
{
    MP_GPIO_WriteLow(&lcd->en);
    __delay_us(2);

    MP_GPIO_WriteHigh(&lcd->en);
    __delay_us(2);

    MP_GPIO_WriteLow(&lcd->en);
    __delay_us(50);
}

static void lcd0802_write_nibble(lcd0802_t *lcd, uint8_t nibble)
{
    MP_GPIO_Write(&lcd->d4, (nibble >> 0) & 0x01u);
    MP_GPIO_Write(&lcd->d5, (nibble >> 1) & 0x01u);
    MP_GPIO_Write(&lcd->d6, (nibble >> 2) & 0x01u);
    MP_GPIO_Write(&lcd->d7, (nibble >> 3) & 0x01u);

    lcd0802_pulse_enable(lcd);
}

static void lcd0802_write_byte(lcd0802_t *lcd, uint8_t value, uint8_t rs)
{
    MP_GPIO_Write(&lcd->rs, rs);

    lcd0802_write_nibble(lcd, (uint8_t)((value >> 4) & 0x0Fu));
    lcd0802_write_nibble(lcd, (uint8_t)(value & 0x0Fu));
}

static void lcd0802_write_cmd(lcd0802_t *lcd, uint8_t cmd)
{
    lcd0802_write_byte(lcd, cmd, 0u);

    /* Clear display ve Return home daha uzun sürer */
    if ((cmd == 0x01u) || (cmd == 0x02u))
    {
        __delay_ms(2);
    }
    else
    {
        __delay_us(50);
    }
}

static void lcd0802_write_data(lcd0802_t *lcd, uint8_t data)
{
    lcd0802_write_byte(lcd, data, 1u);
    __delay_us(50);
}

static uint8_t lcd0802_get_ddram_addr(const lcd0802_t *lcd, uint8_t row, uint8_t col)
{
    /* 2 sat?rl? klasik HD44780 adresleme */
    (void)lcd;

    if (row == 0u)
    {
        return (uint8_t)(0x00u + col);
    }
    else
    {
        return (uint8_t)(0x40u + col);
    }
}

static uint8_t lcd0802_strlen(const char *str)
{
    uint8_t len = 0u;

    while (str[len] != '\0')
    {
        len++;
    }

    return len;
}

/* =========================
   Public API
   ========================= */

void LCD0802_Init(lcd0802_t *lcd)
{
    if (lcd == 0)
    {
        return;
    }

    /* Pin yönleri */
    MP_GPIO_InitOutput(&lcd->rs);
    MP_GPIO_InitOutput(&lcd->en);
    MP_GPIO_InitOutput(&lcd->d4);
    MP_GPIO_InitOutput(&lcd->d5);
    MP_GPIO_InitOutput(&lcd->d6);
    MP_GPIO_InitOutput(&lcd->d7);

    /* Güvenli ba?lang?ç seviyesi */
    MP_GPIO_WriteLow(&lcd->rs);
    MP_GPIO_WriteLow(&lcd->en);
    MP_GPIO_WriteLow(&lcd->d4);
    MP_GPIO_WriteLow(&lcd->d5);
    MP_GPIO_WriteLow(&lcd->d6);
    MP_GPIO_WriteLow(&lcd->d7);

    __delay_ms(40);

    /* 4-bit init sequence */
    lcd0802_write_nibble(lcd, 0x03u);
    __delay_ms(5);

    lcd0802_write_nibble(lcd, 0x03u);
    __delay_us(150);

    lcd0802_write_nibble(lcd, 0x03u);
    __delay_us(150);

    lcd0802_write_nibble(lcd, 0x02u);
    __delay_us(150);

    /* Function Set: 4-bit, 2-line, 5x8 font */
    lcd0802_write_cmd(lcd, 0x28u);

    /* Display OFF */
    lcd0802_write_cmd(lcd, 0x08u);

    /* Clear */
    lcd0802_write_cmd(lcd, 0x01u);

    /* Entry mode: increment, no shift */
    lcd0802_write_cmd(lcd, 0x06u);

    /* Display ON, cursor OFF, blink OFF */
    lcd0802_write_cmd(lcd, 0x0Cu);
}

void LCD0802_Clear(lcd0802_t *lcd)
{
    if (lcd == 0)
    {
        return;
    }

    lcd0802_write_cmd(lcd, 0x01u);
}

void LCD0802_Home(lcd0802_t *lcd)
{
    if (lcd == 0)
    {
        return;
    }

    lcd0802_write_cmd(lcd, 0x02u);
}

void LCD0802_SetCursor(lcd0802_t *lcd, uint8_t row, uint8_t col)
{
    uint8_t addr;

    if (lcd == 0)
    {
        return;
    }

    if (row >= lcd->rows)
    {
        return;
    }

    if (col >= lcd->cols)
    {
        return;
    }

    addr = lcd0802_get_ddram_addr(lcd, row, col);
    lcd0802_write_cmd(lcd, (uint8_t)(0x80u | addr));
}

void LCD0802_WriteChar(lcd0802_t *lcd, char c)
{
    if (lcd == 0)
    {
        return;
    }

    lcd0802_write_data(lcd, (uint8_t)c);
}

void LCD0802_WriteString(lcd0802_t *lcd, const char *str)
{
    if ((lcd == 0) || (str == 0))
    {
        return;
    }

    while (*str != '\0')
    {
        LCD0802_WriteChar(lcd, *str);
        str++;
    }
}

void LCD0802_WriteStringAt(lcd0802_t *lcd, uint8_t row, uint8_t col, const char *str)
{
    if ((lcd == 0) || (str == 0))
    {
        return;
    }

    LCD0802_SetCursor(lcd, row, col);
    LCD0802_WriteString(lcd, str);
}

void LCD0802_ScrollText(lcd0802_t *lcd, uint8_t row, const char *text, uint16_t speed_ms)
{
    uint8_t len;
    uint8_t window;
    uint16_t start;
    uint16_t end;
    uint16_t pos;
    uint8_t j;
    uint16_t delay_copy;

    if ((lcd == 0) || (text == 0))
    {
        return;
    }

    if (row >= lcd->rows)
    {
        return;
    }

    window = lcd->cols;
    len = lcd0802_strlen(text);

    /* Metin ekrana s???yorsa direkt yaz ve sat?r? bo?lukla doldur */
    if (len <= window)
    {
        LCD0802_SetCursor(lcd, row, 0u);

        for (j = 0u; j < window; j++)
        {
            if (j < len)
            {
                LCD0802_WriteChar(lcd, text[j]);
            }
            else
            {
                LCD0802_WriteChar(lcd, ' ');
            }
        }
        return;
    }

    /* Soldan bo?lukla giri?, sa?dan bo?lukla ç?k?? */
    end = (uint16_t)(len + window);

    for (start = 0u; start < end; start++)
    {
        LCD0802_SetCursor(lcd, row, 0u);

        for (j = 0u; j < window; j++)
        {
            pos = (uint16_t)(start + j);

            if (pos < window)
            {
                LCD0802_WriteChar(lcd, ' ');
            }
            else if ((pos - window) < len)
            {
                LCD0802_WriteChar(lcd, text[pos - window]);
            }
            else
            {
                LCD0802_WriteChar(lcd, ' ');
            }
        }

        delay_copy = speed_ms;

        while (delay_copy >= 10u)
        {
            __delay_ms(10);
            delay_copy -= 10u;
        }

        while (delay_copy > 0u)
        {
            __delay_ms(1);
            delay_copy--;
        }
    }
}