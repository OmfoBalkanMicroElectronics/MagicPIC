# MagicPIC — Firmware

Bu klasör MagicPIC'in tüm gömülü yazılım kaynağını içerir.

Genel proje tanıtımı, donanım ve programlayıcı bilgileri için [ana README](../README.md)'ye
bakın.

## Yapı
firmware/
├── lib/                 Paylaşılan kod — TÜM projeler bunu kullanır
│   ├── core/            Sistem, GPIO HAL, kart pinleri, debug
│   └── drivers/         Çevre birimi sürücüleri
└── projects/            Uygulamalar (her biri bağımsız MPLAB X projesi)


## Tek kural

> **`lib/` içindeki dosyalar asla bir proje klasörüne kopyalanmaz.**
> Projelere **göreli yol (relative) link** olarak eklenir.

Sebebi basit: LCD sürücüsünde bir zamanlama hatası bulduğunuzda düzeltmeyi tek yerde
yaparsınız ve tüm projeler faydalanır. Kopyalarsanız 6 ay sonra 5 farklı sürümünüz olur ve
hangisinin doğru olduğunu bilemezsiniz.

## Katman modeli
UYGULAMA        projects/xx_name.X/main.c
↓           "Ne yapılacak" bilinir, "nasıl" bilinmez
ÖZELLİK         lib/drivers/*/  (örn. lcd_scroll.c)
↓           Donanım bilgisi SIFIR
PROTOKOL        lib/drivers/*/  (örn. lcd0802.c)
↓           Cihaz komut seti ve zamanlaması
ARAYÜZ          lib/drivers/*/  (örn. lcd_if_gpio.c / lcd_if_595.c)
↓           Fiziksel bağlantı yolu — DEĞİŞTİRİLEBİLİR
HAL             lib/core/gpio_hal.h + board_pins.h
↓           Makro tabanlı, sıfır maliyet
DONANIM         PIC16F886 SFR



Her katman yalnızca **bir alt katmanın arayüzünü** kullanır, implementasyonunu bilmez. Bu
sayede LCD'yi 6 pinli paralel bağlantıdan 3 pinli 74HC595'e taşımak için üst üç katmanda
**tek satır** değişmedi.

## Yeni projede kütüphaneyi kullanmak

**1) Dosyaları link olarak ekleyin**

**Source Files** sağ tık → **Add Existing Item...** → gerekli `.c` dosyaları →
**"Store path as" → `Relative`** ⚠️

`Copy` seçilirse paylaşım biter.

**2) Include dizinleri**

**Project Properties → XC8 Compiler → Preprocessing and messages → Include directories:**
.;../../lib/core;../../lib/drivers/lcd0802;../../lib/drivers/hc595

Baştaki `.` **ilk sırada** olmalı → proje klasörüne koyduğunuz `board_pins.h` kütüphanedeki
varsayılanı ezer. Pin özelleştirmesinin tüm mekanizması budur.

**3) Derleyici makroları**

**Preprocessor macros:**
LCD_INTERFACE=LCD_IF_74HC595


## Zorunlu derleyici ayarları

| Ayar | Değer | Neden |
|---|---|---|
| Compiler | XC8 v3.10+ | Test edilen sürüm |
| C standard | `-std=c99` | `stdint.h`, blok içi bildirim |
| Optimization | `-O0` | Ücretsiz XC8 sınırı; kod bu varsayımla yazıldı |
| Stack model | `compiled:auto:auto` | Varsayılan |
| Config bits | `-mdefault-config-bits` | `#pragma config` kaynaktan gelir |

`-O0` önemli: kütüphane hiçbir yerde optimizasyona bağımlı davranış (örneğin `inline`
edileceği varsayımıyla yazılmış kritik zamanlama) içermez. Daha yüksek optimizasyonla da
çalışır ama doğrulama `-O0` ile yapılmıştır.

## Kaynak bütçesi

PIC16F886: **8 kB Flash / 368 byte RAM / 256 byte EEPROM / 8 seviye donanım stack**

Şu anki tam yapılandırma (LCD + 595 + scroll + debug) yaklaşık:

| Kaynak | Kullanım | Oran |
|---|---|---|
| Program (Flash) | ~2.0–2.5 kB | ~%25–30 |
| Data (RAM) | ~15–20 byte | ~%5 |
| Stack derinliği | 6 / 8 seviye | 2 seviye ISR payı |

Stack en kıymetli kaynaktır ve **derleyici sizi uyarmaz** — taşarsa program sessizce çöker.
Bu yüzden çağrı derinliği kuralı katıdır.

## Kodlama kuralları

Katkı vermeden önce mutlaka okuyun:
[Kodlama kuralları](../README.md#kodlama-kuralları-katkı-verecekler-için-zorunlu)

Özet: dinamik bellek yok, `float` yok, `printf` yok, `stdint.h` tipleri, çağrı derinliği ≤ 6,
header'da `static inline` yok, `%` ve `/` operatörlerinden kaçın, analog bloklarda tam
register yaz, `LATx` yoktur, uydurma register ismi yasak.

## Alt klasör dokümanları

- [`lib/core/README.md`](lib/core/README.md) — sistem, HAL, pinler, debug
- [`lib/drivers/README.md`](lib/drivers/README.md) — sürücü indeksi ve yazım şablonu
- [`projects/README.md`](projects/README.md) — proje listesi ve oluşturma rehberi

- 


