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

Int_t merging() {
  const Int_t nfiles = 1024;
  const Int_t nsets  = 4;
  TString filenames[nsets][nfiles];
  Int_t   set_runs[nsets][nfiles];
  Int_t   set_nrun[nsets] = {0};
  TString str_head = "./run";
  TString str_tail = "/hist_MakeClover_Nobu_20190727_1st.root";
  Int_t set_runs[nsets][nfiles];
  Int_t set_nrun[nsets] = {0};

  for(Int_t i = 2066; i <= 2111; i++) { set_runs[0][(set_nrun[0])++] = i; }
  for(Int_t i = 2112; i <= 2114; i++) { set_runs[1][(set_nrun[1])++] = i; }
  for(Int_t i = 2115; i <= 2118; i++) { set_runs[2][(set_nrun[2])++] = i; }
  for(Int_t i = 2119; i <= 2200; i++) { set_runs[3][(set_nrun[3])++] = i; }
  
  for (Int_t j = 0; j < nsets; j++) {
    for (Int_t i = 0; i < set_nrun[j]; i++) {
      filenames[j][i] = str_head + Form("%d",set_runs[j][i]) + str_tail; 
      std::cout << "set: " << j << ", " << filenames[j][i] << std::endl;
    }
  }
  
  TDirectory *mergedroot = TFile::Open("merged.root","recreate");
  TDirectory *first_dir  = TFile::Open("output/date20190727/run2130/hist_MakeClover_Nobu_20190727_1st.root","read");
  TDirectory *second_dir = TFile::Open("output/date20190727/run2131/hist_MakeClover_Nobu_20190727_1st.root","read");
  
  std::cout << std::endl << "Merge start." << std::endl;
  std::cout << "Base dir: " << first_dir->GetName() << std::endl;
  CopyDir(first_dir, mergedroot);
  MergeDir(second_dir, mergedroot);
  mergedroot->Write();
  mergedroot->Close();
  std::cout << "Mearge finished." << std::endl;
  return;
}
