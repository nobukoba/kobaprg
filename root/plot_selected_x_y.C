#include "TPad.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TBox.h"
#include "TGListTree.h"

void plot_selected_x_y(){
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

void plot_selected_x_y(const Int_t nx = 2, const Int_t ny = 2){
  TCanvas* canvas = gPad->GetCanvas();
  canvas->Clear();
  canvas->Divide(nx,ny);
  for(Int_t j = 0; j<=nx*ny; j++){
    canvas->GetPad(j)->SetBit(TBox::kCannotMove);
    //  canvas->GetPad(j)->SetFillStyle(4000);
  }
  canvas->cd(1);
  canvas->Update();
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");
  if (!hist_fListTree) {return;}
  if(!hist_fListTree->GetFirstItem()){return;}
  if (!hist_fListTree->GetSelected()) {
    if(hist_fListTree->GetFirstItem()->GetFirstChild()){
      hist_fListTree->SetSelected(hist_fListTree->GetFirstItem()->GetFirstChild());
    }else{return;}
  }
  TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetSelected();
  Int_t i = 0;
  while(cur_ListTreeItem && (i < nx*ny)){
    hist_fListTree->Clicked(cur_ListTreeItem,0);
    cur_ListTreeItem = cur_ListTreeItem->GetNextSibling();
    i++;
  }
  return;
}
