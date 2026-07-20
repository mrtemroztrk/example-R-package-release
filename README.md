# ExamplePackage07 

`ExamplePackage07`, Rcpp ve OpenMP teknolojileri kullanılarak C++ ile yazılmış yüksek performanslı sayısal işlem fonksiyonları sunan bir R paketidir. Büyük veri kümeleri üzerinde hızlı işlem yapmak için optimize edilmiştir.

## Özellikler

-  **Paralel Mesafe Hesaplama:** OpenMP ile çok çekirdekli sistemlerde Öklid mesafesi hesaplama.
-  **Hızlı Hareketli Ortalama (Rolling Mean):** Kayan pencere (sliding window) algoritması ile anında hesaplama.
-  **Hızlı Grup Ortalamaları:** C++ `std::unordered_map` (hash table) yapısı ile `aggregate` işlemlerine göre çok daha hızlı gruplama.
-  **Vektör Kare Alma:** R'ın standart döngülerine göre C++ seviyesinde hızlı vektör eleman karesi hesaplama.

---

## Kurulum (Installation)

Paketi GitHub üzerinden doğrudan yükleyebilirsiniz. Paket içerisinde C++ kodları barındırdığından, yükleme öncesinde sisteminizde bir derleyicinin kurulu olması gerekmektedir.

### Ön Gereksinimler

- **Windows:** [Rtools](https://cran.r-project.org/bin/windows/Rtools/) yüklü olmalıdır.
- **macOS:** Terminalden `xcode-select --install` komutuyla Xcode Command Line Tools kurulmuş olmalıdır.
- **Linux:** `r-base-dev` veya `build-essential` paketleri kurulu olmalıdır.

### R Konsolu üzerinden Kurulum

```R
# devtools paketi kurulu değilse:
install.packages("devtools")

# Paketi GitHub'dan yükleme:
devtools::install_github("mrtemroztrk/example-R-package-release")
```

---

## Örnek Kullanım (Usage Examples)

Paketi yükledikten sonra aşağıdaki gibi kullanmaya başlayabilirsiniz:

```R
library(ExamplePackage07)
```

### 1. Paralel Mesafe Matrisi Hesaplama (`paralelUzaklikC`)
Verilen bir sayısal matrisin satırları arasındaki Öklid mesafelerini paralel olarak hesaplar.

```R
# Rastgele 5 satır ve 3 sütunluk bir matris oluşturalım
matris <- matrix(runif(15), nrow = 5, ncol = 3)

# Tüm çekirdekleri kullanarak paralel mesafe matrisini hesapla
mesafeler <- paralelUzaklikC(matris, threads = 0)
print(mesafeler)
```

### 2. Hızlı Hareketli Ortalama (`hareketliOrtalamaC`)
Sayısal bir vektörün belirli bir pencere boyutuna (`n`) göre hareketli ortalamasını hesaplar. İlk `n-1` eleman `NA` değerini alır.

```R
veri <- c(1, 2, 4, 8, 16, 32)

# 3 elemanlık pencere boyutu ile hareketli ortalama
hareketli_ort <- hareketliOrtalamaC(veri, n = 3)
print(hareketli_ort)
# Çıktı: [1] NA NA 2.333333 4.666667 9.333333 18.666667
```

### 3. Hızlı Grup Ortalaması (`grupOzetC`)
Bir sayısal vektörün değerlerini belirtilen kategorilere göre gruplandırarak hızlıca ortalamasını alır.

```R
degerler <- c(10, 20, 15, 30, 25)
kategoriler <- c("A", "B", "A", "B", "A")

# Grupların ortalamasını hesapla
ozet <- grupOzetC(degerler, kategoriler)
print(ozet)
# Çıktı listesi:
# $grup -> "A", "B"
# $ortalama -> 16.66667, 25.00000
```

### 4. Vektörün Karesini Alma (`vektorKareC`)
Bir sayısal vektörün tüm elemanlarının karesini hızlı bir şekilde döndürür.

```R
vektor <- c(1, 2, 3, 4, 5)
kareler <- vektorKareC(vektor)
print(kareler)
# Çıktı: [1] 1 4 9 16 25
```

---

## Performans Karşılaştırması (Benchmark)

Paketin R içindeki standart fonksiyonlara kıyasla performansını görmek için [demo_benchmark.R](file:///home/mrtemroztrk/Projects/MuratPaket/demo_benchmark.R) dosyasını çalıştırabilirsiniz:

```bash
Rscript demo_benchmark.R
```

### Örnek Karşılaştırma Sonuçları:
- **Mesafe Hesaplama (1000 satır x 100 sütun):** `paralelUzaklikC` (çoklu iş parçacığı ile) R'ın standart `dist()` fonksiyonuna göre **çok daha hızlıdır**.
- **Hareketli Ortalama (1.000.000 eleman):** C++ tabanlı `hareketliOrtalamaC` fonksiyonu, `stats::filter()` fonksiyonuna kıyasla **mil saniyeler düzeyinde** sonuç verir.
- **Grup Ortalaması (1.000.000 satır):** `grupOzetC` fonksiyonu, R'ın standart `aggregate()` fonksiyonundan **katlarca hızlı** çalışarak büyük veri setlerinde zaman kazandırır.

---

## Lisans (License)

Bu paket **GPL (>= 3)** lisansı altında lisanslanmıştır.
