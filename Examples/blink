/*
 * ============================================================================
 * Project Name: MagicPIC
 * File:         main.c
 * Author:       BarbaroR4(Barbaros A.Y., OBM)
 * Date:         2026
 * 
 * Target:       Microchip PIC16F886
 * Environment:  MPLAB X IDE
 * Compiler:     XC8 (Free/Standard Mode)
 * Repository:   https://github.com/OmfoBalkanMicroElectronics/MagicPIC
 *
 * Description: 
 * Basic hardware initialization and test program for the MagicPIC project.
 * ============================================================================
 * 
 * MIT License
 * 
 * Copyright (c) 2026 OmfoBalkanMicroElectronics
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * ============================================================================
 */






// --- Configuration Bits ---
// Instead of an external crystal, we tell it to use the internal one
#pragma config FOSC = INTRC_NOCLKOUT 
#pragma config WDTE = OFF            // Disable Watchdog Timer
#pragma config PWRTE = OFF           // Disable Power-up Timer
#pragma config BOREN = OFF           // Disable Brown-out Reset
#pragma config LVP = OFF             // Disable Low-Voltage Programming

// Tell the compiler our clock speed so __delay_ms() works correctly
#define _XTAL_FREQ 4000000           // We will configure the chip for 4MHz

#include <xc.h>

void main(void) {
    // 1. Set the Internal Clock Speed
    // The F886 boots at 4MHz by default, but it's good practice to set it explicitly.
    OSCCONbits.IRCF = 0b110; 

 
    // The F886 has analog pins and they are turned on by default. You MUST clear them to use them as standard digital pins.

    ANSEL = 0x00;  
    ANSELH = 0x00; 

    // 3. Set Pin Direction
    //  (0 = Output)
    TRISBbits.TRISB0 = 0; // Set RB0 as an output

    // 4. Set Initial Pin State
    PORTBbits.RB0 = 0;    // Turn the LED off to start

    // 5. Infinite Loop
    while(1) {
        PORTBbits.RB0 = 1; // Drive pin high (5V)
        __delay_ms(500);   // Wait half a second
        PORTBbits.RB0 = 0; // Drive pin low (0V)
        __delay_ms(500);   // Wait half a second
    }
}

//There you have it mate,blinky blinky fills my tummy
