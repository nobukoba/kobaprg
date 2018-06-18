#include "TROOT.h"

void start_auto_drawing(){
  gROOT->ProcessLine("pHistBrowser->GetTimerUpdatingGPad()->Start(1000,kFALSE);");
  return;
}
