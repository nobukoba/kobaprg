#include "TPad.h"
#include "TCanvas.h"
#include "TStyle.h"

void SetOptLogy(){
  TCanvas* canvas = gPad->GetCanvas();
  gStyle->SetOptLogy(!gStyle->GetOptLogy());
  canvas->UseCurrentStyle();
  canvas->Update();
}
