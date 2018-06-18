#include "TCanvas.h"
#include "TPad.h"

void clear(){
  TCanvas* canvas = gPad->GetCanvas();
  canvas->Clear();
  canvas->cd();
  canvas->Update();
}



