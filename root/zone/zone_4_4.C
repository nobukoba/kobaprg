#include "TROOT.h"

void zone_4_4(){
  gROOT->ProcessLine(".L ./zone_x_y.C");
  gROOT->ProcessLine("zone_x_y(4,4)");
  return;
}
