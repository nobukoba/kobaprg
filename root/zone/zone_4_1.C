#include "TROOT.h"

void zone_4_1(){
  gROOT->ProcessLine(".L ./zone_x_y.C");
  gROOT->ProcessLine("zone_x_y(4,1)");
  return;
}
