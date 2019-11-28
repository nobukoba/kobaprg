#include <iostream>
#include "TF1.h"
#include "TH1.h"
#include "TGraph.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TList.h"
#include "TLine.h"
#include "TMarker.h"
#include "TString.h"

void fit_narrow_gaus(Double_t x0, Double_t x1) {
  if (!gPad) {
    std::cout << "There is no gPad. This script is terminated." << std::endl;
    return;
  }
  TList* listofpri = gPad->GetListOfPrimitives();
  TH1* hist = 0;
  TIter next(listofpri); TObject *obj;
  while ((obj = next())){
    if (obj->InheritsFrom("TH2")) {
      std::cout << "This script can not handle TH2 histograms." << std::endl;
      return;
    }
    if (obj->InheritsFrom("TH1")) {
      hist = (TH1*)obj;
      break;
    }
  }
  if (!hist) {
    std::cout << "TH1 histogram was not found in this pad." << std::endl;
    return;
  }

  if (x0 > x1) {
    Double_t tmpx = x0;
    x0 = x1;
    x1 = tmpx;
  }
  
  Double_t ymax   = hist->GetMaximum();
  Double_t maxbin = hist->GetMaximumBin();
  Double_t maxcen = hist->GetBinCenter(maxbin);

  std::cout << "hist->GetMaximum(): " << hist->GetMaximum() << std::endl;
  std::cout << "hist->GetMaximumBin(): " << hist->GetMaximumBin() << std::endl;
  std::cout << "maxcen: " << maxcen << std::endl;
  
  Double_t bwid  = hist->GetBinWidth(maxbin);
  Double_t xlow = maxcen - 5*bwid;
  Double_t xup  = maxcen + 5*bwid;
  Double_t xrang = xup - xlow;
  
  TList *funclist = hist->GetListOfFunctions();
  Int_t j = 0;
  while(funclist->FindObject(Form("fit_narrow_gaus_%d",j))){
    j++;
  }
  
  TF1* fgaus = new TF1(Form("fit_narrow_gaus_init_%d",j), "gaus", xlow, xup);
  Double_t ymax = hist->GetBinContent(hist->GetMaximumBin());
  fgaus->SetParLimits(0, 0., ymax*10000.);
  fgaus->SetParLimits(1, xlow, xup);
  fgaus->SetParLimits(2, xrang/10., xrang);
  hist->Fit(fgaus,"R+");
  
  Double_t par[3];
  par[0] = fgaus->GetParameter(0);
  par[1] = fgaus->GetParameter(1);
  par[2] = fgaus->GetParameter(2);
  funclist->Last()->Delete();

  xlow = par[1] - 0.75 * par[2];
  xup  = par[1] + 0.75 * par[2];
  
  TF1* fit_narrow_gaus = new TF1(Form("fit_narrow_gaus_%d",j),"gaus",xlow,xup);
  fit_narrow_gaus->SetLineWidth(1);
  hist->Fit(fit_narrow_gaus,"R+");
  gPad->Update();
  gPad->Modified();
  return;
}

void fit_narrow_gaus() {
  if (!gPad) {
    std::cout << "There is no gPad. This script is terminated." << std::endl;
    return;
  }
  TList* listofpri = gPad->GetListOfPrimitives();
  TH1* hist = 0; TIter next(listofpri); TObject *obj;
  while ((obj = next())){
    if (obj->InheritsFrom("TH2")) {
      std::cout << "This script can not handle TH2 histograms." << std::endl;
      return;
    }
    if (obj->InheritsFrom("TH1")) {
      hist = (TH1*)obj;
      break;
    }
  }
  if (!hist) {
    std::cout << "TH1 histogram was not found in this pad." << std::endl;
    return;
  }
  Double_t x0 = hist->GetXaxis()->GetBinLowEdge(hist->GetXaxis()->GetFirst());
  Double_t x1 = hist->GetXaxis()->GetBinUpEdge(hist->GetXaxis()->GetLast());
  fit_narrow_gaus(x0, x1);
}
