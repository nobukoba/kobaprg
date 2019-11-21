#include "TROOT.h"

void plot_checked_3_2(){
  gROOT->ProcessLine(".L ../plot_checked_x_y.C");
  gROOT->ProcessLine("plot_checked_x_y(3,2)");
  return;
}

