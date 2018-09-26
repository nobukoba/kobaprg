#include <iostream>
#include <sstream>
#include "TROOT.h"
#include "TGInputDialog.h"
#include "TCanvas.h"
#include "TVirtualPad.h"
#include "TList.h"
#include "TH2.h"

void cut_xy_gui(Double_t x1, Double_t x2, Double_t y1, Double_t y2){
  TCanvas* canvas = gPad->GetCanvas();
  if (canvas == 0) {
    std::cout << "There is no canvas. This script is terminated." << std::endl;
    return;
  }
  TVirtualPad *sel_pad = canvas->GetPad(gPad->GetNumber());
  if (sel_pad == 0) {
    std::cout << "There is no sel_pad. This script is terminated." << std::endl;
    return;
  }
  TList *listofpri = sel_pad->GetListOfPrimitives();
  if (listofpri == 0) {
    std::cout << "The pad includes nothing. This script is terminated." << std::endl;
    return;
  }
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
  str += "_cut";
  TString str_n = str;
  Int_t num = 1;
  while (gROOT->Get(str_n.Data())) {
    str_n = Form("%s%d",str.Data(),num);
    num++;
  }

  TH2D *hout = hist->Clone(str_n);
  hout->Reset();
  hout->SetTitle(hist->GetTitle());
  Int_t i1 = hist->GetXaxis()->FindBin(x1);
  Int_t i2 = hist->GetXaxis()->FindBin(x2);
  Int_t j1 = hist->GetYaxis()->FindBin(y1);
  Int_t j2 = hist->GetYaxis()->FindBin(y2);
  Double_t xbinw = hist->GetXaxis()->GetBinWidth(1);
  Double_t ybinw = hist->GetYaxis()->GetBinWidth(1);
  Double_t xfactor1 = (hist->GetXaxis()->GetBinUpEdge(i1) - x1)  / xbinw;
  Double_t xfactor2 = (x2 - hist->GetXaxis()->GetBinLowEdge(i2)) / xbinw;
  Double_t yfactor1 = (hist->GetYaxis()->GetBinUpEdge(j1) - y1)  / ybinw;
  Double_t yfactor2 = (y2 - hist->GetYaxis()->GetBinLowEdge(j2)) / ybinw;
  Double_t xfactor, yfactor;
  for (Int_t i = i1; i <= i2; i++) {
    if      (i == i1) {xfactor = xfactor1;}
    else if (i == i2) {xfactor = xfactor2;}
    else              {xfactor = 1.;}
    for (Int_t j = j1; j <= j2; j++) {
      if      (j == j1) {yfactor = yfactor1;}
      else if (j == j2) {yfactor = yfactor2;}
      else              {yfactor = 1.;}
      hout->Fill(hist->GetXaxis()->GetBinCenter(i),
		 hist->GetYaxis()->GetBinCenter(j),
		 hist->GetBinContent(i,j)*xfactor*yfactor);
    }
  }
  hout->Draw("colz");
  sel_pad->Update();
  return;
}

void cut_xy_gui(){
  TCanvas* canvas;
  if (!(canvas = gPad->GetCanvas())) {
    std::cout << "There is no canvas." << std::endl;
    return;
  }
  gPad->SetCrosshair();
  TMarker *mk = (TMarker*)canvas->WaitPrimitive("TMarker","Marker");
  Double_t x1 = mk->GetX();
  Double_t y1 = mk->GetY();
  delete mk;
  TVirtualPad *sel_pad  = gROOT->GetSelectedPad();
  TList* listofpri= sel_pad->GetListOfPrimitives();
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
    gPad->SetCrosshair(0);
    return;
  }
  TLine line;
  Double_t xrange_min = hist->GetXaxis()->GetBinLowEdge(hist->GetXaxis()->GetFirst());
  Double_t xrange_max = hist->GetXaxis()->GetBinUpEdge(hist->GetXaxis()->GetLast());
  line.DrawLine(xrange_min,y1,xrange_max,y1);
  line.DrawLine(x1,hist->GetMinimum(),x1,hist->GetMaximum());
  mk = (TMarker*)canvas->WaitPrimitive("TMarker","Marker");
  Double_t x2 = mk->GetX();
  Double_t y2 = mk->GetY();
  line.DrawLine(xrange_min,y2,xrange_max,y2);
  line.DrawLine(x2,hist->GetMinimum(),x2,hist->GetMaximum());
  delete mk;
  gPad->SetCrosshair(0);

  std::cout << std::endl << "Clicked Position" << std::endl;
  std::cout << "1st (x, y) = (" << x1 << ", " << y1 << ")"<< std::endl;
  std::cout << "2nd (x, y) = (" << x2 << ", " << y2 << ")"<< std::endl;

  if (x1 > x2) {
    Double_t tmpx = x2;
    x1 = x2;
    x2 = tmpx;
  } 
  if (y1 > y2) {
    Double_t tmpy = y2;
    y1 = y2;
    y2 = tmpy;
  }
  cut_xy_gui(x1, x2, y1, y2);
  return;
}
