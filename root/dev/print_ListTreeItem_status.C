#include "TPad.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TBox.h"
#include "TGListTree.h"

TGListTreeItem *NextItem(TGListTreeItem *cur_item){
  if(cur_item->GetFirstChild()){
    return cur_item->GetFirstChild();
  }
  return SearchNextItem(cur_item);
}

TGListTreeItem *SearchNextItem(TGListTreeItem *cur_item){
  if(cur_item->GetNextSibling()){
    return cur_item->GetNextSibling();
  } else if (cur_item->GetParent()){
    return SearchNextItem(cur_item->GetParent());
  }else{
    return 0;
  }
}

void print_ListTreeItem_status(){
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");
  if (!hist_fListTree) {return;}
  TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetFirstItem();
  
  while(cur_ListTreeItem = NextItem(cur_ListTreeItem)){
    std::cout<< "cur_ListTreeItem GetText: " << cur_ListTreeItem->GetText() <<std::endl;
    std::cout<< "cur_ListTreeItem IsChecked: " << cur_ListTreeItem->IsChecked() <<std::endl;
    std::cout<< "cur_ListTreeItem IsActive: " << cur_ListTreeItem->IsActive() <<std::endl;
  }
  
  return;
}
