#include "system_config.h"
#include "board_pins.h"

/* ================= CONFIG WORD 1 (PIC16F886) ================= */
#pragma config FOSC  = HS             /* Harici kristal 20 MHz (HS) */
#pragma config WDTE  = OFF
#pragma config PWRTE = ON             /* PWRT + OST: kristal oturmadan kod baslamaz */
#pragma config MCLRE = ON             /* LVP giris dizisi icin harici MCLR aktif */
#pragma config CP    = OFF
#pragma config CPD   = OFF
#pragma config BOREN = ON
#pragma config IESO  = OFF
#pragma config FCMEN = OFF
#pragma config LVP   = ON             /* >>> ZORUNLU: ZEPPP / Arduino Nano LVP <<< */

/* ================= CONFIG WORD 2 (PIC16F886) ================= */
#pragma config BOR4V = BOR40V         /* 20 MHz -> VDD >= 4.5V */
#pragma config WRT   = OFF

/* ---------------- Alt seviye baslatma (static: ROM/stack dostu) ---------------- */

static void CLOCK_Initialize(void)
{
    /* FOSC = HS iken clock dogrudan OSC1/OSC2 kristalinden gelir.
     * IRCF bitleri KULLANILMAZ (sadece INTOSC icin gecerlidir).        */
    uint8_t guard = 0U;

    OSCCONbits.SCS = 0U;                 /* 0 = FOSC<2:0> (harici HS)  */

    /* OSTS = 1 -> cihaz CONFIG'te tanimli harici osilatorden calisiyor.
     * Sinirli dongu: kilitlenme yerine kontrollu cikis.                */
    while ((OSCCONbits.OSTS == 0U) && (guard < 200U))
    {
        guard++;
        __delay_us(50);
    }
}

static void ANALOG_Disable(void)
{
    /* PIC16F886'da pinler reset sonrasi ANALOG gelir -> dijital yap */
    ANSEL  = 0x00U;   /* AN0..AN7  */
    ANSELH = 0x00U;   /* AN8..AN13 */

    ADCON0bits.ADON = 0U;   /* ADC modulu kapali */
    ADCON1 = 0x00U;         /* VREF = VDD/VSS (ADC kapali, notr durum) */

    CM1CON0bits.C1ON = 0U;  /* Karsilastirici 1 kapali */
    CM2CON0bits.C2ON = 0U;  /* Karsilastirici 2 kapali */
    SRCON  = 0x00U;         /* SR latch notr */

    /* PIC16F886'da TEK bir CVREF modulu vardir.
     * VRCON bitleri: VREN, VROE, VRR, VRSS, VR3:VR0
     * (C1VREN/C2VREN bu cihazda YOKTUR.)
     * Tum modulu tek yazimla kapatmak en verimli ve en guvenli yoldur: */
    VRCON = 0x00U;          /* VREN=0 (kapali), VROE=0 (RA2 serbest) */
}

static void PORTS_Initialize(void)
{
    /* 1) Data register'lari temizle (glitch'siz yon degisimi) */
    PORTA = 0x00U;
    PORTB = 0x00U;
    PORTC = 0x00U;

    /* 2) Varsayilan: tum pinler giris */
    TRISA = 0xFFU;
    TRISB = 0xFFU;
    TRISC = 0xFFU;

    /* 3) PORTB pull-up'lar ve IOC kapali */
    OPTION_REGbits.nRBPU = 1U;
    WPUB = 0x00U;
    IOCB = 0x00U;

    /* 4) LVP = ON -> RB3/PGM giris + pull-up kapali (kartta 10k pull-down) */
    BOARD_LVP_PGM_Guard();

    /* 5) Kart pin haritasi */
    BOARD_PinsInitialize();
}

/* ---------------- Genel gecikme yardimcisi ---------------- */

void SYS_DelayMs(uint16_t ms)
{
    while (ms-- != 0U)
    {
        __delay_ms(1);
    }
}

/* ---------------- Ust seviye baslatma (main.c bunu cagirir) ---------------- */

void SYSTEM_Initialize(void)
{
    SYS_GlobalInterruptDisable();
    CLOCK_Initialize();
    ANALOG_Disable();
    PORTS_Initialize();
}