#include "TPad.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TBox.h"
#include "TGListTree.h"

void zone_x_y(){
  char command[1024];
  strlcpy(command, "2 2",sizeof(command));
  new TGInputDialog(gClient->GetRoot(),0,
		    "Enter numer of colums and rows: nx ny",
		    command,command);
  TString str = command;
  str.ReplaceAll(","," ");
  Int_t nx, ny;
  sscanf(str.Data(),"%d %d",&nx,&ny);
  plot_selected_x_y(nx,ny);
  return;
}

void zone_x_y(const Int_t nx = 2, const Int_t ny = 2){
  //std::cout << std::endl << "Macro: kobamac/root/zone_x_y.C" << std::endl;
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
