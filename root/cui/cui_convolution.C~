#include <vector>
#include "TROOT.h"
#include "TH1.h"
#include "TMath.h"

void cui_convolution(TH1* hin) {
  double sigma=10.; // sigma of Gaussian
  TH1D* hout = (TH1D*)hin->Clone("convoluted"); // convoluted function with Gaussian
  
  double dx = hin->GetBinWidth(1);
  for (unsigned int i=1; i <= hin->GetNbinsX(); i++) {
    double fout = 0.;
    for (unsigned int j=1; j <= hin->GetNbinsX(); j++) {
      fout = fout + 0.5*fin[j]
	*(TMath::Erf((xin[i]-(xin[j]-0.5*dx))/(sqrt(2.)*sigma))
	  -TMath::Erf((xin[i]-(xin[j]+0.5*dx))/(sqrt(2.)*sigma)));
    }
    hout->SetBinContent(i, fout);
  }
  return;
}
