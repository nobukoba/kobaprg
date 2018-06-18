#include "TROOT.h"

void zon_2_4(){
  gROOT->ProcessLine(".L ../zon_x_y.C");
  gROOT->ProcessLine("zon_x_y(2,4)");
  return;
}
