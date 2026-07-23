#include "adc.h"

void ADC_Init(void) {
    // 1. Pin Ayarlar?: AN0 (RA0) analog giri? olarak ayarlan?r
    TRISA0 = 1;  // RA0 giri? yap?ld?
    ANSEL |= 0x01; // AN0 pini analog moduna al?nd? (Bit 0 = 1)
    
    // 2. Voltaj Referans Ayar?: VREF+ = VDD (5V), VREF- = VSS (GND)
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    
    // 3. Dönü?üm Saati Frekans?: 20 MHz için FOSC/32 idealdir (TAD süresi uyumu için)
    ADCON0bits.ADCS = 2; // 10 -> FOSC/32
    
    // 4. Sonuç Hizalama: 10-bitlik veri sa?a dayal? (Right Justified) yap?l?r
    ADCON1bits.ADFM = 1;
    
    // 5. ADC Modülünü Aktif Et
    ADCON0bits.ADON = 1;
}

unsigned int ADC_Read(char channel) {
    if(channel > 13) return 0; // PIC16F886'da maksimum 14 analog kanal (0-13) vard?r
    
    // Kanal Seçimi: ADCON0 register içindeki CHS bitlerine kanal numaras?n? yükle
    ADCON0bits.CHS = channel;
    
    // ADC dahili örnekleme kondansatörünün dolmas? için k?sa bir süre bekle (Acquisition Time)
    __delay_us(20); 
    
    // Dönü?ümü Ba?lat: GO/DONE bitini 1 yap
    ADCON0bits.GO_nDONE = 1;
    
    // Dönü?üm bitene kadar (GO/DONE biti tekrar 0 olana kadar) bekle
    while(ADCON0bits.GO_nDONE);
    
    // 10-bitlik sonucu birle?tirerek geri döndür
    return ((ADRESH << 8) + ADRESL);
}
