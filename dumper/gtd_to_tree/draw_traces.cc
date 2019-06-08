#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "TROOT.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraphErrors.h"

int main(int argc, char* argv[]){
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " input_root_name [output_root_name]" << std::endl;
    return 1;
  }
  TString input_file_name = argv[1];
  TString output_file_name;
  if (argc == 2) {
    output_file_name = input_file_name;
    if(output_file_name.EndsWith(".root")){
      output_file_name.Resize(output_file_name.Length()-5);
    }
    output_file_name += "_traces.root";
  }else{
    output_file_name = argv[2];
  }
  std::cout << "Input filename  : " << input_file_name << std::endl;
  std::cout << "Output filename : " << output_file_name << std::endl;
  std::ifstream ifs(input_file_name.Data(), std::ios::in | std::ios::binary);
  if (!ifs){
    std::cout << "The input file was not opened." << std::endl;
    return 1;
  }
  
  const UShort_t MaxNumOfWaveData    = 4096;
  const UShort_t MaxNumOfTimingMarks = 4096;

  TFile* input_file = new TFile(input_file_name, "read");
  TTree* input_tree = (TTree *)(input_file->Get("tr"));
  ULong64_t input_RawDataOffset;
  UShort_t  input_BoardID;		   
  UShort_t  input_Channel;		   
  UShort_t  input_HeaderType;		   
  ULong64_t input_Timestamp;		   
  Double_t  input_DiscTime;		   
  ULong64_t input_PrevDiscTime;	   
  UInt_t    input_PreRiseE;		   
  UInt_t    input_PostRiseE;		   
  ULong64_t input_Flags;		   
  UShort_t  input_BaseSample;		   
  UInt_t    input_SampledBaseline;	   
  UShort_t  input_PrevPostRiseBeginSample;
  UShort_t  input_PreRiseEndSample;	   
  UShort_t  input_PreRiseBeginSample;
  UShort_t  input_nWaveData;
  Short_t   input_WaveData[MaxNumOfWaveData];
  UShort_t  input_nTimingMarks;
  UShort_t  input_TimingMarks[MaxNumOfTimingMarks];
  input_tree->SetBranchAddress("RawDataOffset",           &input_RawDataOffset);
  input_tree->SetBranchAddress("BoardID",	          &input_BoardID);		   
  input_tree->SetBranchAddress("Channel",	          &input_Channel);		   
  input_tree->SetBranchAddress("HeaderType",              &input_HeaderType);		   
  input_tree->SetBranchAddress("Timestamp",               &input_Timestamp);		   
  input_tree->SetBranchAddress("DiscTime",	          &input_DiscTime);		   
  input_tree->SetBranchAddress("PrevDiscTime",            &input_PrevDiscTime);	   
  input_tree->SetBranchAddress("PreRiseE",	          &input_PreRiseE);		   
  input_tree->SetBranchAddress("PostRiseE",               &input_PostRiseE);		   
  input_tree->SetBranchAddress("Flags",	                  &input_Flags);		   
  input_tree->SetBranchAddress("BaseSample",              &input_BaseSample);		   
  input_tree->SetBranchAddress("SampledBaseline",         &input_SampledBaseline);	   
  input_tree->SetBranchAddress("PrevPostRiseBeginSample", &input_PrevPostRiseBeginSample);
  input_tree->SetBranchAddress("PreRiseEndSample",        &input_PreRiseEndSample);	   
  input_tree->SetBranchAddress("PreRiseBeginSample",      &input_PreRiseBeginSample);
  input_tree->SetBranchAddress("nWaveData",               &input_nWaveData);
  input_tree->SetBranchAddress("WaveData",                input_WaveData);
  input_tree->SetBranchAddress("nTimingMarks",            &input_nTimingMarks);
  input_tree->SetBranchAddress("TimingMarks",             input_TimingMarks);
  
  //ULong64_t RawDataOffset;
  //UShort_t  BoardID;		   
  //UShort_t  Channel;		   
  //UShort_t  HeaderType;		   
  //ULong64_t Timestamp;		   
  //Double_t  DiscTime;		   
  //ULong64_t PrevDiscTime;	   
  //UInt_t    PreRiseE;		   
  //UInt_t    PostRiseE;		   
  //ULong64_t Flags;		   
  //UShort_t  BaseSample;		   
  //UInt_t    SampledBaseline;	   
  //UShort_t  PrevPostRiseBeginSample;
  //UShort_t  PreRiseEndSample;	   
  //UShort_t  PreRiseBeginSample;     
  //UShort_t  nWaveData;
  //UShort_t  WaveData[MaxNumOfWaveData];
  //UShort_t  nTimingMarks;
  //UShort_t  TimingMarks[MaxNumOfTimingMarks];
  //output_tree->Branch("RawDataOffset",           &RawDataOffset);
  //output_tree->Branch("BoardID",                 &BoardID);
  //output_tree->Branch("Channel",                 &Channel);
  //output_tree->Branch("HeaderType",              &HeaderType);
  //output_tree->Branch("Timestamp",               &Timestamp);
  //output_tree->Branch("DiscTime",                &DiscTime);
  //output_tree->Branch("PrevDiscTime",            &PrevDiscTime);
  //output_tree->Branch("PreRiseE",                &PreRiseE);
  //output_tree->Branch("PostRiseE",               &PostRiseE);
  //output_tree->Branch("Flags",                   &Flags);
  //output_tree->Branch("BaseSample",              &BaseSample);
  //output_tree->Branch("SampledBaseline",         &SampledBaseline);
  //output_tree->Branch("PrevPostRiseBeginSample", &PrevPostRiseBeginSample);
  //output_tree->Branch("PreRiseEndSample",        &PreRiseEndSample);
  //output_tree->Branch("PreRiseBeginSample",      &PreRiseBeginSample);
  //output_tree->Branch("nWaveData",               &nWaveData);
  //output_tree->Branch("WaveData",                WaveData, "WaveData[nWaveData]/S");
  //output_tree->Branch("nTimingMarks",            &nTimingMarks);
  //output_tree->Branch("TimingMarks",             TimingMarks, "TimingMarks[nTimingMarks]/s");
  
  TFile *output_file = new TFile(output_file_name.Data(), "new");
  

  //TList *grlist = (TList *)output_file->GetList()->FindObject("grlist");
  //if (!grlist) {
  //  // I need a directory for graphs. See http://root.cern.ch/root/roottalk/roottalk02/0672.html
  //  grlist = new TList();
  //  grlist->SetName("grlist");
  //  output_file->Add(grlist);
  //}
  
  ULong64_t NumOfEntries = input_tree->GetEntries();
  std::cout << "input_tree->GetEntries(): " << NumOfEntries << std::endl;

  TGraph *gr = new TGraph();
  gr->SetName("traces");
  gr->SetTitle("traces");
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(0.3);

  //for (Int_t i = 0; i < NumOfEntries; i++) {
  for (Int_t i = 0; i < 10000; i++) {
    input_tree->GetEntry(i);
    if (input_Channel != 0) {
      continue;
    }
    if (i %100000 == 0) {
      std::cout << "Event number: " << i << std::endl;
      std::cout << "Timestamp: " << input_Timestamp << std::endl;
      std::cout << "nWaveData: " << input_nWaveData << std::endl;
    }
    for (Int_t j = 0; j < input_nWaveData; j++) {
      if (input_WaveData[j]>0) {
	gr->SetPoint(gr->GetN(),input_Timestamp+j,input_WaveData[j]);
      }else{
	gr->SetPoint(gr->GetN(),input_Timestamp+j,-input_WaveData[j]);
      }
    }
  }
  output_file->Add(gr);

  
  /* Ending transactions */
  input_file->Close();
  //grlist->Write();
  output_file->Write();
  output_file->Close();
  std::cout << "The program was finished." << std::endl;
  return 0;
}
  
