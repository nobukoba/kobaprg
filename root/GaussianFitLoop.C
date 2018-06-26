#include "TH1.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TList.h"
#include "TTimer.h"
void fitting(){
  gROOT->ProcessLine("GaussianFit()");
}

void GaussianFitLoop(){
  gROOT->ProcessLine(".L GaussianFit.C");
  TTimer *timer = new TTimer();
  timer->Connect("Timeout()",0,0,"fitting()");
  timer->Timeout();
  timer->Start(1000,kFALSE);
  new TGMsgBox(gClient->GetRoot(),0, "Script is running!!", "GaussianFitLoop.C is now running!!\nDo you stop it?", kMBIconAsterisk, kMBYes);
  delete timer;
}
