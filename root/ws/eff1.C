#include <iostream>
#include "TH1.h"

Double_t eff1_sum(TH1 *hist, Double_t min, Double_t max) {
  Int_t nmin = hist->FindBin(min + 0.5*hist->GetBinWidth(1));
  Int_t nmax = hist->FindBin(max - 0.5*hist->GetBinWidth(1));
  Double_t sum = hist->Integral(nmin, nmax);
  std::cout << "ch from " << nmin << " to " << nmax << std::endl;
  std::cout << "x  from " << min  << " to " << max  << std::endl;
  std::cout << "sum  " << sum <<std::endl;
  return sum;
}

void eff1(TH1 *hist){
  std::cout <<  "Histogram name: " << hist->GetName() << std::endl;
  
  Double_t sum0 = eff1_sum(hist, 0., 1.);
  Double_t sum1 = eff1_sum(hist, 1., 2.);
  Double_t sum2 = eff1_sum(hist, 2., 3.);
  Double_t sum3 = eff1_sum(hist, 4., 5.);
  Double_t sum4 = eff1_sum(hist, 8., 9.);
  Double_t eff1 = sum0/(sum0+sum1);
  Double_t eff2 = sum0/(sum0+sum2);
  Double_t eff3 = sum0/(sum0+sum3);
  Double_t eff4 = sum0/(sum0+sum4);
  Double_t eff0 = eff1*eff2*eff3*eff4;
  
  std::cout << std::endl;
  std::cout << "Efficiency No.1 "  << eff1 << std::endl;
  std::cout << "Efficiency No.2 "  << eff2 << std::endl;
  std::cout << "Efficiency No.3 "  << eff3 << std::endl;
  std::cout << "Efficiency No.4 "  << eff4 << std::endl;
  std::cout << "Total efficiency " << eff0 << std::endl;
  
  return;
}
