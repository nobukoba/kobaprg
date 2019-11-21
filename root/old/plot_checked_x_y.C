#include <sstream>
#include "TPad.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TBox.h"
#include "TGListTree.h"
#include "TList.h"
#include "TGMsgBox.h"
#include "TTimer.h"
#include "TSystem.h"

void plot_panels(){
  TCanvas* canvas = gPad->GetCanvas();
  TList* list     = canvas->GetListOfPrimitives();
  Int_t  num_pads = list->GetSize();
  for(Int_t j = 0; j<num_pads; j++){
    ((TVirtualPad*)list->At(j))->Clear();
  }
  canvas->cd(1);

  TList *current_checked_item  = (TList *)gROOT->Get("current_checked_item");
  if(current_checked_item->GetEntries()==0){return;}
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TGListTree *hist_fListTree = (TGListTree *)gBrowserEx->GetHistListTree();
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
    cur_ListTreeItem = gBrowserEx->NextItem(cur_ListTreeItem);
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
  plot_checked_x_y(par0, par1);
  return;
}

void plot_checked_x_y(const Int_t nx = 2, const Int_t ny = 2){
  TCanvas* canvas = gPad->GetCanvas();
  canvas->Clear();
  canvas->Divide(nx,ny);
  for(Int_t j = 0; j<=nx*ny; j++){
    canvas->GetPad(j)->SetBit(TBox::kCannotMove);
    //  canvas->GetPad(j)->SetFillStyle(4000);
  }
  canvas->cd(1);
  canvas->Update();
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("gBrowserEx->GetHistListTree();");
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
    cur_ListTreeItem = gBrowserEx->NextItem(cur_ListTreeItem);
  }
  
  if (num_checked > (nx*ny)) {
    TTimer *timer = new TTimer();
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
