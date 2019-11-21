#include <iostream>
#include <sstream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TGInputDialog.h"
#include "TH1.h"

void set_range_for_checked(Double_t x0, Double_t x1){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TGListTree *hist_fListTree = (TGListTree *)gBrowserEx->GetHistListTree();
  TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetFirstItem();
  while(cur_ListTreeItem){
    if(cur_ListTreeItem->IsChecked()){
      //std::cout<< cur_ListTreeItem->GetText() << std::endl;
      TObject *userdata = (TObject*)cur_ListTreeItem->GetUserData();
      if (userdata->InheritsFrom("TKey")){
      	userdata = ((TKey*)userdata)->ReadObj();
	cur_ListTreeItem->SetUserData(userdata);
      }
      if (userdata->InheritsFrom("TH1")){
	((TH1*)userdata)->GetXaxis()->SetRangeUser(x0,x1);
      }
    }
    cur_ListTreeItem = gBrowserEx->NextItem(cur_ListTreeItem);
  }
  return;
}

void set_range_for_checked(){
  char retstr[256];
  new TGInputDialog(gClient->GetRoot(),0,
                    "Range: %f %f",
                    "0.0 1.0",retstr);
  if(retstr[0] == 0 && retstr[1] == 0){
    std::cout << "Cancel button was pushed. This script is terminated." << std::endl;
    return;
  }
  TString str = retstr;
  str.ReplaceAll(","," ");
  std::istringstream iss(str.Data());
  Double_t par0, par1;
  iss >> par0 >> par1;
  set_range_for_checked(par0, par1);
  return;
}
