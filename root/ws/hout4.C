#include "TROOT.h"

void hout4(){
  gROOT->ProcessLine(".L ./houtn.C");
  gROOT->ProcessLine("houtn(4)");
  return;
}
