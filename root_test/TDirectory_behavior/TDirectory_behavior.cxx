#include <iostream>
 #include "TFile.h"
#include "TH1.h"
#include "TDirectory.h"
#include "TROOT.h"
#include "TDirectoryFile.h"

int main(int argc, char *argv[]) {
  std::cout << "aa" <<std::endl;
  TFile *f = new TFile("aa.root","RECREATE");
  TDirectory *d1 = new TDirectoryFile("d1","d1","",f);
  d1->cd();
  TH1* h1 = new TH1F("h1", "h1", 10, 0., 1.);
  TDirectory *d2 = new TDirectoryFile("d2","d2","",f);
  d2->cd();
  TH1* h2 = new TH1F("h2", "h2", 10, 0., 1.);
  std::cout << "d2->FindObject h2 " << d2->FindObject("h2") << std::endl;
  std::cout << "d1->FindObject h1 " << d1->FindObject("h1") << std::endl;
  d1->cd();
  std::cout << "d1->FindObject h1 " << d1->FindObject("h1") << std::endl;

  gROOT->cd();
  TH1* h3 = new TH1F("h3", "h3", 10, 0., 1.);
  std::cout << "gROOT->FindObject h3 " << ((TDirectory*)gROOT)->FindObject("h3") << std::endl;
  d1->cd();
  std::cout << "gROOT" << gROOT << std::endl;
  std::cout << "((TDirectory*)gROOT)->FindObject h3 " << ((TDirectory*)gROOT)->FindObject("h3") << std::endl;
  std::cout << "gROOT->FindObject h3 " << gROOT->TDirectory::FindObject("h3") << std::endl;
  gROOT->cd();
  std::cout << "gROOT" << gROOT << std::endl;
  std::cout << "((TDirectory*)gROOT)->FindObject h3 " << ((TDirectory*)gROOT)->FindObject("h3") << std::endl;
  std::cout << "gROOT->FindObject h3 " << gROOT->FindObject("h3") << std::endl;
  std::cout << "d2->FindObject h2 " << d2->FindObject("h2") << std::endl;
  
  f->ls();
  f->Close();

  return 0;
}
