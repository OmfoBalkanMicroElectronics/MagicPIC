## MagicPIC

<p align="center">
  <img src="images/magicpic.png" width="900">
</p>

*"Inspired by a 16-year-old MagicFINDER Plus 3 and the curiosity to understand what was inside."*


*Brought to you by OBM*

MagicPIC, klasik Microchip PIC mikrodenetleyicilerini daha yakından tanımak, belgelemek ve geliştirmek amacıyla oluşturulmuş açık kaynaklı bir gömülü sistem projesidir.


Proje, **PIC16F886** mikrodenetleyicisini temel almakta olup; donanım tasarımı, çevre birimi sürücüleri, örnek uygulamalar ve kapsamlı dokümantasyon geliştirmeyi hedeflemektedir.


## Amaç


MagicPIC'in temel amacı, PIC platformunu öğrenmek isteyen geliştiricilere anlaşılır, erişilebilir ve pratik kaynaklar sunmaktır.


Bu proje kapsamında;




•PIC16F886 tabanlı geliştirme kartlarının tasarlanması


•Temel ve gelişmiş çevre birimlerinin sürülmesi


•LCD, OLED, UART, SPI, I²C, ADC, PWM ve Timer örnekleri


•Modüler sürücü kütüphaneleri


•Örnek projeler


•Donanım şemaları


•Teknik dokümantasyon




geliştirilecektir.


## Proje Durumu


MagicPIC aktif geliştirme aşamasındadır.


İlk donanım prototiplerinin hazırlanmasının ardından örnek uygulamalar ve sürücü kütüphaneleri düzenli olarak depoya eklenecektir. Projenin temel hedeflerinden biri, PIC16F886 için Türkçe kaynak eksikliğini gidermek ve geliştiricilerin hızlı bir şekilde çalışmaya başlayabileceği kapsamlı bir referans oluşturmaktır.


## Yol Haritası




PIC16F886 geliştirme kartı


GPIO sürücüleri


Timer modülleri


ADC örnekleri


UART haberleşmesi


SPI ve I²C sürücüleri


LCD ve OLED örnekleri


Bootloader araştırmaları


Gelişmiş çevre birimi örnekleri


Donanım ve yazılım dokümantasyonu




Katkıda Bulunma


## Katkıda Bulunma

MagicPIC açık kaynaklı bir projedir.

Hata bildirimleri, öneriler, dokümantasyon katkıları ve yeni örnek projeler her zaman memnuniyetle karşılanmaktadır.

## Lisans

Bu proje MIT Lisansı altında lisanslanmıştır.  
Detaylar için `LICENSE` dosyasına bakabilirsiniz.

## Maintainer

**Barbaror4**

# FAQ (Sıkça Sorulan Sorular)

## Proje ismi nereden geldi?

MagicPIC ismi, **ProCenter Elektronik** tarafından üretilen ve geçmişte kullanılan **MagicFINDER** serisi cihazlardan esinlenerek oluşturulmuştur.

## Neden MagicFINDER?

Bu projenin isim hikayesi aslında bir elektronik tamir macerasına dayanıyor.

Babamın uzun yıllardır kullandığı **MagicFINDER Plus 3** uydu ölçüm cihazı, Şubat aylarında arızalandı. Elektroniğe olan ilgim nedeniyle cihazı inceleyip tamir etmeye karar verdim.

İlk incelemelerde sorunun klavye tarafında, özellikle zebra konektör veya keypad bağlantılarında olduğunu düşündüm. Çeşitli jumper ve kablo bağlantıları ile denemeler yapmama rağmen cihazın tuşları düzgün çalışmadı. Sadece bazı tuşlar, özellikle **DOWN** ve güç tuşu çalışıyordu.

10 Temmuz tarihinde cihazı tekrar incelemeye karar verdim. Bu sefer doğrudan cihazın ana işlemcisi olan **PIC18F4525** mikrodenetleyicisine odaklandım.

Keypad matris bağlantılarını ve işlemci pinlerini kontrol ettiğimde sorunun işlemcinin **RE1 pini** üzerinde olduğunu tespit ettim. Yapılan ölçümlerde:

- RE1 pininin VDD hattına kısa devre yaptığı
- Yaklaşık **3.51kΩ** seviyesinde kaçak direnç bulunduğu
- Dahili ESD koruma yapısının hasar görmüş olabileceği

belirlendi.

Ne yazık ki bu durum mikrodenetleyicinin silikon seviyesinde oluşmuş bir arızaya işaret ediyordu. RE1 pini izole edildiğinde ise cihazın güç kontrol süreci düzgün çalışmadığı için cihaz tamamen normal şekilde başlayamıyordu.

Bu durum benim için biraz üzücüydü. Elektronik cihazlara sadece bir eşya olarak değil, içerisinde yıllarca çalışan bir tasarım ve emek bulunan sistemler olarak bakıyorum. Bu cihaz ailemizde 16 yıldan uzun süredir kullanılıyordu ve tüm donanımı; ekranı, tuner bölümü ve diğer sistemleri hâlâ çalışır durumdayken, yalnızca tek bir mikrodenetleyici pini nedeniyle işlevlerinin büyük kısmını kaybetmesi oldukça ilginç bir deneyimdi.

Bu deneyim, benim için sadece bir cihaz tamiri değildi. MagicFINDER olmasaydı belki de bugün PIC mikrodenetleyicileriyle bu kadar ilgilenmeyecektim.

Bir cihazın yıllarca çalıştıktan sonra, çok küçük ve doğrudan değiştirilemeyen bir mikrodenetleyici arızası nedeniyle kullanılamaz hale gelmesi benim için önemli bir öğrenme deneyimi oldu. Sorunun çevre birimlerinde değil, doğrudan işlemcinin içinde olduğunu anlamak ve bunu değiştirememek, elektronik sistemlerin ne kadar hassas ve karmaşık olabileceğini gösterdi.

Bu noktadan sonra klasik mikrodenetleyicilere, özellikle PIC ailesine olan ilgim arttı. MagicPIC, biraz da bu deneyimin sonucunda; mikrodenetleyicileri daha iyi anlamak, kendi donanımlarımı geliştirmek ve bu platformları daha yakından tanımak amacıyla ortaya çıktı.
