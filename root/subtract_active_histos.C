#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
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

void GetHistActiveItems(TList *items){
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");
  if (!hist_fListTree) {
    std:: cout << "pHistBrowser->GetHistListTree() is null." << std::endl;
    return;
  }
  TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetFirstItem();
  while(cur_ListTreeItem){
    if(cur_ListTreeItem->IsActive()){
      items->Add(new TObjString(Form("%lld", (unsigned long long)cur_ListTreeItem)));
    }
    cur_ListTreeItem = NextItem(cur_ListTreeItem);
  }
  return;
}

void subtract_active_histos(){
  TList *ordered_items = (TList *) gROOT->ProcessLine("pHistBrowser->GetHistListTreeActiveItems();");
  TList items_ins;
  TList *items = &items_ins;
  if (ordered_items) {
    items = ordered_items;
  }else{
    GetHistActiveItems(items);
  }
  if (items->GetEntries() <= 1) {
    std::cout << "Entries is less than 2. Exit." << std::endl;
    return;
  }
  
  TH1 *subtracted = 0;
  TGListTreeItem *cur_ListTreeItem;
  TIter next(items);
  TObject * obj;
  while(obj = next()){
    cur_ListTreeItem = (TGListTreeItem *) (((TObjString*)obj)->GetString().Atoll());
    TObject *userdata = (TObject*)cur_ListTreeItem->GetUserData();
    if (userdata->InheritsFrom("TKey")){
      userdata = ((TKey*)userdata)->ReadObj();
      cur_ListTreeItem->SetUserData(userdata);
    }
    if (userdata->InheritsFrom("TH1")){
      TH1 *hist = (TH1*)userdata;
      gROOT->cd();
      if (subtracted == 0) {
	TString str = hist->GetName();
	str += "_sub";
	TString str_n = str;
	Int_t num = 1;
	while (gROOT->Get(str_n.Data())) {
	  str_n = Form("%s%d",str.Data(),num);
	  num++;
	}
	subtracted = (TH1*) hist->Clone(str_n);
	subtracted->SetTitle(hist->GetTitle());
      }else{
	subtracted->Add(hist,-1);
      }
    }
  }
  return;
}
