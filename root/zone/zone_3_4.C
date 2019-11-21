#include "TROOT.h"

void zone_3_4(){
  gROOT->ProcessLine(".L ./zone_x_y.C");
  gROOT->ProcessLine("zone_x_y(3,4)");
  return;
}
