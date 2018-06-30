#include "TROOT.h"

void hout6(){
  gROOT->ProcessLine(".L ./houtn.C");
  gROOT->ProcessLine("houtn(6)");
  return;
}
