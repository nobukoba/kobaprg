#include "TROOT.h"

void zon_4_2(){
  gROOT->ProcessLine(".L ../zon_x_y.C");
  gROOT->ProcessLine("zon_x_y(4,2)");
  return;
}
