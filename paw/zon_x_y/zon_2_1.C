#include "TROOT.h"

void zon_2_1(){
  gROOT->ProcessLine(".L ../zon_x_y.C");
  gROOT->ProcessLine("zon_x_y(2,1)");
  return;
}
