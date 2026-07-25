/*
 * File:   tm1638.c
 * Author: cfylmz
 *
 * Created on 25 Temmuz 2026 Cumartesi, 11:42
 */


#include "tm1638.h"

/* Commands */
#define TM1638_CMD_DATA_AUTO   0x40u
#define TM1638_CMD_DATA_FIXED  0x44u
#define TM1638_CMD_DATA_READ   0x42u
#define TM1638_CMD_DISPLAY     0x80u
#define TM1638_CMD_ADDRESS     0xC0u

static void tm1638_delay(void)
{
    __delay_us(3);
}

static void tm1638_dio_output(tm1638_t *dev)
{
    MP_GPIO_InitOutput(&dev->dio);
}

static void tm1638_dio_input(tm1638_t *dev)
{
    MP_GPIO_InitInput(&dev->dio);
}

static void tm1638_write_byte(tm1638_t *dev, uint8_t data)
{
    uint8_t i;
    uint8_t value = data;

    tm1638_dio_output(dev);

    for (i = 0u; i < 8u; i++)
    {
        MP_GPIO_WriteLow(&dev->clk);
        tm1638_delay();

        MP_GPIO_Write(&dev->dio, (uint8_t)(value & 0x01u));
        tm1638_delay();

        MP_GPIO_WriteHigh(&dev->clk);
        tm1638_delay();

        value >>= 1;
    }
}

static uint8_t tm1638_read_byte(tm1638_t *dev)
{
    uint8_t i;
    uint8_t data = 0u;

    tm1638_dio_input(dev);

    for (i = 0u; i < 8u; i++)
    {
        MP_GPIO_WriteLow(&dev->clk);
        tm1638_delay();

        MP_GPIO_WriteHigh(&dev->clk);
        tm1638_delay();

        if (MP_GPIO_Read(&dev->dio))
        {
            data |= (uint8_t)(1u << i);
        }
    }

    return data;
}

static void tm1638_send_command(tm1638_t *dev, uint8_t cmd)
{
    MP_GPIO_WriteLow(&dev->stb);
    tm1638_delay();

    tm1638_write_byte(dev, cmd);

    MP_GPIO_WriteHigh(&dev->stb);
    tm1638_delay();
}

static uint8_t tm1638_encode_char(char c)
{
    switch (c)
    {
        /* Digits */
        case '0': return 0x3Fu;
        case '1': return 0x06u;
        case '2': return 0x5Bu;
        case '3': return 0x4Fu;
        case '4': return 0x66u;
        case '5': return 0x6Du;
        case '6': return 0x7Du;
        case '7': return 0x07u;
        case '8': return 0x7Fu;
        case '9': return 0x6Fu;

        /* Letters / approximations */
        case 'A': case 'a': return 0x77u;
        case 'B': case 'b': return 0x7Cu;
        case 'C': case 'c': return 0x39u;
        case 'D': case 'd': return 0x5Eu;
        case 'E': case 'e': return 0x79u;
        case 'F': case 'f': return 0x71u;
        case 'G': case 'g': return 0x3Du;
        case 'H': case 'h': return 0x76u;
        case 'I': case 'i': return 0x06u;
        case 'J': case 'j': return 0x1Eu;
        case 'K': case 'k': return 0x75u; /* approx */
        case 'L': case 'l': return 0x38u;
        case 'M': case 'm': return 0x37u; /* approx */
        case 'N': case 'n': return 0x54u; /* approx */
        case 'O': case 'o': return 0x3Fu;
        case 'P': case 'p': return 0x73u;
        case 'Q': case 'q': return 0x67u; /* approx */
        case 'R': case 'r': return 0x50u;
        case 'S': case 's': return 0x6Du;
        case 'T': case 't': return 0x78u;
        case 'U': case 'u': return 0x3Eu;
        case 'V': case 'v': return 0x3Eu; /* approx */
        case 'W': case 'w': return 0x2Au; /* approx */
        case 'X': case 'x': return 0x76u; /* approx */
        case 'Y': case 'y': return 0x6Eu;
        case 'Z': case 'z': return 0x5Bu;

        /* Symbols */
        case '-': return 0x40u;
        case '_': return 0x08u;
        case '=': return 0x48u;
        case ' ': return 0x00u;

        default:  return 0x00u;
    }
}

void TM1638_SetBrightness(tm1638_t *dev, uint8_t brightness)
{
    if (dev == 0)
    {
        return;
    }

    if (brightness > TM1638_BRIGHTNESS_MAX)
    {
        brightness = TM1638_BRIGHTNESS_MAX;
    }

    dev->brightness = brightness;
    tm1638_send_command(dev, (uint8_t)(TM1638_CMD_DISPLAY | 0x08u | brightness));
}

void TM1638_Refresh(tm1638_t *dev)
{
    uint8_t i;

    if (dev == 0)
    {
        return;
    }

    tm1638_send_command(dev, TM1638_CMD_DATA_AUTO);

    MP_GPIO_WriteLow(&dev->stb);
    tm1638_delay();

    tm1638_write_byte(dev, TM1638_CMD_ADDRESS);

    for (i = 0u; i < 16u; i++)
    {
        tm1638_write_byte(dev, dev->ram[i]);
    }

    MP_GPIO_WriteHigh(&dev->stb);
    tm1638_delay();
}

void TM1638_Init(tm1638_t *dev, uint8_t brightness)
{
    uint8_t i;

    if (dev == 0)
    {
        return;
    }

    MP_GPIO_InitOutput(&dev->stb);
    MP_GPIO_InitOutput(&dev->clk);
    MP_GPIO_InitOutput(&dev->dio);

    MP_GPIO_WriteHigh(&dev->stb);
    MP_GPIO_WriteHigh(&dev->clk);
    MP_GPIO_WriteHigh(&dev->dio);

    for (i = 0u; i < 16u; i++)
    {
        dev->ram[i] = 0x00u;
    }

    __delay_ms(2);

    TM1638_SetBrightness(dev, brightness);
    TM1638_Refresh(dev);
}

void TM1638_ClearDisplay(tm1638_t *dev)
{
    uint8_t i;

    if (dev == 0)
    {
        return;
    }

    for (i = 0u; i < 16u; i += 2u)
    {
        dev->ram[i] = 0x00u;
    }

    TM1638_Refresh(dev);
}

void TM1638_ClearLEDs(tm1638_t *dev)
{
    uint8_t i;

    if (dev == 0)
    {
        return;
    }

    for (i = 1u; i < 16u; i += 2u)
    {
        dev->ram[i] = 0x00u;
    }

    TM1638_Refresh(dev);
}

void TM1638_AllLEDsOn(tm1638_t *dev)
{
    uint8_t i;

    if (dev == 0)
    {
        return;
    }

    for (i = 1u; i < 16u; i += 2u)
    {
        dev->ram[i] = 0x01u;
    }

    TM1638_Refresh(dev);
}

void TM1638_SetDigitRaw(tm1638_t *dev, uint8_t digit, uint8_t segments)
{
    if (dev == 0)
    {
        return;
    }

    if (digit > 7u)
    {
        return;
    }

    dev->ram[digit * 2u] = segments;
    TM1638_Refresh(dev);
}

void TM1638_SetCharAt(tm1638_t *dev, uint8_t digit, char c)
{
    if (dev == 0)
    {
        return;
    }

    if (digit > 7u)
    {
        return;
    }

    dev->ram[digit * 2u] = tm1638_encode_char(c);
    TM1638_Refresh(dev);
}

void TM1638_SetText(tm1638_t *dev, const char *text)
{
    uint8_t i;
    char c;

    if (dev == 0)
    {
        return;
    }

    for (i = 0u; i < 8u; i++)
    {
        c = ' ';

        if ((text != 0) && (text[i] != '\0'))
        {
            c = text[i];
        }

        dev->ram[i * 2u] = tm1638_encode_char(c);
    }

    TM1638_Refresh(dev);
}

void TM1638_SetNumber(tm1638_t *dev, int32_t value)
{
    char out[9];
    uint8_t pos = 8u;
    uint8_t negative = 0u;
    uint32_t mag;

    if (dev == 0)
    {
        return;
    }

    out[8] = '\0';

    for (pos = 0u; pos < 8u; pos++)
    {
        out[pos] = ' ';
    }

    if (value == 0)
    {
        out[7] = '0';
        TM1638_SetText(dev, out);
        return;
    }

    if (value < 0)
    {
        negative = 1u;
        mag = (uint32_t)(-(value + 1)) + 1u;
    }
    else
    {
        mag = (uint32_t)value;
    }

    pos = 8u;

    while ((mag > 0u) && (pos > 0u))
    {
        pos--;
        out[pos] = (char)('0' + (mag % 10u));
        mag /= 10u;
    }

    if ((negative != 0u) && (pos > 0u))
    {
        pos--;
        out[pos] = '-';
    }

    TM1638_SetText(dev, out);
}

void TM1638_SetNumberZeroPad(tm1638_t *dev, uint32_t value)
{
    char out[9];
    int8_t i;

    if (dev == 0)
    {
        return;
    }

    for (i = 7; i >= 0; i--)
    {
        out[i] = (char)('0' + (value % 10u));
        value /= 10u;
    }

    out[8] = '\0';
    TM1638_SetText(dev, out);
}

void TM1638_SetHex(tm1638_t *dev, uint32_t value)
{
    char out[9];
    int8_t i;
    uint8_t nibble;

    if (dev == 0)
    {
        return;
    }

    for (i = 7; i >= 0; i--)
    {
        nibble = (uint8_t)(value & 0x0Fu);

        if (nibble < 10u)
        {
            out[i] = (char)('0' + nibble);
        }
        else
        {
            out[i] = (char)('A' + (nibble - 10u));
        }

        value >>= 4;
    }

    out[8] = '\0';
    TM1638_SetText(dev, out);
}

void TM1638_SetLED(tm1638_t *dev, uint8_t led_num, uint8_t state)
{
    if (dev == 0)
    {
        return;
    }

    if (led_num > 7u)
    {
        return;
    }

    dev->ram[(led_num * 2u) + 1u] = state ? 0x01u : 0x00u;
    TM1638_Refresh(dev);
}

void TM1638_SetLEDs(tm1638_t *dev, uint8_t mask)
{
    uint8_t i;

    if (dev == 0)
    {
        return;
    }

    for (i = 0u; i < 8u; i++)
    {
        dev->ram[(i * 2u) + 1u] = ((mask >> i) & 0x01u) ? 0x01u : 0x00u;
    }

    TM1638_Refresh(dev);
}

uint8_t TM1638_ReadButtons(tm1638_t *dev)
{
    uint8_t i;
    uint8_t value;
    uint8_t buttons = 0u;

    if (dev == 0)
    {
        return 0u;
    }

    MP_GPIO_WriteLow(&dev->stb);
    tm1638_delay();

    tm1638_write_byte(dev, TM1638_CMD_DATA_READ);

    for (i = 0u; i < 4u; i++)
    {
        value = tm1638_read_byte(dev);

        if (value & 0x01u)
        {
            buttons |= (uint8_t)(1u << i);
        }

        if (value & 0x10u)
        {
            buttons |= (uint8_t)(1u << (i + 4u));
        }
    }

    MP_GPIO_WriteHigh(&dev->stb);
    tm1638_delay();

    tm1638_dio_output(dev);
    MP_GPIO_WriteHigh(&dev->dio);

    return buttons;
}