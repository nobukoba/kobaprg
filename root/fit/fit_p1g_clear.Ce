#include <iostream>
#include "TF1.h"
#include "TH1.h"
#include "TGraph.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TList.h"
#include "TLine.h"
#include "TMarker.h"
#include "TString.h"

Bool_t GetSelectedTH1(TH1*& hist, TCanvas*& canvas,
		      TVirtualPad*& sel_pad, TList*& listofpri){
  if (!(canvas = gPad->GetCanvas())) {
    std::cout << "GetSelectedTH1: There is no canvas." << std::endl;
    return false;
  }
  if (!(sel_pad = canvas->GetPad(gPad->GetNumber()))) {
    std::cout << "GetSelectedTH1: There is no selected pad." << std::endl;
    return false;
  }
  if (!(listofpri = sel_pad->GetListOfPrimitives())) {
    std::cout << "GetSelectedTH1: There is nothing in this pad." << std::endl;
    return false;
  }
  TIter next(listofpri); TObject *obj;
  hist = 0;
  while (obj = next()){
    if (obj->InheritsFrom("TH2")) {
      std::cout << "GetSelectedTH1: This script can not handle TH2 histograms." << std::endl;
      return false;
    }
    if (obj->InheritsFrom("TH1")) {
      hist = (TH1*)obj;
      break;
    }
  }
  if (!hist) {
    std::cout << "GetSelectedTH1: TH1 histogram was not found in this pad." << std::endl;
      return false;
    }
  return true;
}

void fit_p1g_clear() {
  TH1* hist; TCanvas* canvas; TVirtualPad* sel_pad; TList* listofpri;
  if (!GetSelectedTH1(hist, canvas, sel_pad, listofpri)) {
    std::cout << "This script is terminated." << std::endl;
    return;
  }
  
  TIter next(listofpri);
  TObject *obj;
  while (obj = next()){
    TString objname = obj->GetName();
    if (objname.EqualTo("TLine")) {
      listofpri->Remove(obj);
    } 
  }
  TList *funclist = hist->GetListOfFunctions();
  if(funclist == 0){
    std::cout << "The GetListOfFunctions() is null. The script is terminated." << std::endl;
    return;
  }
  
  TIter nextfunc(funclist);
  TF1 *funcobj = 0;
  while (funcobj = (TF1*)nextfunc()){
    TString funcname = funcobj->GetName();
    if(funcname.BeginsWith("fit_p1g")){
      funclist->Remove(funcobj);
    }
  }
  sel_pad->Modified();
  sel_pad->Update();
  sel_pad->Update();
  return;
}
