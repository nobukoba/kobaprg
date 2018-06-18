#include "TCanvas.h"

void lgx(){
  opt_logx();
}

void opt_logx(){
  TCanvas* canvas = gPad->GetCanvas();
  gStyle->SetOptLogx();
  canvas->UseCurrentStyle();
  canvas->Update();
}
