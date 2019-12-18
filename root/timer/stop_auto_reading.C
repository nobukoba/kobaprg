#include "TROOT.h"

void stop_auto_reading(){
  gROOT->ProcessLine("MyTFile::GetShmTimer()->Stop();");
  return;
}
