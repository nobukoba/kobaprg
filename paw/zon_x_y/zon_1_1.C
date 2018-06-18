#include "TROOT.h"

void zon_1_1(){
  gROOT->ProcessLine(".L ../zon_x_y.C");
  gROOT->ProcessLine("zon_x_y(1,1)");
  return;
}
