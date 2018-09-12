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

Bool_t GetSelectedTH1(TH1*& hist, TCanvas*& canvas,
		      TVirtualPad*& sel_pad, TList*& listofpri){
  if (!(canvas = gPad->GetCanvas())) {
    std::cout << "GetSelectedTH1: There is no canvas." << std::endl;
    return false;
  }
  if (!(sel_pad = canvas->GetPad(gPad->GetNumber()))) {
    std::cout << "GetSelectedTH1: There is no selected pad." << std::endl;
    return false;
  }
  if (!(listofpri = sel_pad->GetListOfPrimitives())) {
    std::cout << "GetSelectedTH1: There is nothing in this pad." << std::endl;
    return false;
  }
  TIter next(listofpri); TObject *obj;
  hist = 0;
  while (obj = next()){
    if (obj->InheritsFrom("TH2")) {
      std::cout << "GetSelectedTH1: This script can not handle TH2 histograms." << std::endl;
      return false;
    }
    if (obj->InheritsFrom("TH1")) {
      hist = (TH1*)obj;
      break;
    }
  }
  if (!hist) {
    std::cout << "GetSelectedTH1: TH1 histogram was not found in this pad." << std::endl;
      return false;
    }
  return true;
}

void fit_p1g() {
  TH1* hist; TCanvas* canvas; TVirtualPad* sel_pad; TList* listofpri;
  if (!GetSelectedTH1(hist, canvas, sel_pad, listofpri)) {
    std::cout << "This script is terminated." << std::endl;
    return;
  }
  
  gPad->SetCrosshair();
  TMarker *mk = (TMarker*)sel_pad->WaitPrimitive("TMarker","Marker");
  Double_t x0 = mk->GetX();
  delete mk;
  TLine line;
  line.DrawLine(x0,hist->GetMinimum(),x0,hist->GetMaximum());
  mk = (TMarker*)sel_pad->WaitPrimitive("TMarker","Marker");
  Double_t x1 = mk->GetX();
  line.DrawLine(x1,hist->GetMinimum(),x1,hist->GetMaximum());
  delete mk;
  gPad->SetCrosshair(0);

  if (x0 > x1) {
    Double_t tmpx = x0;
    x0 = x1;
    x1 = tmpx;
  }

  Double_t y0 = hist->GetBinContent(hist->GetBin(x0));
  Double_t y1 = hist->GetBinContent(hist->GetBin(x1));
  if (y0 == 0.){y0 = 1.;}
  if (y1 == 0.){y1 = 1.;}
  
  Double_t bwid  = hist->GetBinWidth(1);
  Double_t xrang = x1 - x0;

  TList *funclist = hist->GetListOfFunctions();
  Int_t j = 0;
  while(funclist->FindObject(Form("fit_p1g_%d",j))){
    j++;
  }

  TF1* fgaus = new TF1(Form("fit_p1g_init_gaus_%d",j), "gaus", x0+0.25*xrang, x1-0.25*xrang);
  Double_t ymax = hist->GetBinContent(hist->GetMaximumBin());
  fgaus->SetParLimits(0, 0., ymax*10000.);
  fgaus->SetParLimits(1, x0, x1);
  fgaus->SetParLimits(2, xrang/10., xrang);
  hist->Fit(fgaus,"R+");

  Double_t par[5];
  par[0] = (y0*x1-y1*x0)/(x1-x0);
  par[1] = (y1-y0)/(x1-x0);
  //par[0] = (log(y0)*x1-log(y1)*x0)/(x1-x0);
  //par[1] = (log(y1)-log(y0))/(x1-x0);
  par[2] = fgaus->GetParameter(0);
  par[3] = fgaus->GetParameter(1);
  par[4] = fgaus->GetParameter(2);
  funclist->Last()->Delete();
  
  TF1* fit_p1g = new TF1(Form("fit_p1g_%d",j),"pol1(0)+gaus(2)",x0,x1);
  fit_p1g->SetParameters(&(par[0]));
  fit_p1g->SetParName(0,"p0");
  fit_p1g->SetParName(1,"p1");
  fit_p1g->SetParName(2,"Constant");
  fit_p1g->SetParName(3,"Mean");
  fit_p1g->SetParName(4,"Sigma");
  fit_p1g->SetLineWidth(1);
  hist->Fit(fit_p1g,"R+");
  return;
}
