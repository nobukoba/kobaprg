#include "TROOT.h"

void plot_selected_1_2(){
  gROOT->ProcessLine(".L ../plot_selected_x_y.C");
  gROOT->ProcessLine("plot_selected_x_y(1,2)");
  return;
}
