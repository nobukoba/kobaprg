#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"

void scale_active_histos(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  char retstr[256] = "1.0";
  TString disstr = "Please enter scale for the all histos";
  new TGInputDialog(gClient->GetRoot(),0,
		    disstr.Data(),retstr,retstr);
  if(retstr[0] == 0 && retstr[1] == 0){
    std::cout << "Cancel button was pushed. This script is terminated." << std::endl;
    return;
  }
  std::istringstream iss(retstr.Data());
  Double_t par;
  iss >> par;
  TIter next(gBrowserEx->GetListOfOrderedActiveHistos());
  TH1 *hist;
  gROOT->cd();
  while((hist = (TH1 *)next())){
    TString str = hist->GetName();
    str += "_scl";
    TString str_n = str;
    Int_t num = 1;
    while (gROOT->Get(str_n.Data())) {
      str_n = Form("%s%d",str.Data(),num);
      num++;
    }
    TH1 *scl = (TH1*) hist->Clone(str_n);
    scl->Scale(par);
  }
  return;
}
