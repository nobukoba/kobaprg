#include "TPad.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TBox.h"
#include "TGListTree.h"

void print_ListTreeItem_status(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("gBrowserEx->GetHistListTree();");
  TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetFirstItem();
  
  while(cur_ListTreeItem){
    std::cout << ""  << std::endl;
    std::cout << "GetText(): " << cur_ListTreeItem->GetText() << std::endl;
    std::cout << "IsChecked(): " << cur_ListTreeItem->IsChecked() << std::endl;
    std::cout << "IsActive(): " << cur_ListTreeItem->IsActive() << std::endl;
    std::cout << "GetUserData(): " << cur_ListTreeItem->GetUserData() << std::endl;
    std::cout << "GetUserData()->GetName(): " << ((TObject*)cur_ListTreeItem->GetUserData())->GetName() << std::endl;
    std::cout << "GetUserData()->GetTitle(): " << ((TObject*)cur_ListTreeItem->GetUserData())->GetTitle() << std::endl;
    std::cout << "GetUserData()->ClassName(): " << ((TObject*)cur_ListTreeItem->GetUserData())->ClassName() << std::endl;
    cur_ListTreeItem = gBrowserEx->NextItem(cur_ListTreeItem);
  }
  
  return;
}
