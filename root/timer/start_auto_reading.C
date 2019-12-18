#include "TROOT.h"

void start_auto_reading(){
  gROOT->ProcessLine("MyTFile::StartConversion();");
  return;
}
