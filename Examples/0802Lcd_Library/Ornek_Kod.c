#include <xc.h>
#include "lcd0802.h"

#define _XTAL_FREQ 20000000 

#pragma config FOSC = HS, WDTE = OFF, PWRTE = OFF, MCLRE = ON
#pragma config CP = OFF, CPD = OFF, BOREN = OFF, IESO = OFF, FCMEN = OFF
#pragma config LVP = ON, WRT = OFF        

void main(void) {
    OSCCON = 0x00; 
    ANSEL = 0x00; 
    ANSELH = 0x00;
    
    // LCD Pin Yonlendirmeleri
    TRISA0 = 0; TRISA1 = 0;
    TRISB0 = 0; TRISB1 = 0; TRISB2 = 0; TRISB4 = 0;

    Lcd_Init(); 

    while(1) {
        Lcd_Set_Cursor(2, 1);
        Lcd_Write_String("MagicPIC"); // Alt satira sabit yazi
        
        // Ust satirda kayan yazi testi (Hiz: 300 ms)
        Lcd_Scroll_Text(1, "        PIC16F886 Kütüphane Testi        ", 300);
    }
}
