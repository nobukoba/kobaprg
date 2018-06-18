#include "TROOT.h"

void stop_auto_obj_list(){
  gROOT->ProcessLine("pHistBrowser->GetTimerAddO2L()->Stop();");
  return;
}
