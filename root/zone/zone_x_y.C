#include "TPad.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TBox.h"
#include "TGListTree.h"

void zone_x_y(const Int_t nx = 2, const Int_t ny = 2){
  //std::cout << std::endl << "Macro: zone_x_y.C" << std::endl;
  TCanvas* canvas = gPad->GetCanvas();
  if (canvas == 0) {
    std::cout << "There is no canvas. The script is terminated." << std::endl;
    return;
  }
  canvas->Clear();
  canvas->Divide(nx,ny);
  for(Int_t j = 0; j<=nx*ny; j++){
    canvas->GetPad(j)->SetBit(TBox::kCannotMove);
    //  canvas->GetPad(j)->SetFillStyle(4000);
  }
  canvas->cd(1);
  canvas->Update();
  return;
}

void zone_x_y(){
  char retstr[256];
  new TGInputDialog(gClient->GetRoot(),0,
		    "Enter numer of colums and rows: nx ny",
                    "2 2",retstr);
  if(retstr[0] == 0 && retstr[1] == 0){
    std::cout << "Cancel button was pushed. This script is terminated." << std::endl;
    return;
  }
  TString str = retstr;
  str.ReplaceAll(","," ");
  std::istringstream iss(str.Data());
  Int_t par0, par1;
  iss >> par0 >> par1;
  zone_x_y(par0, par1);
  return;
}
