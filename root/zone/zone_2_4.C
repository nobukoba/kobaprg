#include "TROOT.h"

void zone_2_4(){
  gROOT->ProcessLine(".L ./zone_x_y.C");
  gROOT->ProcessLine("zone_x_y(2,4)");
  return;
}
