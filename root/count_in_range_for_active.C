#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TGInputDialog.h"
#include "TH1.h"

void count_in_range_for_active(){
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
	TH1* hist = (TH1*)userdata;
	std::cout << hist->GetName() << ": ";
	std::cout << hist->Integral(hist->GetXaxis()->GetFirst(),
				     hist->GetXaxis()->GetLast()) << std::endl;
      }
    }
    cur_ListTreeItem = pHistBrowser->NextItem(cur_ListTreeItem);
  }
  return;
}
