#include "TROOT.h"

void plot_selected_2_3(){
  gROOT->ProcessLine(".L ../plot_selected_x_y.C");
  gROOT->ProcessLine("plot_selected_x_y(2,3)");
  return;
}
