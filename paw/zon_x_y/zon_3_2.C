#include "TROOT.h"

void zon_3_2(){
  gROOT->ProcessLine(".L ../zon_x_y.C");
  gROOT->ProcessLine("zon_x_y(3,2)");
  return;
}
