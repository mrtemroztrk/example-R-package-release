#include <Rcpp.h>
#include <cmath>
#include <unordered_map>
#include <string>

#ifdef _OPENMP
#include <omp.h>
#endif

using namespace Rcpp;

//' C++ ve OpenMP ile Paralel Mesafe Matrisi Hesaplama
//'
//' Bu fonksiyon, verilen bir matrisin satırları arasındaki
//' Öklid mesafelerini paralel olarak hesaplar.
//'
//' @param X Sayısal bir matris (satırlar gözlemleri, sütunlar özellikleri temsil eder).
//' @param threads Kullanılacak iş parçacığı sayısı. 0 verilirse sistemdeki tüm çekirdekler kullanılır.
//' @return N x N boyutunda bir mesafe matrisi.
//' @export
// [[Rcpp::export]]
NumericMatrix paralelUzaklikC(NumericMatrix X, int threads = 0) {
    int n = X.nrow();
    int d = X.ncol();
    NumericMatrix D(n, n);
    
    #ifdef _OPENMP
    if (threads > 0) {
        omp_set_num_threads(threads);
    }
    #endif
    
    #pragma omp parallel for schedule(static)
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            if (i == j) {
                D(i, j) = 0.0;
            } else if (i > j) {
                double sum = 0.0;
                for (int k = 0; k < d; k++) {
                    double diff = X(i, k) - X(j, k);
                    sum += diff * diff;
                }
                double dist = std::sqrt(sum);
                D(i, j) = dist;
                D(j, i) = dist; // Simetriklik özelliği
            }
        }
    }
    
    return D;
}

//' C++ ile Hızlı Hareketli Ortalama (Rolling Mean) Hesaplama
//'
//' Bu fonksiyon, sayısal bir vektör üzerinde belirtilen pencere boyutuna göre
//' hızlıca hareketli ortalama hesaplar.
//'
//' @param x Sayısal vektör.
//' @param n Pencere boyutu (pozitif bir tam sayı).
//' @return Giriş vektörüyle aynı uzunlukta hareketli ortalamaları içeren vektör. İlk n-1 eleman NA olacaktır.
//' @export
// [[Rcpp::export]]
NumericVector hareketliOrtalamaC(NumericVector x, int n) {
    int size = x.size();
    NumericVector sonuc(size);
    
    if (n <= 0 || n > size) {
        std::fill(sonuc.begin(), sonuc.end(), R_NaReal);
        return sonuc;
    }
    
    double sum = 0.0;
    
    // İlk pencereyi doldur
    for (int i = 0; i < n - 1; i++) {
        sonuc[i] = R_NaReal;
        sum += x[i];
    }
    sum += x[n - 1];
    sonuc[n - 1] = sum / n;
    
    // Kayan pencere algoritması (Sliding Window)
    for (int i = n; i < size; i++) {
        sum += x[i] - x[i - n];
        sonuc[i] = sum / n;
    }
    
    return sonuc;
}

//' C++ ile Hızlı Grup Ortalaması Hesaplama
//'
//' Bir vektörün değerlerini belirtilen gruplara göre gruplayarak
//' ortalamalarını hızlıca hesaplar.
//'
//' @param x Değerlerin bulunduğu sayısal vektör.
//' @param grup Grupları belirten karakter vektörü.
//' @return Grup isimlerini ve ortalamalarını içeren bir liste.
//' @export
// [[Rcpp::export]]
List grupOzetC(NumericVector x, CharacterVector grup) {
    int n = x.size();
    if (n != grup.size()) {
        stop("x ve grup vektörlerinin uzunlukları eşit olmalıdır.");
    }
    
    std::unordered_map<std::string, double> grup_toplam;
    std::unordered_map<std::string, int> grup_sayi;
    
    for (int i = 0; i < n; i++) {
        // Hızlı karakter çevrimi
        std::string g = as<std::string>(grup[i]);
        grup_toplam[g] += x[i];
        grup_sayi[g]++;
    }
    
    int n_groups = grup_toplam.size();
    CharacterVector isimler(n_groups);
    NumericVector ortalamalar(n_groups);
    
    int idx = 0;
    for (auto const& [anahtar, deger] : grup_toplam) {
        isimler[idx] = anahtar;
        ortalamalar[idx] = deger / grup_sayi[anahtar];
        idx++;
    }
    
    List sonuc = List::create(
        Named("grup") = isimler,
        Named("ortalama") = ortalamalar
    );
    
    return sonuc;
}
