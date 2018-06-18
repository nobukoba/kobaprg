#include "TROOT.h"

void zon_4_3(){
  gROOT->ProcessLine(".L ../zon_x_y.C");
  gROOT->ProcessLine("zon_x_y(4,3)");
  return;
}
