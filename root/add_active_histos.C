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

void add_active_histos(){
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");
  if (!hist_fListTree) {return;}
  TH1 *added = 0;
  TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetFirstItem();
  while(cur_ListTreeItem){
    if(cur_ListTreeItem->IsActive()){
      TObject *userdata = (TObject*)cur_ListTreeItem->GetUserData();
      if (userdata->InheritsFrom("TKey")){
      	userdata = ((TKey*)userdata)->ReadObj();
	cur_ListTreeItem->SetUserData(userdata);
      }
      if (userdata->InheritsFrom("TH1")){
	TH1 *hist = (TH1*)userdata;
	gROOT->cd();
	if (added == 0) {
	  TString str = hist->GetName();
	  str += "_add";
	  TString str_n = str;
	  Int_t num = 1;
	  while (gROOT->Get(str_n.Data())) {
	    str_n = Form("%s%d",str.Data(),num);
	    num++;
	  }
	  added = (TH1*) hist->Clone(str_n);
	}else{
	  added->Add(hist);
	}
      }
    }
    cur_ListTreeItem = NextItem(cur_ListTreeItem);
  }
  return;
}
