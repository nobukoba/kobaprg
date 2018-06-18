#include "TPad.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TGListTree.h"

void plot_checked_x_y(){
  char command[1024];
  strlcpy(command, "2 2",sizeof(command));
  new TGInputDialog(gClient->GetRoot(),0,
		    "Enter numer of colums and rows: nx ny",
		    command,command);
  TString str = command;
  str.ReplaceAll(","," ");
  Int_t nx, ny;
  sscanf(str.Data(),"%d %d",&nx,&ny);
  plot_checked_x_y(nx,ny);
  return;
}

void plot_checked_x_y(const Int_t nx = 2, const Int_t ny = 2){
  TCanvas* canvas = gPad->GetCanvas();
  canvas->Clear();
  canvas->Divide(nx,ny);
  canvas->cd(1);
  canvas->Update();
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");
  if (!hist_fListTree) {return;}
  TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetFirstItem();
  Int_t i = 0;
  while(cur_ListTreeItem && (i < nx*ny)){
    TGListTreeItem *cur_ListTreeItem_child = cur_ListTreeItem->GetFirstChild();
    while(cur_ListTreeItem_child && (i < nx*ny)){
      if(cur_ListTreeItem_child->IsChecked()){
	hist_fListTree->Clicked(cur_ListTreeItem_child,0);
	i++;
      }
      cur_ListTreeItem_child = cur_ListTreeItem_child->GetNextSibling();
    }
    cur_ListTreeItem = cur_ListTreeItem->GetNextSibling();
  }
  return;
}
