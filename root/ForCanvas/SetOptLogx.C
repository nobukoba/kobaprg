#include "TPad.h"
#include "TCanvas.h"
#include "TStyle.h"

void SetOptLogx(){
  TCanvas* canvas = gPad->GetCanvas();
  gStyle->SetOptLogx(!gStyle->GetOptLogx());
  canvas->UseCurrentStyle();
  canvas->Update();
}
