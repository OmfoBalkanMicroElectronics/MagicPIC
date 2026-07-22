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
 * File:   tm1638.h
 * Author: Barbaror4 / OBM
 * Comments: MagicPIC TM1638 LED & KEY Sürücü Kütüphanesi
 * Revision history: 1.0
 */

#ifndef TM1638_H
#define	TM1638_H

#include <xc.h> 

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif

/* TM1638 Donan?m Pin Tan?mlamalar? */
#define TM_STB RA2
#define TM_CLK RA3
#define TM_DIO RA4

/* TM1638 Komut Yap?s? */
#define TM_CMD_DATA      0x40  // Veri komut ayar?
#define TM_CMD_DISPLAY   0x80  // Ekran ve parlakl?k komut ayar?
#define TM_CMD_ADDRESS   0xC0  // Adres komut ayar?

/* Parlakl?k Seviye Makrolar? (0-7 Aras?) */
#define TM_BRIGHTNESS_MIN  0x00
#define TM_BRIGHTNESS_MED  0x04
#define TM_BRIGHTNESS_MAX  0x07

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

/**
    <p><b>Function prototype: void TM1638_WriteByte(char data);</b></p>
    <p><b>Summary:</b> TM1638 entegresine seri olarak 1 byte veri gönderir.</p>
 */
void TM1638_WriteByte(char data);

/**
    <p><b>Function prototype: char TM1638_ReadByte(void);</b></p>
    <p><b>Summary:</b> TM1638 entegresinden seri olarak 1 byte veri okur.</p>
 */
char TM1638_ReadByte(void);

/**
    <p><b>Function prototype: void TM1638_SendCommand(char cmd);</b></p>
    <p><b>Summary:</b> Entegreye donan?msal strobe (STB) e?li?inde kontrol komutu iletir.</p>
 */
void TM1638_SendCommand(char cmd);

/**
    <p><b>Function prototype: void TM1638_Init(char brightness);</b></p>
    <p><b>Summary:</b> Sürücüyü ba?lat?r, haf?zay? temizler ve parlakl?k seviyesini belirler.</p>
 */
void TM1638_Init(char brightness);

/**
    <p><b>Function prototype: void TM1638_SetDigit(char digit, char font_idx);</b></p>
    <p><b>Summary:</b> Belirtilen 7-segment hanesine (0-7) font sözlü?ünden karakter basar.</p>
 */
void TM1638_SetDigit(char digit, char font_idx);

/**
    <p><b>Function prototype: void TM1638_SetLED(char led_num, char state);</b></p>
    <p><b>Summary:</b> Belirtilen ba??ms?z LED'i (0-7) aktif veya pasif yapar.</p>
 */
void TM1638_SetLED(char led_num, char state);

/**
    <p><b>Function prototype: void TM1638_SetLEDs(char mask);</b></p>
    <p><b>Summary:</b> 8 bitlik maske de?erine göre tüm LED grubunu tek seferde günceller.</p>
 */
void TM1638_SetLEDs(char mask);

/**
    <p><b>Function prototype: char TM1638_ReadButtons(void);</b></p>
    <p><b>Summary:</b> Kart üzerindeki 8 adet butonun anl?k lojik durum matrisini okur.</p>
 */
char TM1638_ReadButtons(void);

/**
    <p><b>Function prototype: void TM1638_ClearDisplay(void);</b></p>
    <p><b>Summary:</b> 7-segment ekranlar? ve tüm LED'leri kapatarak göstergeyi temizler.</p>
 */
void TM1638_ClearDisplay(void);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* TM1638_H */
