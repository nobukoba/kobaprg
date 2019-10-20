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
    std:: cout << std::endl << "pHistBrowser->GetHistListTree() is null." << std::endl;
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

void scale_each_active_histo(){
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");
  if (!hist_fListTree) {return;}
  char retstr[256] = "1.0";
  TList *ordered_items = (TList *) gROOT->ProcessLine("pHistBrowser->GetHistListTreeActiveItems();");
  TList items_ins;
  TList *items = &items_ins;
  if (ordered_items) {
    items = ordered_items;
  }else{
    GetHistActiveItems(items);
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
      TString str = hist->GetName();
      str += "_scl";
      TString str_n = str;
      Int_t num = 1;
      while (gROOT->Get(str_n.Data())) {
	str_n = Form("%s%d",str.Data(),num);
	num++;
      }
      TH1 *scl = (TH1*) hist->Clone(str_n);
      TString disstr = "Please enter scale for ";
      disstr += cur_ListTreeItem->GetText();
      disstr += "\nFormat: %f";
      new TGInputDialog(gClient->GetRoot(),0,
			disstr.Data(),retstr,retstr);
      if(retstr[0] == 0 && retstr[1] == 0){
	std::cout << "Cancel button was pushed. This script is terminated." << std::endl;
	return;
      }
      std::istringstream iss(retstr);
      Double_t par;
      iss >> par;
      scl->Scale(par);
    }
  }
  return;
}
