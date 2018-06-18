#include "TCanvas.h"

void lnx(){
  opt_linx();
}

void opt_linx(){
  TCanvas* canvas = gPad->GetCanvas();
  gStyle->SetOptLogx(0);
  canvas->UseCurrentStyle();
  canvas->Update();
}
