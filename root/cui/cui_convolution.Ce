#include "TROOT.h"
#include "TH1.h"
#include "TMath.h"

void cui_convolution(TH1* hin, Double_t par) {
  TH1* hout = (TH1*)hin->Clone("convoluted"); // convoluted function with Gaussian
  Double_t  dx = hin->GetBinWidth(1);
  for (Int_t i=1; i <= hin->GetNbinsX(); i++) {
    Double_t fout = 0.;
    Double_t x_i = hin->GetBinCenter(i);
    for (Int_t j=1; j <= hin->GetNbinsX(); j++) {
      Double_t x_j = hin->GetBinCenter(j);
      //Double_t sigma = par*sqrt(x_j);
      Double_t sigma = par;
      fout = fout + 0.5*hin->GetBinContent(j)
	*(TMath::Erf((x_i-(x_j-0.5*dx))/(sqrt(2.)*sigma))
	  -TMath::Erf((x_i-(x_j+0.5*dx))/(sqrt(2.)*sigma)));
    }
    hout->SetBinContent(i, fout);
  }
  return;
}
