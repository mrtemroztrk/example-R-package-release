#include <Rcpp.h>
using namespace Rcpp;

//' C++ ile Vektörün Karesini Alan Hızlı Fonksiyon
//' @param x Sayısal vektör
//' @export
// [[Rcpp::export]]
NumericVector vektorKareC(NumericVector x) {
    int n = x.size();
    NumericVector sonuc(n);
    
    for(int i = 0; i < n; i++) {
        sonuc[i] = x[i] * x[i]; 
    }
    
    return sonuc;
}