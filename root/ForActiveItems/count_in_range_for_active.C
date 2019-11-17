#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"

void count_in_range_for_active(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TIter next(gBrowserEx->GetListOfOrderedActiveHistos());
  TH1 *hist;
  gROOT->ProcessLine(".L ../cui/count_in_range.C");
  while((hist = (TH1 *)next())){
    gROOT->ProcessLine(Form("count_in_range((TH1*)%p)",hist));
  }
  return;
}
