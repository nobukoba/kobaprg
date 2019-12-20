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
  UShort_t  input_PostRiseEndSample;	   
  UShort_t  input_PostRiseBeginSample;
  UShort_t  input_PreRiseEndSample;	   
  UShort_t  input_PreRiseBeginSample;
  UShort_t  input_nWaveData;
  UShort_t  input_WaveData[MaxNumOfWaveData];
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
  input_tree->SetBranchAddress("PostRiseEndSample",       &input_PostRiseEndSample);	   
  input_tree->SetBranchAddress("PostRiseBeginSample",     &input_PostRiseBeginSample);
  input_tree->SetBranchAddress("PreRiseEndSample",        &input_PreRiseEndSample);	   
  input_tree->SetBranchAddress("PreRiseBeginSample",      &input_PreRiseBeginSample);
  input_tree->SetBranchAddress("nWaveData",               &input_nWaveData);
  input_tree->SetBranchAddress("WaveData",                input_WaveData);
  input_tree->SetBranchAddress("nTimingMarks",            &input_nTimingMarks);
  input_tree->SetBranchAddress("TimingMarks",             input_TimingMarks);
  
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

  const Int_t ngr = 6;
  TGraph *gr[ngr];

  for (Int_t i = 0; i < ngr; i++) {
    gr[i] = new TGraph();
    gr[i]->SetMarkerStyle(20);
    gr[i]->SetMarkerSize(1.0);
  }

  gr[0]->SetName("traces");
  gr[0]->SetMarkerSize(0.3);
  gr[1]->SetName("PrevDisc");
  gr[1]->SetMarkerColor(2);
  gr[2]->SetName("PreRiseBegin");
  gr[2]->SetMarkerColor(3);
  gr[3]->SetName("PreRiseEnd");
  gr[3]->SetMarkerColor(4);
  gr[4]->SetName("PostRiseBegin");
  gr[4]->SetMarkerColor(5);
  gr[5]->SetName("PostRiseEnd");
  gr[5]->SetMarkerColor(6);
  
  Int_t d_window           =   6; // 6 sample * 10 ns/sample = 60 ns
  Int_t k_window           =  20; //  200 ns
  Int_t m_window           = 350; // 3500 ns
  Int_t k0_window          =  80; //  800 ns
  Int_t d2_window          =  19; //  190 ns
  Int_t d3_window          =  20; //  200 ns
  Int_t delay_for_wave_win =  26; //  260 ns
  Int_t d_add              =   1; //   10 ns
  
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
    //std::cout << "input_PreRiseEndSample: " << input_PreRiseEndSample << std::endl;
    for (Int_t j = 0; j < input_nWaveData; j++) {
      ULong64_t jtime = j+input_Timestamp-d_window-d2_window-d3_window-delay_for_wave_win-d_add-k0_window-k_window; // offset = 172 samples
      gr[0]->SetPoint(gr[0]->GetN(), jtime, input_WaveData[j]);
      //if (input_PreRiseEndSample==input_WaveData[j]) {
      //  std::cout << "j: " << j << " jtime: " << jtime << " input_WaveData[j]: " << input_WaveData[j] << std::endl;
      //}
      if(jtime == input_Timestamp-d_window-d2_window-d3_window-d_add) {
        if (input_PreRiseEndSample!=input_WaveData[j]) {
          std::cout << "input_PreRiseEndSample: " << input_PreRiseEndSample << std::endl;
          std::cout << "jtime: " << jtime << " input_WaveData[j]: " << input_WaveData[j] << std::endl;
        }
      }
      if(jtime == input_Timestamp+k0_window+k_window) {
        if (input_PostRiseEndSample!=input_WaveData[j]) {
          std::cout << "input_PostRiseEndSample: " << input_PostRiseEndSample << std::endl;
          std::cout << "jtime: " << jtime << " input_WaveData[j]: " << input_WaveData[j] << std::endl;
        }
      }
    }
    gr[1]->SetPoint(gr[1]->GetN(),input_PrevDiscTime+k0_window+k_window,input_PrevPostRiseBeginSample);
    gr[2]->SetPoint(gr[2]->GetN(),input_Timestamp-d_window-d2_window-d3_window-d_add-m_window,input_PreRiseBeginSample);
    gr[3]->SetPoint(gr[3]->GetN(),input_Timestamp-d_window-d2_window-d3_window-d_add,input_PreRiseEndSample);
    gr[4]->SetPoint(gr[4]->GetN(),input_Timestamp+k0_window+k_window,input_PostRiseEndSample);
    gr[5]->SetPoint(gr[5]->GetN(),input_Timestamp+k0_window+k_window+m_window,input_PostRiseBeginSample);
  }
  for (Int_t i = 0; i < ngr; i++) {
    output_file->Add(gr[i]);
  }

  
  /* Ending transactions */
  input_file->Close();
  //grlist->Write();
  output_file->Write();
  output_file->Close();
  std::cout << "The program was finished." << std::endl;
  return 0;
}
  
