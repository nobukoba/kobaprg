#include "TPad.h"
#include "TROOT.h"
#include "TCanvas.h"

void draw_frame(){
  char retstr[256];
  new TGInputDialog(gClient->GetRoot(),0,
		    "Draw frame with xmin, ymin, xmax, ymax\nFormat: %f %f %f %f",
                    "0.0 0.0 1.0 1.0",retstr);
  if(retstr[0] == 0 && retstr[1] == 0){
    std::cout << "Cancel button was pushed. This script is terminated." << std::endl;
    return;
  }
  TString str = retstr;
  str.ReplaceAll(","," ");
  std::istringstream iss(str);
  Double_t par0, par1, par2, par3;
  iss >> par0 >> par1 >> par2 >> par3;
  draw_frame(par0, par1, par2, par3);
  return;
}

void draw_frame(const Double_t xmin = 0.0,
		const Double_t ymin = 0.0,
		const Double_t xmax = 1.0,
		const Double_t ymax = 1.0){
  //std::cout << std::endl << "Macro: zone_x_y.C" << std::endl;
  if (gPad == 0) {
    std::cout << "There is no gPad. The script is terminated." << std::endl;
    return;
  }
  gPad->DrawFrame(xmin,ymin,xmax,ymax);
  return;
}

