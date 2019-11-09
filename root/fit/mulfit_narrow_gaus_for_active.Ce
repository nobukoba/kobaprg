#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TGInputDialog.h"
#include "TH1.h"

void mulfit_narrow_gaus_for_active(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TGListTree *hist_fListTree = gBrowserEx->GetHistListTree();
  gROOT->ProcessLine(".L fit_narrow_gaus.C");
  TIter next(gBrowserEx->GetHistListTreeActiveHistos());
  TObjString* objstr;
  while((objstr = (TObjString*)next())){
    TGListTreeItem *cur_ListTreeItem = (TGListTreeItem *) objstr->GetString().Atoll();
    hist_fListTree->DoubleClicked(cur_ListTreeItem,-1);
    gROOT->ProcessLine("fit_narrow_gaus();");
  }
  return;
}
