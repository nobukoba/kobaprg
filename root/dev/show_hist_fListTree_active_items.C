#include "TPad.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TBox.h"
#include "TGListTree.h"

void print_hist_fListTree_active_items(){
  std::cout << std::endl << "Macro: print_hist_fListTree_active_items.C" << std::endl;
  TGFileBrowser *hist_browser = (TGFileBrowser *) gROOT->ProcessLine("gBrowserEx->GetHistBrowser();");
  TList *listitems = (TList *) gROOT->ProcessLine("gBrowserEx->GetHistListTreeActiveItems();");
  if (!listitems) {
    std::cout << "listitems: " << listitems << " is null." << std::endl;
    return;
  }
  TIter next(listitems);
  TObject * obj;
  while((obj = next())){
    TGListTreeItem *cur_ListTreeItem = (TGListTreeItem *) (((TObjString*)obj)->GetString().Atoll());
    std::cout << ""  << std::endl;
    std::cout << "fY: " << cur_ListTreeItem->fY << std::endl;
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
