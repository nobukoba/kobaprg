#include "TCanvas.h"

void lgz(){
  opt_logz();
}

void opt_logz(){
  TCanvas* canvas = gPad->GetCanvas();
  gStyle->SetOptLogz();
  canvas->UseCurrentStyle();
  canvas->Update();
}
