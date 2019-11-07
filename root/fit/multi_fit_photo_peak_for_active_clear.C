#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TGInputDialog.h"
#include "TH1.h"
void multi_fit_photo_peak_for_active_clear(){
  HistBrowser *pHistBrowser = (HistBrowser *)gROOT->ProcessLine("pHistBrowser;");
  if (!pHistBrowser) {return;}
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");
  TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetFirstItem();
  gROOT->ProcessLine(".L fit_photo_peak_clear.C");
  while(cur_ListTreeItem){
    if(cur_ListTreeItem->IsActive()){
      //std::cout<< cur_ListTreeItem->GetText() << std::endl;
      TObject *userdata = (TObject*)cur_ListTreeItem->GetUserData();
      if (userdata->InheritsFrom("TKey")){
      	userdata = ((TKey*)userdata)->ReadObj();
	cur_ListTreeItem->SetUserData(userdata);
      }
      if (userdata->InheritsFrom("TH1")){
	hist_fListTree->DoubleClicked(cur_ListTreeItem,1);
	gROOT->ProcessLine("fit_photo_peak_clear();");
      }
    }
    cur_ListTreeItem = pHistBrowser->NextItem(cur_ListTreeItem);
  }
  return;
}
