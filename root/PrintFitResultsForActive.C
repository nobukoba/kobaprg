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

void PrintFitResultsForActive(){
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");
  if (!hist_fListTree) {return;}
  TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetFirstItem();
  while(cur_ListTreeItem){
    if(cur_ListTreeItem->IsActive()){
      //std::cout<< cur_ListTreeItem->GetText() << std::endl;
      TObject *userdata = (TObject*)cur_ListTreeItem->GetUserData();
      if (userdata->InheritsFrom("TKey")){
      	userdata = ((TKey*)userdata)->ReadObj();
	cur_ListTreeItem->SetUserData(userdata);
      }
      if (!userdata->InheritsFrom("TH1")){
	continue;
      }
      TList *funclist = ((TH1*)userdata)->GetListOfFunctions();
      if(funclist == 0){
	//std::cout << "The GetListOfFunctions() is null. The script is terminated." << std::endl;
	continue;
      }
      Int_t j = 0;
      TF1 *funcobj = 0;
      while (funcobj = (TF1*)funclist->FindObject(Form("fit_p1g_%d",j))) {
	if (j == 0) {
	  std::cout << "HistName: " << ((TH1*)userdata)->GetName();
	}
	std::cout << ", " << funcobj->GetParameter(3);
	j++;
      }
      if (j>0) {
	std::cout << std::endl;
      }
    }
    cur_ListTreeItem = NextItem(cur_ListTreeItem);
  }
  return;
}
