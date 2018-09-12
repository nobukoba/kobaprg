#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TGInputDialog.h"
#include "TH1.h"

TGListTreeItem *SearchNextItem(TGListTreeItem *cur_item){
  if(cur_item->GetNextSibling()){
    return cur_item->GetNextSibling();
  } else if (cur_item->GetParent()){
    return SearchNextItem(cur_item->GetParent());
  }else{
    return 0;
  }
}

TGListTreeItem *NextItem(TGListTreeItem *cur_item){
  if(cur_item->GetFirstChild()){
    return cur_item->GetFirstChild();
  }
  return SearchNextItem(cur_item);
}

void multi_fit_photo_peak_for_active(Double_t x0, Double_t x1){
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");
  if (!hist_fListTree) {return;}
  TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetFirstItem();
  gROOT->ProcessLine(".L fit_photo_peak.C");
  while(cur_ListTreeItem){
    if(cur_ListTreeItem->IsActive()){
      //std::cout<< cur_ListTreeItem->GetText() << std::endl;
      TObject *userdata = (TObject*)cur_ListTreeItem->GetUserData();
      if (userdata->InheritsFrom("TKey")){
      	userdata = ((TKey*)userdata)->ReadObj();
	cur_ListTreeItem->SetUserData(userdata);
      }
      if (userdata->InheritsFrom("TH1")){
	hist_fListTree->DoubleClicked(cur_ListTreeItem,-1);
	gROOT->ProcessLine(Form("fit_photo_peak(%lf,%lf);",x0,x1));
      }
    }
    cur_ListTreeItem = NextItem(cur_ListTreeItem);
  }
  return;
}

void multi_fit_photo_peak_for_active(){
  char command[1024] = "0.0 1.0";
  new TGInputDialog(gClient->GetRoot(),0,
  		    "Enter range: %f %f",
  		    command,command);
  TString str = command;
  str.ReplaceAll(","," ");
  Double_t x0, x1;
  sscanf(str.Data(),"%lf %lf", &x0, &x1);
  return;
  multi_fit_photo_peak_for_active(x0,x1);
  return;
}
