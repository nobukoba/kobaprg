#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"

void scale_each_active_histo(){
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
  char retstr[256] = "1.0";
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
      std::istringstream iss(retstr.Data());
      Double_t par;
      iss >> par;
      scl->Scale(par);
    }
  }
  return;
}
