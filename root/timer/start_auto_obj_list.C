#include "TROOT.h"

void start_auto_obj_list(){
  gROOT->ProcessLine("pHistBrowser->GetTimerAddO2L()->Start(1000,kFALSE);");
  return;
}
