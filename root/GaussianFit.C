#include "TH1.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TList.h"
#include "TLatex.h"
#include "TMath.h"
#include "TF1.h"

TH1 * GaussianFit(){
  TCanvas* canvas = gPad->GetCanvas();
  TVirtualPad *sel_pad = canvas->GetPad(gPad->GetNumber());
  if (sel_pad == 0) {return 0;}
  TList * list = sel_pad->GetListOfPrimitives();
  if (list == 0) {return 0;}
  TH1 *hist = (TH1*) list->At(1);
  if (hist == 0) {return 0;}
  if (hist->InheritsFrom("TH2")) {
    printf("This script can not handle TH2 histograms.\n");
    return 0;
  }
  if (hist->InheritsFrom("TH1") == 0) {return 0;}
  Double_t lw = hist->GetXaxis()->GetBinLowEdge(hist->GetXaxis()->GetFirst());
  Double_t up = hist->GetXaxis()->GetBinUpEdge(hist->GetXaxis()->GetLast());
  hist->Fit("gaus", "R", "", lw, up);
  TF1 * gaus = hist->GetFunction("gaus");
  Double_t constant = gaus->GetParameter(0);
  Double_t mean     = gaus->GetParameter(1);
  Double_t sigma    = gaus->GetParameter(2);
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
  
  TLatex *prev_tlatex_pnt = (TLatex *)list->FindObject("p_latex_GaussianFit");
  if (prev_tlatex_pnt) {
    prev_tlatex_pnt->Delete();
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
  return hist;
}
