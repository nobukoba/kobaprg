#include "TROOT.h"

void plot_checked_2_1(){
  gROOT->ProcessLine(".L ../plot_checked_x_y.C");
  gROOT->ProcessLine("plot_checked_x_y(2,1)");
  return;
}
