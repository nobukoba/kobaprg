#include "TROOT.h"

void start_auto_reading(){
  if (gROOT->FindObjectAny("shm_flag")){
    gROOT->ProcessLine("MyTFile::GetShmTimer()->Start(1000,kFALSE);");
  }
  return;
}
