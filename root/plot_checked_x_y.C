#include "TPad.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TGMsgBox.h"
#include "TTimer.h"
#include "TSystem.h"

TGListTreeItem *NextItem(TGListTreeItem *cur_item){
  if(cur_item->GetFirstChild()){
    return cur_item->GetFirstChild();
  }
  return SearchNextItem(cur_item);
}

TGListTreeItem *SearchNextItem(TGListTreeItem *cur_item){
  if(cur_item->GetNextSibling()){
    return cur_item->GetNextSibling();
  } else if (cur_item->GetParent()){
    return SearchNextItem(cur_item->GetParent());
  }else{
    return 0;
  }
}

void plot_panels(){
  TCanvas* canvas = gPad->GetCanvas();
  TList* list     = canvas->GetListOfPrimitives();
  Int_t  num_pads = list->GetSize();
  for(Int_t j = 0; j<num_pads; j++){
    ((TVirtualPad*)list->At(j))->Clear();
  }
  canvas->cd(1);

  TList *current_checked_item  = (TList *)gROOT->Get("current_checked_item");
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");
  TGListTreeItem *cur_ListTreeItem = (TGListTreeItem *)current_checked_item->At(1);
  Int_t i = 0;
  while(cur_ListTreeItem){
    if(cur_ListTreeItem->IsChecked()){
      if(i<num_pads){
	hist_fListTree->Clicked(cur_ListTreeItem,0);
	i++;
      }else{
	break;
      }
    }
    cur_ListTreeItem = NextItem(cur_ListTreeItem);
  }
  if(cur_ListTreeItem==0){
    current_checked_item->RemoveLast();
    current_checked_item->Add(current_checked_item->At(0));
  }else{
    current_checked_item->RemoveLast();
    current_checked_item->Add((TObject*)cur_ListTreeItem);
  }
}

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
  Int_t num_checked = 0;

  TList *current_checked_item  = new TList();
  current_checked_item->SetName("current_checked_item");
  gROOT->Add(current_checked_item);
  
  while(cur_ListTreeItem){
    if(cur_ListTreeItem->IsChecked()){
      num_checked++;
      if(current_checked_item->GetEntries()==0){
	current_checked_item->Add((TObject*)cur_ListTreeItem);
	current_checked_item->Add((TObject*)cur_ListTreeItem);
      }
    }
    cur_ListTreeItem = NextItem(cur_ListTreeItem);
  }
  
  if (num_checked > (nx*ny)) {
    timer = new TTimer();
    timer->Connect("Timeout()",0,0,"plot_panels()");
    timer->Timeout();
    timer->Start(3000,kFALSE);
    new TGMsgBox(gClient->GetRoot(),0, "Script is running!!", "plot_checked_x_y.C is now running!!\nDo you stop it?", kMBIconAsterisk, kMBYes);
    delete timer;
  }else{
    plot_panels();
  }

  gROOT->Remove(current_checked_item);
  delete current_checked_item;
  return;
}
