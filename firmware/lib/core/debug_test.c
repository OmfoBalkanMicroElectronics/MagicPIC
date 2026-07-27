#include "system_config.h"
#include "board_pins.h"
#include "debug_test.h"
#include "lcd0802.h"
#include "lcd_if.h"
#include "hc595.h"
#include "lcd_scroll.h"

#if (LCD_INTERFACE == LCD_IF_74HC595)

static const char scrollDemoText[] = "MAGICPIC 0802 LCD - PIC16F886 @20MHz - 74HC595 3-PIN MODE *** ";
static const char scrollEnterText[] = "HELLO PIC16F886";



/* 1) Kalp at???: clock ve CONFIG do?ru mu? (250 ms periyot beklenir) */
void DEBUG_HeartBeat(uint8_t blinks)
{
    uint8_t i;
    for (i = 0U; i < blinks; i++)
    {
        DBG_LED_On();
        __delay_ms(125);
        DBG_LED_Off();
        __delay_ms(125);
    }
}

/* 2) GPIO soyutlama katman? do?rulamas?:
 *    Ç?k?? pinine yaz -> ayn? pini PORT üzerinden geri oku.
 *    Beklenen: yaz?lan seviye okunur. Aksi halde pin analog kalm?? (ANSEL
 *    hatas?) veya harici k?sa devre/a??r? yük var demektir.                */
uint8_t DEBUG_GpioSelfTest(void)
{
    uint8_t ok = 1U;

    GPIO_SetOutput(C, 0);

    GPIO_SetHigh(C, 0);
    __delay_us(20);                 /* RC yükü için oturma süresi */
    if (GPIO_Read(C, 0) != 1U) { ok = 0U; }

    GPIO_SetLow(C, 0);
    __delay_us(20);
    if (GPIO_Read(C, 0) != 0U) { ok = 0U; }

    GPIO_Toggle(C, 0);
    __delay_us(20);
    if (GPIO_Read(C, 0) != 1U) { ok = 0U; }
    GPIO_SetLow(C, 0);

    /* LVP güvenli?i: RB3 hâlâ giri? olarak duruyor mu? */
    if (TRISBbits.TRISB3 != 1U) { ok = 0U; }

    return ok;
}

/* 3) Giri? katman? testi: butona bas?ld?kça LED yanar (pull-up do?rulamas?) */
void DEBUG_ButtonEchoLoop(void)
{
    for (;;)
    {
        if (DBG_BTN_IsPressed()) { DBG_LED_On(); }
        else                     { DBG_LED_Off(); }
        __delay_ms(10);          /* basit debounce/örnekleme */
    }
}
/* LCD 4 asamali gorsel self-test:
 * 1) Tum karakter hucreleri doluyor mu? (kontrast/pot dogrulamasi)
 * 2) Iki satir da adreslenebiliyor mu? (DDRAM 0x00 / 0x40)
 * 3) Metin yazimi ve tasma korumasi
 * 4) Sayi/hex bicimlendirme                                        */
void DEBUG_LcdSelfTest(void)
{
    uint8_t i;

    /* Asama 1: her iki satiri '#' ile doldur -> kontrast ayari icin referans */
    LCD_Clear();
    for (i = 0U; i < LCD_COLS; i++) { LCD_PutStringAt(i, 0U, "#"); }
    for (i = 0U; i < LCD_COLS; i++) { LCD_PutStringAt(i, 1U, "#"); }
    SYS_DelayMs(800);

    /* Asama 2 + 3: iki satir adresleme ve metin */
    LCD_Clear();
    LCD_PutStringAt(0U, 0U, "PIC16F88");
    LCD_PutStringAt(0U, 1U, "6 LCD OK");
    SYS_DelayMs(1200);

    /* Asama 4: sayi ve hex bicimlendirme */
    LCD_Clear();
    LCD_PutStringAt(0U, 0U, "N=");
    LCD_PutUint16(12345U);
    LCD_PutStringAt(0U, 1U, "H=0x");
    LCD_PutHex8(0xA5U);
    SYS_DelayMs(1200);
}

/* Canli test: sayac artiyorsa zamanlama ve E darbesi kararlidir.
 * Butona (RB0) basildiginda sayac sifirlanir -> giris + LCD birlikte test. */
void DEBUG_LcdCounterLoop(void)
{
    uint16_t counter = 0U;

    LCD_Clear();
    LCD_PutStringAt(0U, 0U, "COUNT:");

    for (;;)
    {
        if (DBG_BTN_IsPressed()) { counter = 0U; }

        LCD_SetCursor(0U, 1U);
        LCD_PutString("        ");     /* Satiri temizle (artik hane kalmasin) */
        LCD_SetCursor(0U, 1U);
        LCD_PutUint16(counter);

        DBG_LED_Toggle();              /* Ana dongu yasiyor mu? */
        counter++;
        SYS_DelayMs(250);
    }
}
void DEBUG_Hc595WalkTest(void)
{
    uint8_t pattern;
    uint8_t pass;

    HC595_Initialize();

    for (pass = 0U; pass < 3U; pass++)
    {
        pattern = 0x01U;
        while (pattern != 0U)
        {
            HC595_Write(pattern);
            DBG_LED_Toggle();          /* MCU dongusu yasiyor mu? */
            SYS_DelayMs(150);
            pattern = (uint8_t)(pattern << 1);
        }
    }

    HC595_Write(0x00U);                /* Tum cikislar LOW ile bitir */
}
#else
void DEBUG_Hc595WalkTest(void) { ; }   /* GPIO modunda islevsiz (link uyumu) */
#endif
void DEBUG_LcdScrollTest(void)
{
    lcd_scroll_t marquee;
    uint16_t     frame;

    /* --- 1) ENTER modu: metin sagdan girer, soldan cikar --- */
    LCD_Clear();
    LCD_PutStringAt(0U, 0U, "MODE:IN ");
    LCD_ScrollBegin(&marquee, scrollEnterText, 1U, LCD_SCROLL_MODE_ENTER);

    for (frame = 0U; frame < 60U; frame++)
    {
        LCD_ScrollStep(&marquee);
        SYS_DelayMs(180);
        if (LCD_ScrollIsCycleDone(&marquee) != 0U) { break; }   /* 1 tur yeter */
    }
    SYS_DelayMs(400);

    /* --- 2) WRAP modu: kesintisiz dongusel kayan yazi --- */
    LCD_Clear();
    LCD_PutStringAt(0U, 0U, "MODE:WRP");
    LCD_ScrollBegin(&marquee, scrollDemoText, 1U, LCD_SCROLL_MODE_WRAP);

    for (frame = 0U; frame < 60U; frame++)
    {
        LCD_ScrollStep(&marquee);
        SYS_DelayMs(200);
    }
}

/*
 * Non-blocking mimarinin gercek faydasi:
 * Ust satirda sayac artiyor, ALT satirda yazi kayiyor, ayni anda buton
 * okunuyor ? hepsi TEK ana dongude, hicbir islem digerini bloklamiyor.
 * Butona basildikca kaydirma hizi degisir (yavas/hizli).
 */
void DEBUG_LcdMarqueeAppLoop(void)
{
    lcd_scroll_t marquee;
    uint16_t     counter  = 0U;
    uint16_t     tickScrl = 0U;
    uint16_t     tickCnt  = 0U;
    uint8_t      stepDiv  = 8U;      /* Kaydirma periyodu = stepDiv x 25 ms */

    LCD_Clear();
    LCD_ScrollBegin(&marquee, scrollDemoText, 1U, LCD_SCROLL_MODE_WRAP);

    for (;;)
    {
        /* --- 25 ms'lik temel zaman dilimi (yazilim tick) --- */
        SYS_DelayMs(25);
        tickScrl++;
        tickCnt++;

        /* --- Gorev 1: kayan yazi (alt satir) --- */
        if (tickScrl >= stepDiv)
        {
            tickScrl = 0U;
            LCD_ScrollStep(&marquee);
        }

        /* --- Gorev 2: sayac (ust satir, 500 ms) --- */
        if (tickCnt >= 20U)
        {
            tickCnt = 0U;
            LCD_SetCursor(0U, 0U);
            LCD_PutString("N=      ");     /* Satiri temizle */
            LCD_SetCursor(2U, 0U);
            LCD_PutUint16(counter);
            counter++;
            DBG_LED_Toggle();
        }

        /* --- Gorev 3: buton -> kaydirma hizini degistir --- */
        if (DBG_BTN_IsPressed())
        {
            stepDiv = (stepDiv > 3U) ? 3U : 12U;   /* hizli <-> yavas */
            SYS_DelayMs(200);                      /* basit debounce */
        }
    }
}