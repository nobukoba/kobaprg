#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TGInputDialog.h"
#include "TH1.h"

static char retstr[256] = "0.0 1.0";
void inputdialog(){
  //static Int_t i = 0;
  TGInputDialog * id = new TGInputDialog(gClient->GetRoot(),0,
                                         "Range: %f %f",
                                         0, retstr);
  if(retstr[0] == 0 && retstr[1] == 0){
    std::cout << "Cancel button was pushed. This script is terminated." << std::endl;
    return;
  }
  return;
}
