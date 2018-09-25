#if defined(__CINT__) && !defined(__MAKECINT__)
{
  TNamed *named = (TNamed*)gROOT->FindObjectAny("initial_working_dir");
  if (named) {gSystem->cd(named->GetTitle());}
  gSystem->CompileMacro("./kobaprg/root/add_active_files.C","k");
  add_active_files();
}
#else

#include <iostream>
#include "TSystem.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"

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

void CopyDir(TDirectory *source, TDirectory *target) {
//source->ls();
TDirectory *savdir = gDirectory;
TKey *key;
  TIter nextkey(source->GetListOfKeys());
  while ((key = (TKey*)nextkey())) {
    const char *classname = key->GetClassName();
    TClass *cl = gROOT->GetClass(classname);
    TObject *obj = key->ReadObj();
     
    if (!cl) continue;
    if(target->FindObject(obj->GetName())){
      continue;
    }
    if (cl->InheritsFrom(TDirectory::Class())) {
      TDirectory * dir = target->mkdir(obj->GetName());
      CopyDir((TDirectory *)obj,dir);
    } else if (cl->InheritsFrom(TTree::Class())) {
      continue;
      //target->cd();
      //TTree *T = (TTree*)source->Get(key->GetName());
      //T->CloneTree(-1,"fast");
    } else if (cl->InheritsFrom(TH1::Class())){
      target->cd();
      obj->Clone(obj->GetName());
    }
  }
  //adir->SaveSelf(kTRUE);
  savdir->cd();
}

void MergeDir(TDirectory *source, TDirectory *target) {
  //source->ls();
  TDirectory *savdir = gDirectory;
  TKey *key;
  TIter nextkey(source->GetListOfKeys());
  while ((key = (TKey*)nextkey())) {
    if(key->GetCycle() > 1){continue;}
    TObject *tgt = target->FindObject(key->GetName());
    if (!tgt) {
      std::cout << "key: " << key->GetName()
		<< " was not found in the directory: "
		<< target->GetName()
		<< std::endl;
      continue;
    }
    const char *classname = key->GetClassName();
    TClass *cl = gROOT->GetClass(classname);
    const char *tgtclassname = tgt->ClassName();
    TClass *tgtcl = gROOT->GetClass(tgtclassname);
    if (cl != tgtcl) {
      std::cout << "source class: " << classname
		<< " is different from target class: "
		<< tgtclassname
		<< std::endl;
      continue;
    }
    
    TObject *obj = key->ReadObj();
    if (!cl) continue;
    if (cl->InheritsFrom(TDirectory::Class())) {
      MergeDir((TDirectory *)obj,(TDirectory *)tgt);
    } else if (cl->InheritsFrom(TTree::Class())) {
      continue;
      //TTree *T = (TTree*)source->Get(tgt->GetName());
      //T->Add((TTree*)obj);
    } else if (cl->InheritsFrom(TH2::Class())){
      ((TH2*)tgt)->Add((TH2*)obj);
    } else if (cl->InheritsFrom(TH1::Class())){
      ((TH1*)tgt)->Add((TH1*)obj);
    }
  }
  //adir->SaveSelf(kTRUE);
  savdir->cd();
}

void add_active_files(){
  std::cout << std::endl << "Macro: add_active_files.C" << std::endl;
  TNamed *named = (TNamed*)gROOT->FindObjectAny("initial_working_dir");
  if (named) {gSystem->cd(named->GetTitle());}
  std::cout << "gSystem->pwd(): " << gSystem->pwd() << std::endl;
  
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");
  if (!hist_fListTree) {
    std::cout << "hist_fListTree is NULL. histbrowser.C is not runing? This script is terminated." << std::endl;
    return;
  }
  TCollection *list_of_files_in_groot = gROOT->GetListOfFiles();
  TFile* pref = (TFile*)list_of_files_in_groot->FindObject("merged.root");
  if (pref) {
    pref->Close();
  }
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
    return;
  }
  
  TDirectory *mergedroot = TFile::Open("merged.root","recreate");
  TDirectory *first_file = (TDirectory *)(list_of_active_files->At(0));
  std::cout << std::endl << "Merge start." << std::endl;
  std::cout << "Base file: " << first_file->GetName() << std::endl;
  CopyDir(first_file, mergedroot);
  
  for (Int_t i = 1; i < num_entries; i++) {
    std::cout << i << "-th file: " << list_of_active_files->At(i)->GetName() << std::endl; 
    MergeDir((TDirectory *)(list_of_active_files->At(i)), mergedroot);
  }
  
  mergedroot->Write();
  //mergedroot->Close();
  
  std::cout << "Mearge finished." << std::endl;
  return;
}

#endif

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
