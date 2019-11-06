#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TGInputDialog.h"
#include "TH1.h"


TGListTreeItem *SearchNextItem_3(TGListTreeItem *cur_item){
  if(cur_item->GetNextSibling()){
    return cur_item->GetNextSibling();
  } else if (cur_item->GetParent()){
    return SearchNextItem_3(cur_item->GetParent());
  }else{
    return 0;
  }
}

TGListTreeItem *NextItem_3(TGListTreeItem *cur_item){
  if(cur_item->GetFirstChild()){
    return cur_item->GetFirstChild();
  }
  return SearchNextItem_3(cur_item);
}

void count_in_range_for_active(){
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");
  if (!hist_fListTree) {return;}
  TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetFirstItem();
  while(cur_ListTreeItem){
    if(cur_ListTreeItem->IsActive()){
      TObject *userdata = (TObject*)cur_ListTreeItem->GetUserData();
      if (userdata->InheritsFrom("TKey")){
      	userdata = ((TKey*)userdata)->ReadObj();
	cur_ListTreeItem->SetUserData(userdata);
      }
      if (userdata->InheritsFrom("TH1")){
	TH1* hist = (TH1*)userdata;
	std::cout << hist->GetName() << ": ";
	std::cout << hist->Integral(hist->GetXaxis()->GetFirst(),
				     hist->GetXaxis()->GetLast()) << std::endl;
      }
    }
    cur_ListTreeItem = NextItem_3(cur_ListTreeItem);
  }
  return;
}
