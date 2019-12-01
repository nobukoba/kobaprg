#include "TPad.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TBox.h"
#include "TGListTree.h"

void print_active(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TGFileBrowser *hist_browser = gBrowserEx->GetHistBrowser();
  TIter next(gBrowserEx->GetHistListTreeActiveHistos());
  TObjString* objstr;
  while((objstr = (TObjString*)next())){
    TGListTreeItem *cur_ListTreeItem = (TGListTreeItem *) objstr->GetString().Atoll();
    std::cout << ""  << std::endl;
    std::cout << "GetText(): " << cur_ListTreeItem->GetText() << std::endl;
    std::cout << "IsChecked(): " << cur_ListTreeItem->IsChecked() << std::endl;
    std::cout << "IsActive(): " << cur_ListTreeItem->IsActive() << std::endl;
    std::cout << "GetUserData(): " << cur_ListTreeItem->GetUserData() << std::endl;
    std::cout << "GetUserData()->GetName(): " << ((TObject*)cur_ListTreeItem->GetUserData())->GetName() << std::endl;
    std::cout << "GetUserData()->GetTitle(): " << ((TObject*)cur_ListTreeItem->GetUserData())->GetTitle() << std::endl;
    std::cout << "GetUserData()->ClassName(): " << ((TObject*)cur_ListTreeItem->GetUserData())->ClassName() << std::endl;
    std::cout << "FullPathName(cur_ListTreeItem): " << hist_browser->FullPathName(cur_ListTreeItem) << std::endl;
  }
  
  return;
}
