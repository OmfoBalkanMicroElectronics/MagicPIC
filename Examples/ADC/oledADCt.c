#include <xc.h>
#include "oled_ssd1306.h"
#include "adc.h"

#define _XTAL_FREQ 20000000 

#pragma config FOSC = HS, WDTE = OFF, PWRTE = OFF, MCLRE = ON
#pragma config CP = OFF, CPD = OFF, BOREN = OFF, IESO = OFF, FCMEN = OFF
#pragma config LVP = ON, WRT = OFF        

#define OLED_ADDR  0x3C

void main(void) {
    OSCCON = 0x00; 
    ANSEL = 0x00; 
    ANSELH = 0x00;
    
    // I2C Pin Yönlendirmeleri
    TRISC3 = 1; 
    TRISC4 = 1; 

    // Donan?mlar? Ba?lat
    OLED_Init(OLED_ADDR); 
    ADC_Init();

    unsigned int adc_ham = 0;
    unsigned long voltaj_milivolt = 0;
    int volt_tam = 0;
    int volt_ondalik = 0;

    // Ekranda bir kez yaz?lacak sabit ?ablonu olu?tur
    OLED_Set_Cursor(OLED_ADDR, 0, 0);
    OLED_Write_String(OLED_ADDR, "MAGICPIC ADC TEST");
    
    OLED_Set_Cursor(OLED_ADDR, 2, 0);
    OLED_Write_String(OLED_ADDR, "HAM ADC: ");
    
    OLED_Set_Cursor(OLED_ADDR, 4, 0);
    OLED_Write_String(OLED_ADDR, "VOLTAJ : ");

    while(1) {
        // AN0 (RA0) kanal?ndan analog veriyi oku (0 - 1023 aras?)
        adc_ham = ADC_Read(0);
        
        // Okunan de?eri ekranda yazd?r
        OLED_Set_Cursor(OLED_ADDR, 2, 54);
        OLED_Write_Int(OLED_ADDR, adc_ham);
        OLED_Write_String(OLED_ADDR, "    "); // Eski hanelerden kalan art?klar? temizle
        
        // Voltaj hesab?: Float kullan?p haf?zay? ?i?irmemek için milivolt cinsinden (tam say?) hesapl?yoruz
        // 5V referans için: (adc_ham * 5000) / 1023
        voltaj_milivolt = ((unsigned long)adc_ham * 5000) / 1023;
        
        volt_tam = (int)(voltaj_milivolt / 1000);       // Örn: 4850 / 1000 = 4 (Volt k?sm?)
        volt_ondalik = (int)(voltaj_milivolt % 1000);   // Örn: 4850 % 1000 = 850 (Kusurat k?sm?)

        // Volt de?erini ekrana bas (Örn: 4)
        OLED_Set_Cursor(OLED_ADDR, 4, 54);
        OLED_Write_Int(OLED_ADDR, volt_tam);
        OLED_Write_String(OLED_ADDR, "."); // Nokta koy
        
        // E?er ondal?k k?s?m 100'den küçükse (Örn: .050 veya .005 gibi durumlar için s?f?r ekleme yönetimi)
        if(volt_ondalik < 100) OLED_Write_String(OLED_ADDR, "0");
        if(volt_ondalik < 10)  OLED_Write_String(OLED_ADDR, "0");
        
        OLED_Write_Int(OLED_ADDR, volt_ondalik);
        OLED_Write_String(OLED_ADDR, "V   ");

        // Potu çevirirken görsel seviye bar? (Progress Bar) olu?turma:
        // Ekran geni?li?i 128 piksel. 1023 de?erini 120 piksele oranlayal?m: (adc_ham * 120) / 1023
        char bar_genislik = (char)(((unsigned long)adc_ham * 120) / 1023);
        
        // En alt sat?ra (Sayfa 7) dinamik çizgi çiziyoruz
        // Önce eski çizgiyi silmek için ekran?n o k?sm?n? temizleyebiliriz ancak 
        // daha h?zl? olmas? için do?rudan her döngüde çizgiyi s?f?rdan boyuyoruz
        OLED_Set_Cursor(OLED_ADDR, 7, 4);
        I2C_Start(); I2C_Write((char)(OLED_ADDR << 1)); I2C_Write(0x40);
        for(char c = 0; c < 120; c++) {
            if(c < bar_genislik) {
                I2C_Write(0xFF); // Pot de?erine göre dolu bar sütunu
            } else {
                I2C_Write(0x01); // Bar?n bitti?i yerde ince zemin çizgisi
            }
        }
        I2C_Stop();

        __delay_ms(100); // 100ms aral?klarla ekran? güncelle (Ak?c? ve kararl? tarama)
    }
}
