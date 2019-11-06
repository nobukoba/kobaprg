#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"

TGListTreeItem *SearchNextItem_4(TGListTreeItem *cur_item){
  if(cur_item->GetNextSibling()){
    return cur_item->GetNextSibling();
  } else if (cur_item->GetParent()){
    return SearchNextItem_4(cur_item->GetParent());
  }else{
    return 0;
  }
}

TGListTreeItem *NextItem_4(TGListTreeItem *cur_item){
  if(cur_item->GetFirstChild()){
    return cur_item->GetFirstChild();
  }
  return SearchNextItem_4(cur_item);
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
    cur_ListTreeItem = NextItem_4(cur_ListTreeItem);
  }
  return;
}

void sub_techno_ap_hist(){
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
  Int_t nhist = items->GetSize();
  std::cout << "The number of selected histos: " << nhist << std::endl;
  if (nhist % 4 != 0) {
    std::cout << "The number of histos should be 4 * runs. Return!!" << std::endl;
    return;
  }
  Int_t nruns = nhist / 4;
  
  for (Int_t i = 0; i < 4; i++) {
    for (Int_t j = 1; j < nruns; j++) {
      Int_t i1 = 4 * (j-1) + i;
      Int_t i2 = 4 * j + i;
      TObject * obj1 = items->At(i1);
      TObject * obj2 = items->At(i2);
      TGListTreeItem *cur_ListTreeItem1 = (TGListTreeItem *) (((TObjString*)obj1)->GetString().Atoll());
      TGListTreeItem *cur_ListTreeItem2 = (TGListTreeItem *) (((TObjString*)obj2)->GetString().Atoll());
      TObject *userdata1 = (TObject*)cur_ListTreeItem1->GetUserData();
      TObject *userdata2 = (TObject*)cur_ListTreeItem2->GetUserData();
      std::cout << "userdata1 " << userdata1->GetName()<< std::endl;
      std::cout << "userdata2 " << userdata2->GetName()<< std::endl;
      if (userdata1->InheritsFrom("TKey")){
	userdata1 = ((TKey*)userdata1)->ReadObj();
	cur_ListTreeItem1->SetUserData(userdata1);
      }
      if (userdata2->InheritsFrom("TKey")){
	userdata2 = ((TKey*)userdata2)->ReadObj();
	cur_ListTreeItem2->SetUserData(userdata2);
      }
      if (userdata1->InheritsFrom("TH1") &&
	  userdata2->InheritsFrom("TH1")){
	TH1 *hist1 = (TH1*)userdata1;
	TH1 *hist2 = (TH1*)userdata2;
	gROOT->cd();
	TString str = hist2->GetName();
	str += "_sub";
	TString str_n = str;
	Int_t num = 1;
	while (gROOT->Get(str_n.Data())) {
	  str_n = Form("%s%d",str.Data(),num);
	  num++;
	}
	TH1 *sub = (TH1*) hist2->Clone(str_n);
	sub->SetTitle(hist2->GetTitle());
	sub->Add(hist1,-1);
      }
    }
  }
  return;
}
