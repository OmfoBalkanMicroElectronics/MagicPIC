# PIC16F886 Dahili ADC Sürücü Kütüphanesi ve Voltaj Göstergesi

Bu çalışma, **MagicPIC** projesi kapsamında PIC16F886 mikrodenetleyicisinin dahili 10-bit Analog-Dijital Dönüştürücü (ADC) modülünü kontrol etmek ve okunan verileri SSD1306 OLED ekran üzerinde gerçek zamanlı (real-time) görselleştirmek amacıyla geliştirilmiştir. Uygulama, yüksek bellek tüketen `float` veri tiplerini ve `sprintf` fonksiyonlarını tamamen devre dışı bırakarak, saf matematiksel oranlama yöntemleriyle asgari RAM/Flash harcaması yapacak şekilde optimize edilmiştir.

## Öne Çıkan Teknik Özellikler

* **TAD Saat Frekansı Frekansı Optimizasyonu:** 20 MHz harici osilatör frekansı altında, örnekleme kondansatörünün kararlı dolumu (Acquisition Time) için donanımsal dönüşüm saati **FOSC/32** (`ADCS = 2`) ve sağa hizalama (`ADFM = 1`) mimarisi kurulmuştur.
* **Hafif Voltaj Dönüşüm Algoritması:** Belleği şişirmemek adına analog voltaj `float` yerine `unsigned long` tipiyle doğrudan milivolt (0-5000 mV) cinsinden hesaplanır. Ardından matematiksel basamak bölme tekniğiyle tam volt ve ondalık kısımlarına ayıklanarak ekrana basılır.
* **Dinamik Grafik İlerleme Çubuğu (Progress Bar):** Ham ADC verisi (0-1023), OLED ekranın piksel sınırlarına (120 piksel genişlik) matematiksel olarak oranlanır. Her döngüde I2C üzerinden dikey piksel maskeleri (`0xFF` ve `0x01`) gönderilerek, potansiyometrenin konumuna göre anlık uzayıp kısalan kararlı bir grafik bar simülasyonu çalıştırılır.

## Donanım Pin Konfigürasyonu

| Çevre Birimi | Sinyal Adı | PIC16F886 Pini | Veri Yönü | Açıklama |
| :--- | :--- | :--- | :--- | :--- |
| **Potansiyometre** | Sinyal (Orta Bacak) | RA0 / AN0 (2. Pin) | Giriş | Analog Gerilim Girişi (0V - 5V) |
| **Potansiyometre** | Sol / Sağ Bacaklar | VDD / VSS | - | +5V ve GND Hatları |
| **SSD1306 OLED** | SCL | RC3 (14. Pin) | Giriş / Çıkış | Donanımsal I2C Saat Hattı |
| **SSD1306 OLED** | SDA | RC4 (15. Pin) | Giriş / Çıkış | Donanımsal I2C Veri Hattı |

## API Fonksiyonları ve Teknik Tanımları

### `void ADC_Init(void);`
`RA0` pinini dijital moddan çıkarıp analog moduna (`ANSEL`) geçirir. Voltaj referanslarını `VDD-VSS` (5V-GND) olarak ayarlar, dönüşüm saatini yapılandırır ve ADC modülünü enerji tasarrufu modundan çıkararak aktif eder.

### `unsigned int ADC_Read(char channel);`
ADCON0 register bütünü üzerinden hedef analog kanalı seçer. 20 mikro saniyelik elektriksel dolum payından sonra `GO/DONE` bitini tetikler. Donanımsal dönüşüm tamamlandığında `ADRESH` ve `ADRESL` register'larındaki 10-bitlik sağa dayalı veriyi birleştirerek `0-1023` arasında bir tam sayı döndürür.
