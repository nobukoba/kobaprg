#include <iostream>
#include <sstream>
#include "TROOT.h"
#include "TGInputDialog.h"
#include "TVirtualPad.h"
#include "TList.h"
#include "TH2.h"

void banx(Double_t par0, Double_t par1){
  if (!gPad) {
    std::cout << "There is no gPad." << std::endl;
    return 0;
  }
  TVirtualPad *sel_pad = gROOT->GetSelected();
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
  gPad->GetFrame()->SetBit(TBox::kCannotMove);
  sel_pad->Update();
  return;
}

void banx(){
  char retstr[256];
  new TGInputDialog(gClient->GetRoot(),0,
                    "Range: %f %f",
                    "0.0 1.0",retstr);
  if(retstr[0] == 0 && retstr[1] == 0){
    std::cout << "Cancel button was pushed. This script is terminated." << std::endl;
    return;
  }
  TString str = retstr;
  str.ReplaceAll(","," ");
  std::istringstream iss(str);
  Double_t par0, par1;
  iss >> par0 >> par1;
  banx(par0,par1);
  return;
}
