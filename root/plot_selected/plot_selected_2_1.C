#include "TROOT.h"

void plot_selected_2_1(){
  gROOT->ProcessLine(".L ../plot_selected_x_y.C");
  gROOT->ProcessLine("plot_selected_x_y(2,1)");
  return;
}
