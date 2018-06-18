#include "TROOT.h"

void plot_checked_1_1(){
  gROOT->ProcessLine(".L ../plot_checked_x_y.C");
  gROOT->ProcessLine("plot_checked_x_y(1,1)");
  return;
}
