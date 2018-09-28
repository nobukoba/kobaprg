#include <iostream>
#include <sstream>
#include "TROOT.h"
#include "TGInputDialog.h"
#include "TCanvas.h"
#include "TVirtualPad.h"
#include "TList.h"
#include "TH2.h"

void cut_xy_gui(Double_t x1, Double_t x2, Double_t y1, Double_t y2){
  if (!gPad) {
    std::cout << "There is no gPad. This script is terminated." << std::endl;
    return;
  }
  TList *listofpri = gPad->GetListOfPrimitives();
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
  gPad->Update();
  gPad->GetFrame()->SetBit(TBox::kCannotMove);
  return;
}

void cut_xy_gui(){
  if (!gPad) {
    std::cout << "There is no gPad. This script is terminated." << std::endl;
    return;
  }
  gPad->SetCrosshair();
  TCanvas* canvas;
  if (!(canvas = gPad->GetCanvas())) {
    std::cout << "There is no canvas." << std::endl;
    return;
  }
  TMarker *mk = (TMarker*)canvas->WaitPrimitive("TMarker","Marker");
  Double_t x0 = mk->GetX();
  Double_t y0 = mk->GetY();
  delete mk;
  TList* listofpri = gPad->GetListOfPrimitives();
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
  line.DrawLine(x0,hist->GetMinimum(),x0,hist->GetMaximum());
  mk = (TMarker*)canvas->WaitPrimitive("TMarker","Marker");
  gPad->SetCrosshair(0);
  Double_t x1 = mk->GetX();
  Double_t y1 = mk->GetY();
  delete mk;
  line.DrawLine(xrange_min,y1,xrange_max,y1);
  line.DrawLine(x1,hist->GetMinimum(),x1,hist->GetMaximum());

  std::cout << std::endl << "Clicked Position" << std::endl;
  std::cout << "1st (x, y) = (" << x0 << ", " << y0 << ")"<< std::endl;
  std::cout << "2nd (x, y) = (" << x1 << ", " << y1 << ")"<< std::endl;

  if (x0 > x1) {
    Double_t tmpx = x1;
    x0 = x1;
    x1 = tmpx;
  } 
  if (y0 > y1) {
    Double_t tmpy = y1;
    y0 = y1;
    y1 = tmpy;
  }
  cut_xy_gui(x0, x1, y0, y1);
  return;
}
