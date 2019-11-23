#include "TPad.h"
#include "TCanvas.h"
#include "TStyle.h"

void SetOptLogz(){
  TCanvas* canvas = gPad->GetCanvas();
  gStyle->SetOptLogz(!gStyle->GetOptLogz());
  canvas->UseCurrentStyle();
  canvas->Update();
}
