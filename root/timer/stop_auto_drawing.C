#include "TROOT.h"

void stop_auto_drawing(){
  gROOT->ProcessLine("pHistBrowser->GetTimerUpdatingGPad()->Stop();");
  return;
}
