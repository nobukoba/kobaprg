#include "TROOT.h"

void zon_1_3(){
  gROOT->ProcessLine(".L ../zon_x_y.C");
  gROOT->ProcessLine("zon_x_y(1,3)");
  return;
}
