#include "TROOT.h"

void plot_checked_4_3(){
  gROOT->ProcessLine(".L ../plot_checked_x_y.C");
  gROOT->ProcessLine("plot_checked_x_y(4,3)");
  return;
}

