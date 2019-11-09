#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"

void unzoom_x_for_active(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TIter next(gBrowserEx->GetListOfOrderedActiveHistos());
  TH1 * hist;
  while((hist = (TH1*)next())){
    hist->GetXaxis()->UnZoom();
  }
  return;
}
