#include "TROOT.h"

void zon_2_2(){
  gROOT->ProcessLine(".L ../zon_x_y.C");
  gROOT->ProcessLine("zon_x_y(2,2)");
  return;
}
