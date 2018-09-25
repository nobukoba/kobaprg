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

void CopyDir(TDirectory *source) {
   //copy all objects and subdirs of directory source as a subdir of the current directory   
   source->ls();
   TDirectory *savdir = gDirectory;
   TDirectory *adir = savdir->mkdir(source->GetName());
   adir->cd();
   //loop on all entries of this directory
   TKey *key;
   TIter nextkey(source->GetListOfKeys());
   while ((key = (TKey*)nextkey())) {
      const char *classname = key->GetClassName();
      TClass *cl = gROOT->GetClass(classname);
      if (!cl) continue;
      if (cl->InheritsFrom(TDirectory::Class())) {
         source->cd(key->GetName());
         TDirectory *subdir = gDirectory;
         adir->cd();
         CopyDir(subdir);
         adir->cd();
      } else if (cl->InheritsFrom(TTree::Class())) {
	adir->cd();         
	TTree *T = (TTree*)source->Get(key->GetName());
	TTree *newT = T->CloneTree(-1,"fast");
	//TTree *T = (TTree*)source->Get(key->GetName());
	//adir->cd();
	//TTree *newT = T->CloneTree(-1,"fast");
         //newT->Write();
      } else {
	//adir->cd();
	//TObject *obj = key->ReadObj();
	source->cd();
	TObject *obj = key->ReadObj();
	adir->cd();
	obj->Clone();
	//obj->Write();
        //delete obj;
      }
   }
   //adir->SaveSelf(kTRUE);
   savdir->cd();
}

void add_active_files(){
  std::cout << std::endl << "Macro: add_active_files.C" << std::endl;
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");
  if (!hist_fListTree) {
    std::cout << "hist_fListTree is NULL. histbrowser.C is not runing? This script is terminated." << std::endl;
    return;
  }

  TCollection *list_of_files_in_groot = gROOT->GetListOfFiles(); 
  TList *list_of_active_files = new TList();
  TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetFirstItem();
  while(cur_ListTreeItem){
    if(cur_ListTreeItem->IsActive()){
      TObject *userdata = (TObject*)cur_ListTreeItem->GetUserData();
      if (!userdata->InheritsFrom("TFolder")){
	std::cout << "The active item: " << userdata->GetName()
		  << " does not inherits from TFolder. This item is skipped."
		  << std::endl;
	cur_ListTreeItem = NextItem(cur_ListTreeItem);
	continue;
      }
      TObject *fobj = list_of_files_in_groot->FindObject(cur_ListTreeItem->GetText());
      if (fobj) {
	list_of_active_files->Add(fobj);
      }
    }
    cur_ListTreeItem = NextItem(cur_ListTreeItem);
  }
  
  Int_t num_entries = list_of_active_files->GetEntries();
  if (num_entries < 2) {
    std::cout << "The number of active files is less than 2. This script is terminated." << std::endl;
  }
  
  TIter next(list_of_active_files);
  TObject *obj;
  while(obj = next()){
    std::cout << "GetName(): " << obj->GetName() << std::endl;
    std::cout << "ClassName(): " << obj->ClassName() << std::endl;
  }
  
  TFile *mergedroot = TFile::Open("../../merged.root","recreate");
  std::cout << "merged.root is opened now." << std::endl;
  TFile * first_file = (TFile *)list_of_active_files->At(0);
  mergedroot->cd();
  CopyDir(first_file);
  mergedroot->cd();
  std::cout << "" << std::endl;
  std::cout << "" << std::endl;
  std::cout << "Finished." << std::endl;
  //mergedroot->ls();
  mergedroot->Write();
  
  //TH1 *added = 0;
  //TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetFirstItem();
  //while(cur_ListTreeItem){
  //  if(cur_ListTreeItem->IsActive()){
  //    TObject *userdata = (TObject*)cur_ListTreeItem->GetUserData();
  //    if (userdata->InheritsFrom("TKey")){
  //    	userdata = ((TKey*)userdata)->ReadObj();
  //	cur_ListTreeItem->SetUserData(userdata);
  //    }
  //    if (userdata->InheritsFrom("TH1")){
  //	TH1 *hist = (TH1*)userdata;
  //	gROOT->cd();
  //	if (added == 0) {
  //	  TString str = hist->GetName();
  //	  str += "_add";
  //	  TString str_n = str;
  //	  Int_t num = 1;
  //	  while (gROOT->Get(str_n.Data())) {
  //	    str_n = Form("%s%d",str.Data(),num);
  //	    num++;
  //	  }
  //	  added = (TH1*) hist->Clone(str_n);
  //	  added->SetTitle(hist->GetTitle());
  //	}else{
  //	  added->Add(hist);
  //	}
  //    }
  //  }
  //  cur_ListTreeItem = NextItem(cur_ListTreeItem);
  //}
  return;
}
