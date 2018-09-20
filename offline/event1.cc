#include "TFile.h"
#include "TTree.h"

TFile* fout1;
TTree *tree;
int adc1, adc2, adc3, adc4, adc5, adc6;
int tdc1, tdc2, tdc3, tdc4, tdc5, tdc6;

void hstdef_(char * filename){
  fout1 = new TFile(filename, "recreate");
  tree = new TTree("tr", "tr");
  tree->Branch("adc1", &adc1, "adc1/I");
  tree->Branch("adc2", &adc2, "adc2/I");
  tree->Branch("adc3", &adc3, "adc3/I");
  tree->Branch("adc4", &adc4, "adc4/I");
  tree->Branch("adc5", &adc5, "adc5/I");
  tree->Branch("adc6", &adc6, "adc6/I");
  tree->Branch("tdc1", &tdc1, "tdc1/I");
  tree->Branch("tdc2", &tdc2, "tdc2/I");
  tree->Branch("tdc3", &tdc3, "tdc3/I");
  tree->Branch("tdc4", &tdc4, "tdc4/I");
  tree->Branch("tdc5", &tdc5, "tdc5/I");
  tree->Branch("tdc6", &tdc6, "tdc6/I");
  return;
}

void event1_(unsigned short * data) {
  adc1 = data[3]; // swapped data[2] <-- data[3]
  adc2 = data[2]; // swapped data[3] <-- data[2]
  adc3 = data[4];
  adc4 = data[5];
  adc5 = data[6];
  adc6 = data[7];
  tdc1 = data[8];
  tdc2 = data[9];
  tdc3 = data[10];
  tdc4 = data[11];
  tdc5 = data[12];
  tdc6 = data[13];
  tree->Fill();
  return;
}

void hstend_(){
  fout1->Write();
  fout1->Close();
  return;
}
