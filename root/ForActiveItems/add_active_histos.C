#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"

void add_active_histos(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TIter next(gBrowserEx->GetListOfOrderedActiveHistos());
  Int_t k = 0;
  gROOT->ProcessLine(".L ../cui/add.C");
  TH1 *hadded = 0;
  TH1 *hist;
  while((hist = (TH1 *)next())){
    if (k==0) {
      hadded = (TH1*)gROOT->ProcessLine(Form("add((TH1*)%p,(TH1*)%p,1.0,0.0);",hist,hist));
    }else{
      hadded->Add(hist);
    }
    k++;
  }
  return;
}
