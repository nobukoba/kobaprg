#include "TROOT.h"

void zone_2_2(){
  gROOT->ProcessLine(".L ./zone_x_y.C");
  gROOT->ProcessLine("zone_x_y(2,2)");
  return;
}
