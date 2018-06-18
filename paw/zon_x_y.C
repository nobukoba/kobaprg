#include "TString.h"
#include "TGInputDialog.h"
#include "TCanvas.h"

void zon_x_y(){
  char command[1024];
  strlcpy(command, "2 2",sizeof(command));
  TGInputDialog *id = new TGInputDialog(gClient->GetRoot(),0,
		    "Enter numer of colums and rows: nx ny",
		    command,command);
  TString str = command;
  str.ReplaceAll(","," ");
  Int_t nx, ny;
  sscanf(str.Data(),"%d %d",&nx,&ny);
  zon_x_y(nx,ny);
  return;
}

void zon_x_y(const Int_t nx = 2, const Int_t ny = 2){
  TCanvas* canvas = gPad->GetCanvas();
  canvas->Clear();
  canvas->Divide(nx,ny);
  canvas->cd(1);
  canvas->Update();
  return;
}
