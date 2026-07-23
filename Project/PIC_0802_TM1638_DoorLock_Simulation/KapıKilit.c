#include <xc.h>
#include "lcd0802.h"
#include "tm1638.h"

#define _XTAL_FREQ 20000000 

// ==========================================
// CONFIGURATION BITS
// ==========================================
#pragma config FOSC = HS, WDTE = OFF, PWRTE = OFF, MCLRE = ON
#pragma config CP = OFF, CPD = OFF, BOREN = OFF, IESO = OFF, FCMEN = OFF
#pragma config LVP = ON, WRT = OFF        

#define KAPI_ROLE RC0

void main(void) {
    OSCCON = 0x00; 
    ANSEL = 0x00; 
    ANSELH = 0x00;
    
    // Donanimsal Pin Yönlendirmeleri
    TRISA0 = 0; TRISA1 = 0;     // LCD Kontrol
    TRISB0 = 0; TRISB1 = 0;     // LCD Veri
    TRISB2 = 0; TRISB4 = 0;     // LCD Veri (RB3/LVP Muaf)
    TRISA2 = 0; TRISA3 = 0;     // TM1638 STB ve CLK
    TRISC0 = 0; KAPI_ROLE = 0;  // Kap? Rolesi

    // Donan?m baslat?lmas?
    Lcd_Init();
    TM1638_Init(TM_BRIGHTNESS_MAX);

    char girilen_sifre[8];
    char dogru_sifre[8] = {1, 2, 3, 4, 5, 6, 7, 8}; // Varsay?lan Sifre
    int tus_sayaci = 0;
    int uzun_basi_sayac = 0;

    while(1) {
        Lcd_Clear();
        Lcd_Set_Cursor(1, 1); Lcd_Write_String("GIRIS:");   
        Lcd_Set_Cursor(2, 1);         
        
        TM1638_ClearDisplay(); 
        tus_sayaci = 0;

        //sifre giris izleme dongusu
        while(tus_sayaci < 8) {       
            char key_status = TM1638_ReadButtons();
            char basilan_tus = 0;

            // sifre degistirme dongusu
            if(tus_sayaci == 0 && (key_status & 0x80)) {
                uzun_basi_sayac = 0;
                while(TM1638_ReadButtons() & 0x80) {
                    uzun_basi_sayac++;
                    __delay_ms(10);
                    if(uzun_basi_sayac > 200) { // s8 2sn bas?l? tutulursa 
                        Lcd_Clear();
                        Lcd_Set_Cursor(1, 1); Lcd_Write_String("YENI SIF");
                        Lcd_Set_Cursor(2, 1);
                        
                        TM1638_ClearDisplay();
                        TM1638_SetDigit(0, 14); // 'S'
                        TM1638_SetDigit(1, 9);  // 'E'
                        TM1638_SetDigit(2, 10); // 'r' -> Gostergede 't' algisi icin
                        TM1638_SetLEDs(0xFF); // Ayar modu gostergesi LED'ler acik
                        
                        while(TM1638_ReadButtons() != 0);
                        __delay_ms(200);

                        int ayar_sayac = 0;
                        while(ayar_sayac < 8) {
                            char ayar_key = TM1638_ReadButtons();
                            char yeni_tus = 0;

                            if(ayar_key & 0x01) yeni_tus = 1;
                            else if(ayar_key & 0x02) yeni_tus = 2;
                            else if(ayar_key & 0x04) yeni_tus = 3;
                            else if(ayar_key & 0x08) yeni_tus = 4;
                            else if(ayar_key & 0x10) yeni_tus = 5;
                            else if(ayar_key & 0x20) yeni_tus = 6;
                            else if(ayar_key & 0x40) yeni_tus = 7;
                            else if(ayar_key & 0x80) yeni_tus = 8;

                            if(yeni_tus > 0) {
                                dogru_sifre[ayar_sayac] = yeni_tus;
                                Lcd_Write_Char('*');
                                TM1638_SetDigit(ayar_sayac, yeni_tus);
                                ayar_sayac++;
                                while(TM1638_ReadButtons() != 0);
                                __delay_ms(50);
                            }
                        }
                        
                        Lcd_Clear();
                        Lcd_Set_Cursor(1, 1); Lcd_Write_String("KAYDEDIL");
                        Lcd_Set_Cursor(2, 1); Lcd_Write_String("DI...");
                        
                        TM1638_ClearDisplay();
                        TM1638_SetLEDs(0xAA); __delay_ms(500);
                        TM1638_SetLEDs(0x55); __delay_ms(500);
                        TM1638_SetLEDs(0x00);
                        __delay_ms(1000);
                        
                        goto KILIT_SIFIRLA; 
                    }
                }
            }

            // Normal mod,matris button cozumleme
            if(key_status & 0x01) basilan_tus = 1;      
            else if(key_status & 0x02) basilan_tus = 2; 
            else if(key_status & 0x04) basilan_tus = 3; 
            else if(key_status & 0x08) basilan_tus = 4; 
            else if(key_status & 0x10) basilan_tus = 5; 
            else if(key_status & 0x20) basilan_tus = 6; 
            else if(key_status & 0x40) basilan_tus = 7; 
            else if(key_status & 0x80) basilan_tus = 8; 

            if(basilan_tus > 0) {
                girilen_sifre[tus_sayaci] = basilan_tus;
                Lcd_Write_Char('*');
                TM1638_SetDigit(tus_sayaci, basilan_tus);
                tus_sayaci++;
                while(TM1638_ReadButtons() != 0); 
                __delay_ms(50);
            }
        }

        __delay_ms(600); 
        Lcd_Clear();

        // Dogrulama algoritmasi
        int sifre_dogru_mu = 1;
        for(int k = 0; k < 8; k++) {
            if(girilen_sifre[k] != dogru_sifre[k]) {
                sifre_dogru_mu = 0;
                break;
            }
        }

        if(sifre_dogru_mu == 1) {
            Lcd_Set_Cursor(1, 1); Lcd_Write_String("KAPI ACC");
            Lcd_Set_Cursor(2, 1); Lcd_Write_String("MagicPIC");
            
            TM1638_ClearDisplay();
            TM1638_SetDigit(0, 11); TM1638_SetDigit(1, 12); // 'O', 'P'
            TM1638_SetDigit(2, 9);  TM1638_SetDigit(3, 13); // 'E', 'n'
            
            KAPI_ROLE = 1; 
            for(int f = 0; f < 3; f++) {
                TM1638_SetLEDs(0xFF); __delay_ms(300);
                TM1638_SetLEDs(0x00); __delay_ms(300);
            }
            __delay_ms(1000);
            KAPI_ROLE = 0; 
            
        } else {
            Lcd_Set_Cursor(1, 1); Lcd_Write_String("HATALI!");
            Lcd_Set_Cursor(2, 1); Lcd_Write_String("BLOKE..");
            
            TM1638_ClearDisplay();
            TM1638_SetDigit(0, 9); TM1638_SetDigit(1, 10); TM1638_SetDigit(2, 10); // "Err"
            
            for(int r = 0; r < 2; r++) {
                for(int l = 0; l < 8; l++) { TM1638_SetLEDs(1 << l); __delay_ms(70); }
                for(int l = 7; l >= 0; l--) { TM1638_SetLEDs(1 << l); __delay_ms(70); }
            }
            TM1638_SetLEDs(0x00);
            __delay_ms(600);
        }

        KILIT_SIFIRLA: ; 
    }
}
