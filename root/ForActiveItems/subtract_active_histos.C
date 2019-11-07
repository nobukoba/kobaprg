#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"

void subtract_active_histos(){
  HistBrowser *pHistBrowser = (HistBrowser *)gROOT->ProcessLine("pHistBrowser;");
  if (!pHistBrowser) {return;}
  TList *ordered_items = (TList *)pHistBrowser->GetHistListTreeActiveItems();
  TList items_ins;
  TList *items = &items_ins;
  if (ordered_items) {
    items = ordered_items;
  }else{
    pHistBrowser->GetHistActiveItems(items);
  }
  if (items->GetEntries() <= 1) {
    std::cout << "Entries is less than 2. Exit." << std::endl;
    return;
  }
  
  TH1 *subtracted = 0;
  TGListTreeItem *cur_ListTreeItem;
  TIter next(items);
  TObject * obj;
  while((obj = next())){
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
