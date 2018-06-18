#include "TCanvas.h"

void lnz(){
  opt_linz();
}

void opt_linz(){
  TCanvas* canvas = gPad->GetCanvas();
  gStyle->SetOptLogz(0);
  canvas->UseCurrentStyle();
  canvas->Update();
}
