#include <iostream>
#include <sstream>
#include "TROOT.h"
#include "TGInputDialog.h"
#include "TCanvas.h"
#include "TVirtualPad.h"
#include "TList.h"
#include "TH2.h"

void banx_gui(Double_t par0, Double_t par1){
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
  sel_pad->Update();
  return;
}

void banx_gui(){
  TCanvas* canvas;
  if (!(canvas = gPad->GetCanvas())) {
    std::cout << "There is no canvas." << std::endl;
    return;
  }
  gPad->SetCrosshair();
  TMarker *mk = (TMarker*)canvas->WaitPrimitive("TMarker","Marker");
  Double_t x0 = mk->GetX();
  Double_t y0 = mk->GetY();
  delete mk;
  TVirtualPad *sel_pad  = canvas->GetSelectedPad();
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
    gPad->SetCrosshair(0);
    return;
  }
  TLine line;
  Double_t xrange_min = hist->GetXaxis()->GetBinLowEdge(hist->GetXaxis()->GetFirst());
  Double_t xrange_max = hist->GetXaxis()->GetBinUpEdge(hist->GetXaxis()->GetLast());
  line.DrawLine(xrange_min,y0,xrange_max,y0);
  mk = (TMarker*)canvas->WaitPrimitive("TMarker","Marker");
  Double_t x1 = mk->GetX();
  Double_t y1 = mk->GetY();
  line.DrawLine(xrange_min,y1,xrange_max,y1);
  delete mk;
  gPad->SetCrosshair(0);

  std::cout << std::endl << "Clicked Position" << std::endl;
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
