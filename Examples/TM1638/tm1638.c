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
 * File:   tm1638.c
 * Author: Barbaror4 / OBM
 * Comments: MagicPIC TM1638 LED & KEY Sürücü Kütüphanesi Gövde Dosyas?
 * Revision history: 1.0
 */

#include "tm1638.h"

/* 
   7 Segment Karakter Sözlü?ü (Ortak Katot)
   Dizinler: 
   0-8  : '0' ile '8' aras? rakamlar
   9    : 'E' (Error/Set için)
   10   : 'r' 
   11   : 'o'
   12   : 'P' (Open/Program için)
   13   : 'n'
   14   : 'S'
   15   : 'F'
   16   : Bo?luk (Ekran karartma)
*/
const char seg_font[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, // 0-8
    0x79, 0x50, 0x5C, 0x73, 0x37, 0x6D, 0x71, 0x00        // E, r, o, P, n, S, F, Bo?
};

void TM1638_WriteByte(char data) {
    TRISA4 = 0; // RA4 (TM_DIO) pini ç?k?? moduna al?n?r
    for(char i = 0; i < 8; i++) {
        TM_CLK = 0;
        TM_DIO = (data & 0x01) ? 1 : 0; // En önemsiz bit (LSB) önce iletilir
        data >>= 1;   
        __delay_us(1); // Sinyal oturma süresi
        TM_CLK = 1;   
        __delay_us(1);
    }
}

char TM1638_ReadByte(void) {
    char data = 0;
    TRISA4 = 1; // RA4 (TM_DIO) pini okuma için giri? moduna al?n?r
    for(char i = 0; i < 8; i++) {
        TM_CLK = 0;   __delay_us(1);
        TM_CLK = 1;   __delay_us(1);
        if(TM_DIO) {
            data |= (1 << i); // Al?nan bit verisi saklan?r
        }
    }
    return data;
}

void TM1638_SendCommand(char cmd) {
    TM_STB = 0;
    TM1638_WriteByte(cmd);
    TM_STB = 1;
}

void TM1638_Init(char brightness) {
    TM_STB = 1; 
    TM_CLK = 1;
    
    // Geçersiz parlakl?k s?n?rland?rmas? (Maksimum 0x07 olabilir)
    if(brightness > 0x07) {
        brightness = TM_BRIGHTNESS_MAX;
    }
    
    TM1638_SendCommand(TM_CMD_DATA); // Otomatik adres art?rmal? yazma modu [0x40]
    TM1638_SendCommand(TM_CMD_DISPLAY | brightness | 0x08); // Ekran aktif ve parlakl?k ayar?
    
    TM1638_ClearDisplay(); // Ba?lang?çta tüm haf?zay? temizle
}

void TM1638_ClearDisplay(void) {
    TM_STB = 0;
    TM1638_WriteByte(TM_CMD_ADDRESS); // Ba?lang?ç adresi (0xC0)
    for(char i = 0; i < 16; i++) {
        TM1638_WriteByte(0x00); // 16 byte'l?k display ve LED RAM alan?n? s?f?rla
    }
    TM_STB = 1;
}

void TM1638_SetDigit(char digit, char font_idx) {
    if(digit > 7) return; // Hane s?n?r kontrolü
    
    TM1638_SendCommand(TM_CMD_DATA);
    TM_STB = 0;
    // 7-Segment adresleri çift say?lardad?r: 0xC0, 0xC2, 0xC4...
    TM1638_WriteByte(TM_CMD_ADDRESS + (digit * 2));
    TM1638_WriteByte(seg_font[font_idx]);
    TM_STB = 1;
}

void TM1638_SetLED(char led_num, char state) {
    if(led_num > 7) return; // LED s?n?r kontrolü
    
    TM1638_SendCommand(TM_CMD_DATA);
    TM_STB = 0;
    // LED adresleri tek say?lardad?r: 0xC1, 0xC3, 0xC5...
    TM1638_WriteByte(TM_CMD_ADDRESS + (led_num * 2) + 1);
    TM1638_WriteByte(state ? 1 : 0);
    TM_STB = 1;
}

void TM1638_SetLEDs(char mask) {
    TM1638_SendCommand(TM_CMD_DATA);
    for(char i = 0; i < 8; i++) {
        TM_STB = 0;
        TM1638_WriteByte(TM_CMD_ADDRESS + (i * 2) + 1);
        TM1638_WriteByte((mask & (1 << i)) ? 1 : 0);
        TM_STB = 1;
    }
}

char TM1638_ReadButtons(void) {
    char buttons = 0;
    TM_STB = 0;
    TM1638_WriteByte(0x42); // Buton okuma komutu [0x42]
    
    for(char i = 0; i < 4; i++) {
        char byte = TM1638_ReadByte();
        // TM1638 tarama matrisindeki S1, S3, S5, S7 butonlar?n?n kontrolü
        if(byte & 0x01) buttons |= (1 << i);       
        // Tarama matrisindeki S2, S4, S6, S8 butonlar?n?n kontrolü
        if(byte & 0x10) buttons |= (1 << (i + 4)); 
    }
    TM_STB = 1;
    return buttons;
}
