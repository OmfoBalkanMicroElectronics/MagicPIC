# lib/core — Çekirdek Katman

Her MagicPIC projesinin temeli. Bu klasördeki dosyalar **çevre birimi sürücülerinden
bağımsızdır** ve donanıma en yakın katmanı oluşturur.

## Dosyalar

| Dosya | Rol |
|---|---|
| `system_config.h` | CONFIG word (`#pragma config`), `_XTAL_FREQ`, public prototipler |
| `system_config.c` | Clock başlatma, analog blok kapatma, port başlatma |
| `gpio_hal.h` | GPIO soyutlama makroları (sıfır maliyet) |
| `board_pins.h` | Kart seviyesi pin haritası — **projeye göre ezilebilir** |
| `debug_test.h` | Teşhis rutini prototipleri |
| `debug_test.c` | Self-test, heartbeat, buton echo, sürücü testleri |

---

## system_config

### Saat yapılandırması
#define _XTAL_FREQ   20000000UL       / 20 MHz harici HS kristal /
/ Komut çevrimi = FOSC/4 = 5 MHz -> 200 ns /

`__delay_ms()` / `__delay_us()` makroları `_XTAL_FREQ`'e dayanır. **Kristali değiştirirseniz
bu satırı da değiştirmelisiniz**, aksi halde tüm gecikmeler ve LCD zamanlaması bozulur —
belirti: LED beklenenden çok hızlı/yavaş flaşlar.

### CONFIG word

#pragma config FOSC   = HS        // Harici yüksek hızlı kristal
#pragma config WDTE   = OFF       // Watchdog kapalı
#pragma config PWRTE  = ON        // Güç açılış gecikmesi (osilatör oturması)
#pragma config MCLRE  = ON        // MCLR pini reset olarak
#pragma config CP     = OFF
#pragma config CPD    = OFF
#pragma config BOREN  = ON        // Brown-out reset
#pragma config IESO   = OFF
#pragma config FCMEN  = OFF
#pragma config LVP    = ON        //  RB3/PGM feda edilir
#pragma config BOR4V  = BOR40V
#pragma config WRT    = OFF


> ⚠️ **`LVP = ON` bilinçli bir tercihtir.** ZEPPP programlayıcı yalnızca LVP ile çalışır.
> Bedeli RB3 pinidir. Ayrıntı ve `LVP = OFF`'a geçişin **tek yönlü** olduğu uyarısı için
> [ana README → Programlama](../../../README.md#programlama-lvp-ve-hvp).

### API
void SYSTEM_Initialize(void);            / Tek çağrı: her şeyi kurar /
void SYS_DelayMs(uint16_t ms);           / Değişken süreli gecikme /
void SYS_GlobalInterruptEnable(void);
void SYS_GlobalInterruptDisable(void);


`SYSTEM_Initialize()` içinde sırayla:

1. `CLOCK_Initialize()` — `SCS = 0` (harici osilatör), `OSTS` bayrağı beklenir
2. `ANALOG_Disable()` — tüm pinler dijital yapılır
3. `PORTS_Initialize()` — portlar temizlenir, giriş yapılır, pull-up ve LVP koruması kurulur

### Neden `SYS_DelayMs` var, `__delay_ms` yetmiyor mu?

`__delay_ms()` **derleme zamanı sabiti** gerektirir; `__delay_ms(x)` gibi değişkenle
kullanılamaz. `SYS_DelayMs()` bunu bir döngüyle çözer.

### Analog blokların kapatılması — kritik ayrıntı

ANSEL  = 0x00U;   ANSELH = 0x00U;   / Tüm pinler DİJİTAL /
ADCON0bits.ADON = 0U;  ADCON1 = 0x00U;
CM1CON0bits.C1ON = 0U; CM2CON0bits.C2ON = 0U;
SRCON = 0x00U;
VRCON = 0x00U;    /  Bit ismiyle değil, TAM REGISTER yazımı /


**Reset sonrası `ANSEL`/`ANSELH` varsayılan olarak analog'dur.** Bunları temizlemezseniz
ilgili pinlerden **dijital okuma her zaman 0 döner** ve saatler süren "buton çalışmıyor"
teşhisi yaparsınız. Gömülü sistemde en sık yapılan hatalardan biridir.

`VRCON` satırı gerçek bir hatadan doğdu: `VRCONbits.C1VREN` / `C2VREN` PIC16F88x'in **başka
bir türevine** aittir, PIC16F886'da yoktur ve derleme hatası verir:

error: no member named 'C1VREN' in 'VRCONbits_t'; did you mean 'VREN'?


**Kalıcı kural:** Analog, karşılaştırıcı ve gerilim referansı bloklarında bit ismi yerine
**tam register yazımı** kullanın. Bit isimleri türevler arasında değişir, register adresleri
değişmez.

### PIC16F886'da `LATx` YOKTUR


PORTAbits.RA0 = 1;    /  DOĞRU /
LATAbits.LATA0 = 1;   /  HATA — LATx PIC16F1xxx ailesine aittir /


Bu, PIC16F1xxx örneklerinden kod uyarlarken en sık karşılaşılan derleme hatasıdır.

---

## gpio_hal.h

Pin erişimini soyutlar. **Hepsi makrodur** → fonksiyon çağrısı yok, stack tüketimi yok,
`-O0`'da bile tek assembly komutuna iner.

GPIO_SetOutput(A, 0)      / TRISA0 = 0 /
GPIO_SetInput(B, 1)       / TRISB1 = 1 /
GPIO_SetHigh(C, 0)        / PORTCbits.RC0 = 1 /
GPIO_SetLow(C, 0)
GPIO_Toggle(C, 0)
GPIO_Write(C, 0, value)
GPIO_Read(B, 0)           / Pin durumunu döndürür /
GPIO_ReleaseOpenDrain(C, 3)    / Açık-drain: pini bırak (yüksek-Z) /
GPIO_DriveLowOpenDrain(C, 3)   / Açık-drain: aktif LOW sür /
GPIO_PortBPullupsEnable()      / nRBPU = 0 /
GPIO_PortBPullupsDisable()
GPIO_PullupEnableB(0)          / WPUB0 = 1 /
GPIO_PullupDisableB(0)


**Kullanım kuralı:** İlk argüman port **harfi** (`A`, `B`, `C`), ikincisi bit **numarası**
(0–7). Sürücü kodunda `PORTx`/`TRISx` doğrudan yazılmaz; bu makrolar kullanılır. Böylece kart
değişince yalnızca `board_pins.h` düzenlenir.

**Açık-drain makroları** yazılımsal I²C için hazırlandı: I²C hatları asla aktif HIGH
sürülmez, pull-up direncine bırakılır.

---

## board_pins.h

Kart seviyesi bağlantıları anlamlı isimlere bağlar.

/ Debug LED — aktif HIGH, 470 Ω seri direnç /
DBG_LED_Init()  DBG_LED_SetHigh()  DBG_LED_SetLow()  DBG_LED_Toggle()
/ Debug buton — aktif LOW, dahili pull-up /
DBG_BTN_Init()  DBG_BTN_IsPressed()
/ 74HC595 hatları /
HC595_DS_Init()    HC595_DS_SetHigh()    HC595_DS_SetLow()
HC595_SHCP_Init()  HC595_SHCP_SetHigh()  HC595_SHCP_SetLow()
HC595_STCP_Init()  HC595_STCP_SetHigh()  HC595_STCP_SetLow()
/ Tümünü başlat — MAKRO (fonksiyon değil) /
BOARD_PinsInitialize()
BOARD_LVP_PGM_Guard()


### Koruma makroları

#define BOARD_RA6_RA7_USED_BY_XTAL     1   / Kristal pinleri I/O değil /
#define BOARD_RB3_RESERVED_FOR_LVP     1   / RB3 LVP için ayrıldı /


Bu makrolar sadece belgeleyici değil, **aktif korumadır**: bir sürücü yanlışlıkla RA6/RA7
veya RB3'ü kullanmaya çalışırsa `#error` ile derleme durur. Çalışma zamanında saatler
harcamak yerine derleme anında öğrenirsiniz.

`BOARD_LVP_PGM_Guard()` çalışma zamanında RB3'ü giriş olarak bırakır ve pull-up'ını kapatır.
Donanımda RB3'e **10 kΩ pull-down** eklenmesi önerilir.

### ⚠️ `BOARD_PinsInitialize` neden makro?

Başlangıçta `static inline` fonksiyondu ve şu uyarıları üretti:

warning: (2053) function "_BOARD_PinsInitialize@board_pins$F300" is never called


Sebep: XC8, header'daki `static inline` fonksiyondan **her çeviri birimi için ayrı bir kopya**
üretir. **Kural: header dosyalarında `static inline` kullanmayın; makro yazın.**

### Pin haritasını özelleştirmek

Kütüphaneye dokunmadan:

1. Bu dosyayı proje klasörünüze kopyalayın
2. Kopyayı düzenleyin
3. Include dizinlerinde `.` ilk sırada olduğu için derleyici sizin sürümünüzü bulur

Kütüphane güncellendiğinde pin haritanız korunur.

---

## debug_test

Sürücüleri **uygulamadan bağımsız** doğrulayan rutinler. Amacı "çalışmıyor" belirsizliğini
katman katman daraltmaktır.

| Fonksiyon | Ne yapar | Başarısızlık anlamı |
|---|---|---|
| `DEBUG_HeartBeat(n)` | LED'i n kez flaşlar | Kod hiç çalışmıyor veya clock yanlış |
| `DEBUG_GpioSelfTest()` | Pin yaz/oku tutarlılığı (`0` = hata) | GPIO/TRIS yapılandırması bozuk |
| `DEBUG_ButtonEchoLoop()` | Butona basınca LED yanar | Giriş veya pull-up sorunu |
| `DEBUG_Hc595WalkTest()` | QA→QH yürüyen bit | 595 beslemesi, /OE, /MR veya bit sırası |
| `DEBUG_LcdSelfTest()` | 4 aşamalı LCD testi | Kontrast, init, veri hatları |
| `DEBUG_LcdScrollTest()` | ENTER + WRAP modları | Kaydırma indekslemesi |
| `DEBUG_LcdMarqueeAppLoop()` | Sayaç + kayan yazı + buton | Bloklama var mı |

### Doğru teşhis sırası — aşağıdan yukarı


1.HeartBeat        Clock ve kod akışı yaşıyor mu?
1.GpioSelfTest     Pin kontrolü doğru mu?
3.Hc595WalkTest    Shift register çalışıyor mu?  (LCD BAĞLI DEĞİLKEN)
4.LcdSelfTest      LCD init ve karakter yazımı?
5.LcdScrollTest    Özellik katmanı?

**Alt katman geçmeden üst katmanı test etmeyin.** 3. adımı atlarsanız "LCD bozuk mu, 595 mi
bozuk?" sorusunda saplanırsınız.

Yeni sürücü eklerken buraya bir self-test eklemek **zorunludur**.

---

## Sık karşılaşılan hatalar

| Belirti | Sebep |
|---|---|
| Pinden okuma her zaman 0 | `ANSEL`/`ANSELH` temizlenmedi (pin analog modda) |
| `error: no member named 'C1VREN'` | Yanlış türev bit ismi → `VRCON = 0x00U` yazın |
| `error: no member named 'LATA0'` | `LATx` PIC16F886'da yok → `PORTAbits` kullanın |
| `warning: (2053) ... never called` | Header'da `static inline` → makroya çevirin |
| LED çok hızlı/yavaş flaşlıyor | `_XTAL_FREQ` gerçek kristalle uyuşmuyor |
| MCU hiç başlamıyor | `FOSC` HS değil, kristal/yük kondansatörü, VDD < 4.5 V |
| Program rastgele resetleniyor | Stack taşması (çağrı derinliği > 8) veya decoupling yok |



