/*
 * File:   mp_gpio.c
 * Author: cfylmz
 *
 * Created on 25 Temmuz 2026 Cumartesi, 11:02
 */


#include "mp_gpio.h"

static void MP_GPIO_DisableAnalog(const mp_gpio_pin_t *pin)
{
    if ((pin->analog_reg != 0) && (pin->analog_mask != 0))
    {
        *(pin->analog_reg) &= (uint8_t)(~pin->analog_mask);
    }
}

void MP_GPIO_InitOutput(const mp_gpio_pin_t *pin)
{
    MP_GPIO_DisableAnalog(pin);
    *(pin->tris) &= (uint8_t)(~pin->port_mask);
}

void MP_GPIO_InitInput(const mp_gpio_pin_t *pin)
{
    MP_GPIO_DisableAnalog(pin);
    *(pin->tris) |= pin->port_mask;
}

void MP_GPIO_WriteHigh(const mp_gpio_pin_t *pin)
{
    *(pin->port) |= pin->port_mask;
}

void MP_GPIO_WriteLow(const mp_gpio_pin_t *pin)
{
    *(pin->port) &= (uint8_t)(~pin->port_mask);
}

void MP_GPIO_Write(const mp_gpio_pin_t *pin, uint8_t state)
{
    if (state)
    {
        MP_GPIO_WriteHigh(pin);
    }
    else
    {
        MP_GPIO_WriteLow(pin);
    }
}

void MP_GPIO_Toggle(const mp_gpio_pin_t *pin)
{
    *(pin->port) ^= pin->port_mask;
}

uint8_t MP_GPIO_Read(const mp_gpio_pin_t *pin)
{
    return ((*(pin->port) & pin->port_mask) != 0u) ? 1u : 0u;
}