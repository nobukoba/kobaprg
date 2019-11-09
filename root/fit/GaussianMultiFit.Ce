#include <iostream>
#include "TH1.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TList.h"
#include "TLatex.h"
#include "TMath.h"
#include "TF1.h"

Double_t func(Double_t *dim, Double_t *par){
  Double_t x      = dim[0];
  Double_t height = par[0];
  Double_t cent   = par[1];
  Double_t sigma  = par[2];
  return height*TMath::Gaus(x,cent,sigma);
}

void GaussianMultiFit(){
  TCanvas* canvas = gPad->GetCanvas();
  gPad->SetCrosshair();
  TMarker *mk = (TMarker*)canvas->WaitPrimitive("TMarker","Marker");
  Double_t x0 = mk->GetX();
  delete mk;
  TVirtualPad *sel_pad = gROOT->GetSelectedPad();
  TList *listofpri = sel_pad->GetListOfPrimitives();
  TIter next(listofpri);
  TObject *obj;
  TH1 *hist = 0;
  while (obj = next()){
    if (obj->InheritsFrom("TH2")) {
      std::cout << "This script can not handle TH2 histograms." << std::endl;
      return;
    }
    if (obj->InheritsFrom("TH1")) {
      hist = (TH1*)obj;
      break;
    }
  }
  if(hist == 0){
    std::cout << "TH1 histogram was not found in this pad." << std::endl;
    return;
  }
  TLine line;
  line.DrawLine(x0,hist->GetMinimum(),x0,hist->GetMaximum());
  mk = (TMarker*)canvas->WaitPrimitive("TMarker","Marker");
  Double_t x1 = mk->GetX();
  line.DrawLine(x1,hist->GetMinimum(),x1,hist->GetMaximum());
  delete mk;
  gPad->SetCrosshair(0);

  if (x0 > x1) {
    Double_t tmpx = x0;
    x0 = x1;
    x1 = tmpx;
  }
  
  TF1 *f = new TF1("gaus", func, x0, x1, 3);
  f->SetParameter(0,100.);
  f->SetParameter(1,(x0+x1)/2.);
  f->SetParameter(2, 10.);
  hist->Fit(f, "R+");
  
  Double_t constant = f->GetParameter(0);
  Double_t mean     = f->GetParameter(1);
  Double_t sigma    = f->GetParameter(2);
  Double_t fwhm     = sigma * 2.355;
  Double_t integ_area   = constant*TMath::Sqrt(2*TMath::Pi());
  Double_t integ_counts = integ_area/(hist->GetXaxis()->GetBinWidth(1));
  printf("\n");
  printf("  FWHM          : %g\n",fwhm);
  printf("  Integ. area   : %g\n",integ_area);
  printf("  Integ. counts : %g\n",integ_counts);
  
  Double_t kev_per_ch = 0.;
  //kev_per_ch = 10.; // Modify here
  
  TString str;
  str += TString::Format("#splitline{Integ. cnt:  %.2g}",integ_counts);
  if(kev_per_ch > 0){
    str += TString::Format("{#splitline{Energy Resolution:}");
    str += TString::Format("{%.1f keV (FWHM)}}",fwhm*kev_per_ch);
  }else{
    str += TString::Format("{FWHM: %g}",fwhm);
  }
  
  TLatex *prev_tlatex_pnt = (TLatex *)listofpri->FindObject("p_latex_GaussianFit");
  if (prev_tlatex_pnt) {
    listofpri->Remove(prev_tlatex_pnt);
  }
  TLatex tlatex_obj;
  tlatex_obj.SetTextColor(kRed);
  tlatex_obj.SetTextSize(0.05);
  tlatex_obj.SetTextAlign(13);
  TLatex *tlatex_pnt = tlatex_obj.DrawLatexNDC(0.14,0.88,str);
  tlatex_pnt->SetName("p_latex_GaussianFit");
  //printf("tlatex_pnt : %s\n",tlatex_pnt->GetName());
  gPad->Modified();
  gPad->Update();
  gPad->Update();
  return;
}
