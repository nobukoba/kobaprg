#include "TCanvas.h"

void lny(){
  opt_liny();
}

void opt_liny(){
  TCanvas* canvas = gPad->GetCanvas();
  gStyle->SetOptLogy(0);
  canvas->UseCurrentStyle();
  canvas->Update();
}
