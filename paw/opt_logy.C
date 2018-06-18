#include "TCanvas.h"

void lgy(){
  opt_logy();
}

void opt_logy(){
  TCanvas* canvas = gPad->GetCanvas();
  gStyle->SetOptLogy();
  canvas->UseCurrentStyle();
  canvas->Update();
}
