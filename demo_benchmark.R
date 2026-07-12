library(ExamplePackage07)

cat("=== YÜKSEK PERFORMANS KARŞILAŞTIRMASI (BENCHMARK) ===\n\n")

# 1. Mesafe Matrisi Karşılaştırması
cat("1. Mesafe Matrisi Hesaplama (1000 satır x 100 sütun)\n")
set.seed(42)
mat <- matrix(runif(1000 * 100), nrow = 1000, ncol = 100)

t_R <- system.time({
  d_R <- as.matrix(dist(mat))
})
cat("   - R varsayılan dist() süresi:   ", t_R["elapsed"], "saniye\n")

t_C_1 <- system.time({
  d_C_1 <- paralelUzaklikC(mat, threads = 1)
})
cat("   - C++ Tek İş Parçacığı süresi:  ", t_C_1["elapsed"], "saniye\n")

t_C_p <- system.time({
  d_C_p <- paralelUzaklikC(mat, threads = 4)
})
cat("   - C++ Çoklu İş Parçacığı (4):  ", t_C_p["elapsed"], "saniye\n")
cat("   Sonuçlar eşleşiyor mu?:", all.equal(d_R, d_C_p, check.attributes = FALSE), "\n\n")


# 2. Hareketli Ortalama Karşılaştırması
cat("2. Hareketli Ortalama Hesaplama (1.000.000 eleman, pencere: 50)\n")
x <- rnorm(1e6)

# R'da stats::filter()
t_R_filter <- system.time({
  res_R <- stats::filter(x, rep(1/50, 50), sides = 1)
})
cat("   - R filter() süresi:            ", t_R_filter["elapsed"], "saniye\n")

t_C_roll <- system.time({
  res_C <- hareketliOrtalamaC(x, 50)
})
cat("   - C++ Sliding Window süresi:    ", t_C_roll["elapsed"], "saniye\n")
# İlk 49 elemanı NA olduğu için karşılaştırmayı 50. elemandan sonrasına yapalım
cat("   Sonuçlar eşleşiyor mu?:", all.equal(as.numeric(res_R[50:1e6]), res_C[50:1e6], check.attributes = FALSE), "\n\n")


# 3. Grup Özetleme Karşılaştırması
cat("3. Grup Ortalaması Hesaplama (1.000.000 satır, 5 grup)\n")
degerler <- runif(1e6)
gruplar <- sample(c("Grup_A", "Grup_B", "Grup_C", "Grup_D", "Grup_E"), 1e6, replace = TRUE)

t_R_agg <- system.time({
  res_R_agg <- aggregate(degerler ~ gruplar, FUN = mean)
})
cat("   - R aggregate() süresi:         ", t_R_agg["elapsed"], "saniye\n")

t_C_agg <- system.time({
  res_C_agg <- grupOzetC(degerler, gruplar)
})
cat("   - C++ Hash Map süresi:          ", t_C_agg["elapsed"], "saniye\n")

# Sonuçları eşleştirmek için sıralayalım
res_R_agg_sorted <- res_R_agg[order(res_R_agg$gruplar), ]
res_C_agg_df <- data.frame(gruplar = res_C_agg$grup, degerler = res_C_agg$ortalama)
res_C_agg_sorted <- res_C_agg_df[order(res_C_agg_df$gruplar), ]
cat("   Sonuçlar eşleşiyor mu?:", all.equal(res_R_agg_sorted$degerler, res_C_agg_sorted$degerler), "\n")
