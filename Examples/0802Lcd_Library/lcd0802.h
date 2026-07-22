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

/* 
 * File:   lcd0802.h
 * Author: Barbaror4 / OBM
 * Comments: MagicPIC HD44780 0802A LCD Hafif Surucu Kutuphanesi
 * Revision history: 1.0
 */

#ifndef LCD0802_H
#define	LCD0802_H

#include <xc.h> 

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif

/* LCD Donanim Pin Tanimlamalari */
#define RS RA0
#define EN RA1
#define D4 RB0  
#define D5 RB1
#define D6 RB2
#define D7 RB4

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

/**
    <p><b>Function prototype: void Lcd_Port(char a);</b></p>
    <p><b>Summary:</b> LCD veri hatlarina 4-bitlik veri gonderir.</p>
 */
void Lcd_Port(char a);

/**
    <p><b>Function prototype: void Lcd_Cmd(char a);</b></p>
    <p><b>Summary:</b> LCD kontrolcusune komut gonderir.</p>
 */
void Lcd_Cmd(char a);

/**
    <p><b>Function prototype: void Lcd_Clear(void);</b></p>
    <p><b>Summary:</b> LCD ekranini ve DDRAM hafizasini temizler.</p>
 */
void Lcd_Clear(void);

/**
    <p><b>Function prototype: void Lcd_Init(void);</b></p>
    <p><b>Summary:</b> LCD'yi 4-bit modunda baslatir ve ayarlarini yapar.</p>
 */
void Lcd_Init(void);

/**
    <p><b>Function prototype: void Lcd_Write_Char(char a);</b></p>
    <p><b>Summary:</b> LCD ekranina tek bir ASCII karakteri basar.</p>
 */
void Lcd_Write_Char(char a);

/**
    <p><b>Function prototype: void Lcd_Write_String(const char *str);</b></p>
    <p><b>Summary:</b> LCD ekranina bir metin dizisi basar.</p>
 */
void Lcd_Write_String(const char *str);

/**
    <p><b>Function prototype: void Lcd_Set_Cursor(char row, char col);</b></p>
    <p><b>Summary:</b> LCD imlecini belirtilen satir ve sutuna konumlandirir.</p>
 */
void Lcd_Set_Cursor(char row, char col);

/**
    <p><b>Function prototype: void Lcd_Scroll_Text(char row, const char *text, int speed_ms);</b></p>
    <p><b>Summary:</b> Belirtilen satirda dinamik kayan yazi efekti olusturur.</p>
 */
void Lcd_Scroll_Text(char row, const char *text, int speed_ms);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* LCD0802_H */
