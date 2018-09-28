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

Int_t WaitOneClickX(Double_t &x, Double_t &y) {
  if (!gPad) {
    std::cout << "There is no gPad." << std::endl;
    return 0;
  }
  TCanvas* canvas = gPad->GetCanvas();
  Int_t fCrosshairPos = 0;
  Int_t event = 0;
  while (!gSystem->ProcessEvents() && gROOT->GetSelectedPad()) {
    event = gPad->GetEvent();
    //  std::cout << "event: " << event << std::endl;
    if (event == kButton1Down) {
      x = gPad->AbsPixeltoX(gROOT->GetSelectedPad()->GetEventX());
      y = gPad->AbsPixeltoY(gROOT->GetSelectedPad()->GetEventY());
      canvas->HandleInput((EEventType)-1,0,0);
      break;
    }
    if (gPad->GetEvent() == kMouseEnter) continue;
    canvas->FeedbackMode(kTRUE);
    //erase old position and draw a line at current position
    Int_t pxmin,pxmax,pymin,pymax,pxold,pyold,px,py;
    pxold = fCrosshairPos%10000;
    pyold = fCrosshairPos/10000;
    px    = gPad->GetEventX();
    py    = gPad->GetEventY()+1;
    pxmin = 0;
    pxmax = canvas->GetWw();
    pymin = 0;
    pymax = gPad->GetWh();
    if(pxold) gVirtualX->DrawLine(pxold,pymin,pxold,pymax);
    //if(pyold) gVirtualX->DrawLine(pxmin,pyold,pxmax,pyold);
    if (gPad->GetEvent() == kButton1Down ||
	gPad->GetEvent() == kButton1Up   ||
	gPad->GetEvent() == kMouseLeave) {
      fCrosshairPos = 0;
      continue;
    }
    gVirtualX->DrawLine(px,pymin,px,pymax);
    //gVirtualX->DrawLine(pxmin,py,pxmax,py);
    fCrosshairPos = px + 10000*py;
    gSystem->Sleep(10);
  }
  return 1;
}

void fit_p1g() {
  if (!gPad) {
    std::cout << "There is no gPad. This script is terminated." << std::endl;
    return;
  }
  Double_t x0, y0;
  if (!WaitOneClickX(x0, y0)){
    std::cout << "Can not get point. Exit." << std::endl;
    return;
  }
  TVirtualPad *sel_pad  = gROOT->GetSelectedPad();
  TList* listofpri = sel_pad->GetListOfPrimitives();
  TH1* hist = 0;
  TIter next(listofpri); TObject *obj;
  while (obj = next()){
    if (obj->InheritsFrom("TH2")) {
      std::cout << "This script can not handle TH2 histograms." << std::endl;
      gPad->SetCrosshair(0);
      return;
    }
    if (obj->InheritsFrom("TH1")) {
      hist = (TH1*)obj;
      break;
    }
  }
  if (!hist) {
    std::cout << "TH1 histogram was not found in this pad." << std::endl;
    gPad->SetCrosshair(0);
    return;
  }
  Double_t x1, y1;
  if (!WaitOneClickX(x1, y1)){
    std::cout << "Can not get point. Exit." << std::endl;
    return;
  }
  TLine line;
  line.DrawLine(x0,hist->GetMinimum(),x0,hist->GetMaximum());
  line.DrawLine(x1,hist->GetMinimum(),x1,hist->GetMaximum());

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
  sel_pad->Update();
  sel_pad->Modified();
  return;
}
