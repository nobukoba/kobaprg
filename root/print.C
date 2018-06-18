#include "TROOT.h"

void print(){
  gROOT->ProcessLine("pHistBrowser->PrintCanvas();");
}
