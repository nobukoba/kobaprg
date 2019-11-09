#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TGInputDialog.h"
#include "TH1.h"

void multi_fit_photo_peak_for_active(Double_t x0, Double_t x1){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TGListTree *hist_fListTree = gBrowserEx->GetHistListTree();
  gROOT->ProcessLine(".L fit_photo_peak.C");
  TIter next(gBrowserEx->GetHistListTreeActiveHistos());
  TObjString* objstr;
  while((objstr = (TObjString*)next())){
    TGListTreeItem *cur_ListTreeItem = (TGListTreeItem *) objstr->GetString().Atoll();
    hist_fListTree->DoubleClicked(cur_ListTreeItem,-1);
    gROOT->ProcessLine(Form("fit_photo_peak(%lf,%lf);",x0,x1));
  }
  return;
}

void multi_fit_photo_peak_for_active(){
  char retstr[256];
  new TGInputDialog(gClient->GetRoot(),0,
  		    "Enter range: %f %f",
  		    "0.0 1.0",retstr);
  if(retstr[0] == 0 && retstr[1] == 0){
    std::cout << "Cancel button was pushed. This script is terminated." << std::endl;
    return;
  }
  TString str = retstr;
  str.ReplaceAll(","," ");
  std::istringstream iss(str.Data());
  Double_t par0, par1;
  iss >> par0 >> par1;
  multi_fit_photo_peak_for_active(par0,par1);
  return;
}
