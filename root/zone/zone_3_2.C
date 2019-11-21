#include "TROOT.h"

void zone_3_2(){
  gROOT->ProcessLine(".L ./zone_x_y.C");
  gROOT->ProcessLine("zone_x_y(3,2)");
  return;
}
