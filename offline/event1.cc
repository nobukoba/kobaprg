#include <iostream>
#include "TFile.h"
#include "TTree.h"

TFile* fout1;
TTree *tree;
int adc1, adc2, adc3, adc4, adc5, adc6;
int tdc1, tdc2, tdc3, tdc4, tdc5, tdc6;
double e1, e2, e3, e4, e5, e6;
double e123, e456;
double tdiff2_1, tdiff3_1, tdiff3_2, tdiff5_4, tdiff6_4, tdiff6_5;

void hstdef_(char * filename){
  fout1 = new TFile(filename, "recreate");
  tree = new TTree("tr", "tr");
  tree->SetAutoFlush(0);
  
  tree->Branch("adc1", &adc1);
  tree->Branch("adc2", &adc2);
  tree->Branch("adc3", &adc3);
  tree->Branch("adc4", &adc4);
  tree->Branch("adc5", &adc5);
  tree->Branch("adc6", &adc6);
  tree->Branch("tdc1", &tdc1);
  tree->Branch("tdc2", &tdc2);
  tree->Branch("tdc3", &tdc3);
  tree->Branch("tdc4", &tdc4);
  tree->Branch("tdc5", &tdc5);
  tree->Branch("tdc6", &tdc6);

  tree->Branch("e1", &e1);
  tree->Branch("e2", &e2);
  tree->Branch("e3", &e3);
  tree->Branch("e4", &e4);
  tree->Branch("e5", &e5);
  tree->Branch("e6", &e6);
  tree->Branch("e123", &e123);
  tree->Branch("e456", &e456);

  tree->Branch("tdiff2_1", &tdiff2_1);
  tree->Branch("tdiff3_1", &tdiff3_1);
  tree->Branch("tdiff3_2", &tdiff3_2);
  tree->Branch("tdiff5_4", &tdiff5_4);
  tree->Branch("tdiff6_4", &tdiff6_4);
  tree->Branch("tdiff6_5", &tdiff6_5);
  
  return;
}

void event1_(unsigned short * data) {
  adc1 = data[3]; // cable was swapped btw data[2] & data[3]
  adc2 = data[2]; // cable was swapped btw data[2] & data[3]
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

  e1 = 0.0031 * adc1 -0.2096;
  e2 = 0.003  * adc2 -0.1927;
  e3 = 0.0029 * adc3 -0.1251;
  e4 = 0.003  * adc4 -0.2148;
  e5 = 0.0029 * adc5 -0.16;
  e6 = 0.0031 * adc6 -0.2178;

  e123 = e1 + e2 + e3;
  e456 = e4 + e5 + e6;

  tdiff2_1 = tdc2 - tdc1;
  tdiff3_1 = tdc3 - tdc1;
  tdiff3_2 = tdc3 - tdc2;
  tdiff5_4 = tdc5 - tdc4;
  tdiff6_4 = tdc6 - tdc4;
  tdiff6_5 = tdc6 - tdc5;
  
  tree->Fill();
  return;
}

void hstend_(){
  fout1->Write();
  fout1->Close();
  return;
}
