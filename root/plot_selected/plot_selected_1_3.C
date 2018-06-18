#include "TROOT.h"

void plot_selected_1_3(){
  gROOT->ProcessLine(".L ../plot_selected_x_y.C");
  gROOT->ProcessLine("plot_selected_x_y(1,3)");
  return;
}
