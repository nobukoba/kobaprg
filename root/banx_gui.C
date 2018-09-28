#include <iostream>
#include <sstream>
#include "TROOT.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TVirtualPad.h"
#include "TLine.h"
#include "TList.h"
#include "TH2.h"

Int_t WaitOneClick(Double_t &x, Double_t &y) {
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
    //if(pxold) gVirtualX->DrawLine(pxold,pymin,pxold,pymax);
    if(pyold) gVirtualX->DrawLine(pxmin,pyold,pxmax,pyold);
    if (gPad->GetEvent() == kButton1Down ||
	gPad->GetEvent() == kButton1Up   ||
	gPad->GetEvent() == kMouseLeave) {
      fCrosshairPos = 0;
      continue;
    }
    //gVirtualX->DrawLine(px,pymin,px,pymax);
    gVirtualX->DrawLine(pxmin,py,pxmax,py);
    fCrosshairPos = px + 10000*py;
    gSystem->Sleep(10);
  }
  return 1;
}

void banx_gui(Double_t par0, Double_t par1){
  if (!gPad) {
    std::cout << "There is no gPad. This script is terminated." << std::endl;
    return;
  }
  TVirtualPad *sel_pad = gROOT->GetSelectedPad();
  TList *listofpri = sel_pad->GetListOfPrimitives();
  TIter next(listofpri);
  TObject *obj;
  TH2 *hist = 0;
  while (obj = next()){
    if (obj->InheritsFrom("TH2")) {
      hist = (TH2*)obj;
      std::cout << "TH2 hist was found." << std::endl;
      break;
    }
  }
  if(hist == 0){
    std::cout << "TH2 histogram was not found in this pad. This script is terminated." << std::endl;
    return;
  }
  
  gROOT->cd();
  TString str = hist->GetName();
  str += "_bnx";
  TString str_n = str;
  Int_t num = 1;
  while (gROOT->Get(str_n.Data())) {
    str_n = Form("%s%d",str.Data(),num);
    num++;
  }
  
  TString str2 = hist->GetName();
  str2 += "_cut";
  TString str2_n = str2;
  Int_t num2 = 1;
  while (gROOT->Get(str2_n.Data())) {
    str2_n = Form("%s%d",str2.Data(),num);
    num2++;
  }
  
  TH1D *hout = new TH1D(str_n, hist->GetTitle(), hist->GetNbinsX(),
			hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
  TH2 *hout2 = (TH2*)hist->Clone(str2_n);
  hout2->Reset();
  hout2->SetTitle(hist->GetTitle());
  
  Int_t j1 = hist->GetYaxis()->FindBin(par0);
  Int_t j2 = hist->GetYaxis()->FindBin(par1);
  Double_t binw = hist->GetYaxis()->GetBinWidth(1);
  Double_t factor1 = (hist->GetYaxis()->GetBinUpEdge(j1) - par0)  / binw;
  Double_t factor2 = (par1 - hist->GetYaxis()->GetBinLowEdge(j2)) / binw;
  Double_t factor;
  for (Int_t j = j1; j <= j2; j++) {
    if      (j == j1) {factor = factor1;}
    else if (j == j2) {factor = factor2;}
    else              {factor = 1.;}
    for (Int_t i = 0; i <= hist->GetNbinsX()+1; i++) {
      hout->Fill(hist->GetXaxis()->GetBinCenter(i),
		 hist->GetBinContent(i,j)*factor);
      hout2->Fill(hist->GetXaxis()->GetBinCenter(i),
		  hist->GetYaxis()->GetBinCenter(j),
		  hist->GetBinContent(i,j)*factor);
    }
  }
  hout->Draw();
  sel_pad->GetFrame()->SetBit(TBox::kCannotMove);
  sel_pad->Update();
  return;
}

void banx_gui(){
  if (!gPad) {
    std::cout << "There is no gPad. This script is terminated." << std::endl;
    return;
  }
  Double_t x0, y0;
  if (!WaitOneClick(x0, y0)){
    std::cout << "Can not get point. Exit." << std::endl;
    return;
  }
  TVirtualPad *sel_pad  = gROOT->GetSelectedPad();
  TList* listofpri = sel_pad->GetListOfPrimitives();
  TH2* hist = 0;
  TIter next(listofpri); TObject *obj;
  while (obj = next()){
    if (obj->InheritsFrom("TH2")) {
      hist = (TH2*)obj;
      break;
    }
  }
  if (!hist) {
    std::cout << "TH2 histogram was not found in this pad." << std::endl;
    return;
  }
  TLine line;
  Double_t xrange_min = hist->GetXaxis()->GetBinLowEdge(hist->GetXaxis()->GetFirst());
  Double_t xrange_max = hist->GetXaxis()->GetBinUpEdge(hist->GetXaxis()->GetLast());
  line.DrawLine(xrange_min,y0,xrange_max,y0);
  Double_t x1, y1;
  if (!WaitOneClick(x1, y1)){
    std::cout << "Can not get point. Exit." << std::endl;
    return;
  }
  line.DrawLine(xrange_min,y1,xrange_max,y1);
  std::cout << std::endl << "Clicked Positions" << std::endl;
  std::cout << "1st (x, y) = (" << x0 << ", " << y0 << ")"<< std::endl;
  std::cout << "2nd (x, y) = (" << x1 << ", " << y1 << ")"<< std::endl;

  if (y0 > y1) {
    Double_t tmpy = y0;
    y0 = y1;
    y1 = tmpy;
  }
  banx_gui(y0,y1);
  return;
}
