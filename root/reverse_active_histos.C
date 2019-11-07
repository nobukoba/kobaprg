#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"

void reverse_active_histos(){
  HistBrowser *pHistBrowser = (HistBrowser *)gROOT->ProcessLine("pHistBrowser;");
  if (!pHistBrowser) {return;}
  TGListTree *hist_fListTree = (TGListTree *)pHistBrowser->GetHistListTree();
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
	TString str = hist->GetName();
	str += "_rev";
	TString str_n = str;
	Int_t num = 1;
	while (gROOT->Get(str_n.Data())) {
	  str_n = Form("%s%d",str.Data(),num);
	  num++;
	}
	TH1 *rev = (TH1*) hist->Clone(str_n);
	rev->Scale(-1);
      }
    }
    cur_ListTreeItem = pHistBrowser->NextItem(cur_ListTreeItem);
  }
  return;
}
