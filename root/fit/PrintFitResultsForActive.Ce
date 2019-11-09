#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"
void PrintFitResultsForActive(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TIter next(gBrowserEx->GetListOfOrderedActiveHistos());
  TH1 * hist;
  while((hist = (TH1*)next())){
    TList *funclist = hist->GetListOfFunctions();
    if(funclist == 0){
      //std::cout << "The GetListOfFunctions() is null. The script is terminated." << std::endl;
      continue;
    }
    Int_t j = 0;
    TF1 *funcobj = 0;
    //while (funcobj = (TF1*)funclist->FindObject(Form("fit_p1g_%d",j))) {
    while (funcobj = (TF1*)funclist->FindObject(Form("photo_peak_fit_%d",j))) {
      if (j == 0) {
        std::cout << "HistName: " << hist->GetName();
      }
      std::cout << ", " << funcobj->GetParameter(1);
      j++;
    }
    if (j>0) {
      std::cout << std::endl;
    }
  }
  return;
}
