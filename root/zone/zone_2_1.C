#include "TROOT.h"

void zone_2_1(){
  gROOT->ProcessLine(".L ./zone_x_y.C");
  gROOT->ProcessLine("zone_x_y(2,1)");
  return;
}
