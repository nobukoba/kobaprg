#include <iostream>
#include <sstream>
#include "TROOT.h"
#include "TGInputDialog.h"
#include "TCanvas.h"
#include "TVirtualPad.h"
#include "TList.h"
#include "TH2.h"

void bany_gui(Double_t par0, Double_t par1){
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
  str += "_bny";
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
  
  TH1D *hout = new TH1D(str_n, hist->GetTitle(), hist->GetNbinsY(),
			hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax());
  TH2 *hout2 = (TH2*)hist->Clone(str2_n);
  hout2->Reset();
  hout2->SetTitle(hist->GetTitle());

  Int_t i1 = hist->GetXaxis()->FindBin(par0);
  Int_t i2 = hist->GetXaxis()->FindBin(par1);
  Double_t binw = hist->GetXaxis()->GetBinWidth(1);
  Double_t factor1 = (hist->GetXaxis()->GetBinUpEdge(i1) - par0)  / binw;
  Double_t factor2 = (par1 - hist->GetXaxis()->GetBinLowEdge(i2)) / binw;
  Double_t factor;
  for (Int_t i = i1; i <= i2; i++) {
    if      (i == i1) {factor = factor1;}
    else if (i == i2) {factor = factor2;}
    else              {factor = 1.;}
    for (Int_t j = 0; j <= hist->GetNbinsY()+1; j++) {
      hout->Fill(hist->GetYaxis()->GetBinCenter(j),
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

void bany_gui(){
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
  TVirtualPad *sel_pad = gROOT->GetSelectedPad();
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
  line.DrawLine(x0,hist->GetMinimum(),x0,hist->GetMaximum());
  mk = (TMarker*)canvas->WaitPrimitive("TMarker","Marker");
  Double_t x1 = mk->GetX();
  Double_t y1 = mk->GetY();
  line.DrawLine(x1,hist->GetMinimum(),x1,hist->GetMaximum());
  delete mk;
  gPad->SetCrosshair(0);

  std::cout << std::endl << "Clicked Position" << std::endl;
  std::cout << "1st (x, y) = (" << x0 << ", " << y0 << ")"<< std::endl;
  std::cout << "2nd (x, y) = (" << x1 << ", " << y1 << ")"<< std::endl;

  if (x0 > x1) {
    Double_t tmpx = x0;
    x0 = x1;
    x1 = tmpx;
  }

  bany(x0,x1);
  return;
}
