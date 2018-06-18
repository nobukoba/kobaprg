#include "TROOT.h"

void zon_4_4(){
  gROOT->ProcessLine(".L ../zon_x_y.C");
  gROOT->ProcessLine("zon_x_y(4,4)");
  return;
}
