//#if defined(__CINT__) && !defined(__MAKECINT__)
//{
//  TBrowserEx *gBrowserEx_tmp = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
//  TString dir;
//  if (gBrowserEx_tmp) {
//    dir = gBrowserEx_tmp->GetInitialWorkingDir();
//    gSystem->cd(dir.Data());
//  }else{
//    return;
//  }
//  /*gSystem->CompileMacro("./kobaprg/root/ForActiveItems/add_active_dir.C","k");*/
//  /*add_active_dir();*/
//  gROOT->ProcessLine(".L ./kobaprg/root/ForActiveItems/add_active_dir.C+");
//  gROOT->ProcessLine("add_active_dir()");
//}
//#else

#include <iostream>
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGFileBrowser.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"

void CopyDir(TDirectory *source, TDirectory *target) {
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
    delete obj;
  }
}

void MergeDir(TDirectory *source, TDirectory *target) {
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
    delete obj;
  }
}

void add_active_dir(){
  std::cout << std::endl << "Macro: add_active_files.C" << std::endl;
  TBrowserEx *gBrowserEx_tmp = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx_tmp) {
    gSystem->cd((gBrowserEx_tmp->GetInitialWorkingDir()).Data());
  }else{return;}
  std::cout << "gSystem->pwd(): " << gSystem->pwd() << std::endl;
  
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("gBrowserEx->GetHistListTree();");
  TGFileBrowser *hist_browser = (TGFileBrowser *) gROOT->ProcessLine("gBrowserEx->GetHistBrowser();");
  if (!hist_fListTree) {
    std::cout << "hist_fListTree is NULL. histbrowser.C is not runing? This script is terminated." << std::endl;
    return;
  }
  TCollection *list_of_files_in_groot = gROOT->GetListOfFiles();
  TFile* pref = (TFile*)list_of_files_in_groot->FindObject("merged.root");
  if (pref) {
    pref->Close();
  }
  TList *list_of_active_dir = new TList();
  TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetFirstItem();
  while(cur_ListTreeItem){
    if(cur_ListTreeItem->IsActive()){
      TObject *userdata = (TObject*)cur_ListTreeItem->GetUserData();
      if (!userdata->InheritsFrom("TFolder") && !userdata->InheritsFrom("TDirectory")){
  	std::cout << "The active item: " << userdata->GetName()
  		  << " does not inherits from TFolder/TDirectory. This item is skipped."
  		  << std::endl;
  	cur_ListTreeItem = gBrowserEx->NextItem(cur_ListTreeItem);
  	continue;
      }
      TString fullpath = hist_browser->FullPathName(cur_ListTreeItem);
      TDirectory *cur_dir = 0;
      Int_t memo_file_flag = 0;
      TString filename = "";
      if (fullpath.EqualTo("ROOT_Memory")){
	memo_file_flag = 1;
	cur_dir = gROOT;
	filename = fullpath;
      }else if (fullpath.BeginsWith("ROOT_Memory/")){
	memo_file_flag = 1;
	fullpath.Replace(0,12,"");
	cur_dir = (TDirectory *)gROOT->Get(fullpath);
	filename = fullpath;
      }else if(fullpath.BeginsWith("ROOT_Files/")){
	memo_file_flag = 2;
	fullpath.Replace(0,11,"");
	TCollection *lst = gROOT->GetListOfFiles(); 
	TIter next(lst);
	TFile *file;
	while((file=(TFile*)next())){
	  filename = file->GetName();
	  if(fullpath.BeginsWith(filename)){
	    if(fullpath.Length()==filename.Length()){
	      cur_dir = (TDirectory *)file;
	    }else{
	      fullpath.Replace(0,filename.Length()+1,"");
	      cur_dir = (TDirectory *)file->Get(fullpath);
	    }
	    break;
	  }
	}
      }
      
      if (cur_dir) {
  	list_of_active_dir->Add((TObject*)cur_dir);
      }
    }
    cur_ListTreeItem = gBrowserEx->NextItem(cur_ListTreeItem);
  }
  
  Int_t num_entries = list_of_active_dir->GetEntries();
  if (num_entries < 2) {
    std::cout << "The number of active files is less than 2. This script is terminated." << std::endl;
    return;
  }
  
  TDirectory *mergedroot = TFile::Open("merged.root","recreate");
  TDirectory *first_dir = (TDirectory *)(list_of_active_dir->At(0));
  std::cout << std::endl << "Merge start." << std::endl;
  std::cout << "Base dir: " << first_dir->GetName() << std::endl;
  CopyDir(first_dir, mergedroot);
  
  for (Int_t i = 1; i < num_entries; i++) {
    std::cout << i << "-th dir: " << list_of_active_dir->At(i)->GetName() << std::endl; 
    MergeDir((TDirectory *)(list_of_active_dir->At(i)), mergedroot);
  }
  
  mergedroot->Write();
  //mergedroot->Close();
  
  std::cout << "Mearge finished." << std::endl;
  return;
}

//#endif
