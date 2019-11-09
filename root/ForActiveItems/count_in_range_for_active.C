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
  TList *histos = gBrowserEx->GetListOfOrderedActiveHistos();
  TIter next(histos);
  TH1 *hist;
  while((hist = (TH1 *)next())){
    std::cout << hist->GetName() << ": ";
    std::cout << hist->Integral(hist->GetXaxis()->GetFirst(),
                                hist->GetXaxis()->GetLast()) << std::endl;
  }
  return;
}
