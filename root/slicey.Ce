#include <iostream>
#include <sstream>
#include "TROOT.h"
#include "TGInputDialog.h"
#include "TCanvas.h"
#include "TVirtualPad.h"
#include "TList.h"
#include "TMath.h"
#include "TH2.h"

void slicey(){
  if (!gPad) {
    std::cout << "There is no gPad." << std::endl;
    return;
  }
  TList *listofpri = gPad->GetListOfPrimitives();
  TIter next(listofpri);
  TObject *obj;
  TH2 *hist = 0;
  while ((obj = next())){
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
  str += "_sly_";
  TString str_n = str;
  Int_t num = 1;
  TList *groot_list = gROOT->GetList();
  while (1) {
    TIter next(groot_list);
    TObject *obj;
    Int_t found = 0;
    while ((obj = next())) {
      if (!(obj->InheritsFrom("TH1"))) {
	continue;
      } 
      TString name = obj->GetName();
      if (name.BeginsWith(str_n)){
	found = 1;
	break;
      }
    }
    if (found == 1) {
      str_n = Form("%s_sly%d_",hist->GetName(),num);
      num++;
    }else{
      break;
    }
  }
  Int_t nbins = hist->GetNbinsX();
  Int_t ndig = 1;
  while (nbins > (TMath::Power(10, ndig)-1)) {
    ndig++;
  }
  for (Int_t i = 1; i <= hist->GetNbinsX(); i++) {
    TString formatstr = Form("%s%%0%dd",str_n.Data(),ndig);
    TString hname = Form(formatstr.Data(),i);
    TH1D *hout = new TH1D(hname, hist->GetTitle(), hist->GetNbinsY(),
			  hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax());
    for (Int_t j = 0; j <= hist->GetNbinsY()+1; j++) {
      hout->Fill(hist->GetYaxis()->GetBinCenter(j),
		 hist->GetBinContent(i,j));
    }
  }
  return;
}
