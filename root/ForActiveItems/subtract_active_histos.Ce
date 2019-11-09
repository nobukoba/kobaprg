#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"

void subtract_active_histos(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TList *histos = gBrowserEx->GetListOfOrderedActiveHistos();
  if (histos->GetEntries() <= 1) {
    std::cout << "Entries is less than 2. Exit." << std::endl;
    return;
  }
  TIter next(histos);
  TH1 *hist;
  TH1 *subtracted = 0;
  gROOT->cd();
  while((hist = (TH1 *)next())){
    if (subtracted == 0) {
      TString str = hist->GetName();
      str += "_sub";
      TString str_n = str;
      Int_t num = 1;
      while (gROOT->Get(str_n.Data())) {
        str_n = Form("%s%d",str.Data(),num);
        num++;
      }
      subtracted = (TH1*) hist->Clone(str_n);
      subtracted->SetTitle(hist->GetTitle());
    }else{
      subtracted->Add(hist,-1);
    }
  }
  return;
}
