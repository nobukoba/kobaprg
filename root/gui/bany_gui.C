#include <iostream>
#include <sstream>
#include "TROOT.h"
#include "TFrame.h"
#include "TBox.h"
#include "TGInputDialog.h"
#include "TCanvas.h"
#include "TVirtualPad.h"
#include "TList.h"
#include "TH2.h"

void bany_gui(){
  if (!gPad) {
    std::cout << "There is no gPad. This script is terminated." << std::endl;
    return;
  }
  Double_t x0, y0;
  WaitOneClickX *primi = new WaitOneClickX(&x0, &y0); delete primi;
  
  TList* listofpri = gPad->GetListOfPrimitives();
  TH2* hist = 0;
  TIter next(listofpri); TObject *obj;
  while ((obj = next())){
    if (obj->InheritsFrom("TH2")) {
      hist = (TH2*)obj;
      break;
    }
  }
  if (!hist) {
    std::cout << "TH2 histogram was not found in this pad." << std::endl;
    return;
  }
  TLine line;
  Double_t yrange_min = hist->GetYaxis()->GetBinLowEdge(hist->GetYaxis()->GetFirst());
  Double_t yrange_max = hist->GetYaxis()->GetBinUpEdge(hist->GetYaxis()->GetLast());
  line.DrawLine(x0,yrange_min,x0,yrange_max);
  Double_t x1, y1;
  primi = new WaitOneClickX(&x1, &y1); delete primi;
  line.DrawLine(x1,yrange_min,x1,yrange_max);

  std::cout << std::endl << "Clicked Positions" << std::endl;
  std::cout << "1st (x, y) = (" << x0 << ", " << y0 << ")"<< std::endl;
  std::cout << "2nd (x, y) = (" << x1 << ", " << y1 << ")"<< std::endl;

  if (x0 > x1) {
    Double_t tmpx = x0;
    x0 = x1;
    x1 = tmpx;
  }
  
  gROOT->ProcessLine(".L ../cui/bany.C");
  gROOT->ProcessLine(Form("bany((TH1*)%p,%f,%f)",hist,x0,x1));
  
  return;
}
