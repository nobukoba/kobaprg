#include "TROOT.h"

void zon_3_1(){
  gROOT->ProcessLine(".L ../zon_x_y.C");
  gROOT->ProcessLine("zon_x_y(3,1)");
  return;
}
