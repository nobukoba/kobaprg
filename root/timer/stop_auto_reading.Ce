#include "TROOT.h"

void stop_auto_reading(){
  if (gROOT->FindObjectAny("shm_flag")){
    gROOT->ProcessLine("MyTFile::GetShmTimer()->Stop();");
  }
  return;
}
