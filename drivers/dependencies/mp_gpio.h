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

 
#ifndef MP_GPIO_H
#define MP_GPIO_H

#include <xc.h>
#include <stdint.h>

typedef struct
{
    volatile unsigned char *port;
    volatile unsigned char *tris;
    volatile unsigned char *analog_reg;   // Yoksa 0 verilir
    uint8_t port_mask;
    uint8_t analog_mask;
} mp_gpio_pin_t;

void MP_GPIO_InitOutput(const mp_gpio_pin_t *pin);
void MP_GPIO_InitInput(const mp_gpio_pin_t *pin);
void MP_GPIO_WriteHigh(const mp_gpio_pin_t *pin);
void MP_GPIO_WriteLow(const mp_gpio_pin_t *pin);
void MP_GPIO_Write(const mp_gpio_pin_t *pin, uint8_t state);
void MP_GPIO_Toggle(const mp_gpio_pin_t *pin);
uint8_t MP_GPIO_Read(const mp_gpio_pin_t *pin);

#endif