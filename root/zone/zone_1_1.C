#include "TROOT.h"

void zone_1_1(){
  gROOT->ProcessLine(".L ./zone_x_y.C");
  gROOT->ProcessLine("zone_x_y(1,1)");
  return;
}
