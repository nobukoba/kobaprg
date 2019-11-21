#include "TROOT.h"

void zone_4_3(){
  gROOT->ProcessLine(".L ./zone_x_y.C");
  gROOT->ProcessLine("zone_x_y(4,3)");
  return;
}
