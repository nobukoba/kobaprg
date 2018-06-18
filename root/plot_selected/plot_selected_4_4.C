#include "TROOT.h"

void plot_selected_4_4(){
  gROOT->ProcessLine(".L ../plot_selected_x_y.C");
  gROOT->ProcessLine("plot_selected_x_y(4,4)");
  return;
}
