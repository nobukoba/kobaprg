#include "TPad.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TBox.h"
#include "TGListTree.h"

void print_active(){
  HistBrowser *pHistBrowser = (HistBrowser *)gROOT->ProcessLine("pHistBrowser;");
  if (!pHistBrowser) {return;}
  TGListTree *hist_fListTree = (TGListTree *)pHistBrowser->GetHistListTree();
  TGFileBrowser *hist_browser = (TGFileBrowser *) gROOT->ProcessLine("pHistBrowser->GetHistBrowser();");
  if (!hist_fListTree) {return;}
  TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetFirstItem();

  while(cur_ListTreeItem){
    if (!cur_ListTreeItem->IsActive()) {
      cur_ListTreeItem = pHistBrowser->NextItem(cur_ListTreeItem);
      continue;
    }

    std::cout << ""  << std::endl;
    std::cout << "GetText(): " << cur_ListTreeItem->GetText() << std::endl;
    std::cout << "IsChecked(): " << cur_ListTreeItem->IsChecked() << std::endl;
    std::cout << "IsActive(): " << cur_ListTreeItem->IsActive() << std::endl;
    std::cout << "GetUserData(): " << cur_ListTreeItem->GetUserData() << std::endl;
    std::cout << "GetUserData()->GetName(): " << ((TObject*)cur_ListTreeItem->GetUserData())->GetName() << std::endl;
    std::cout << "GetUserData()->GetTitle(): " << ((TObject*)cur_ListTreeItem->GetUserData())->GetTitle() << std::endl;
    std::cout << "GetUserData()->ClassName(): " << ((TObject*)cur_ListTreeItem->GetUserData())->ClassName() << std::endl;
    std::cout << "FullPathName(cur_ListTreeItem): " << hist_browser->FullPathName(cur_ListTreeItem) << std::endl;
    cur_ListTreeItem = pHistBrowser->NextItem(cur_ListTreeItem);
  }
  
  return;
}
