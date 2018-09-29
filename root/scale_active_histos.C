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

void scale_active_histos(){
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");
  if (!hist_fListTree) {return;}
  TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetFirstItem();
  char retstr[256] = "1.0";
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
	str += "_scl";
	TString str_n = str;
	Int_t num = 1;
	while (gROOT->Get(str_n.Data())) {
	  str_n = Form("%s%d",str.Data(),num);
	  num++;
	}
	TH1 *rev = (TH1*) hist->Clone(str_n);
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
	rev->Scale(par);
      }
    }
    cur_ListTreeItem = NextItem(cur_ListTreeItem);
  }
  return;
}
