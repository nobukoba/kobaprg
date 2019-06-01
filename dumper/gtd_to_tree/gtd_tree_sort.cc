#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "TString.h"
#include "TFile.h"
#include "TTree.h"

int main(int argc, char* argv[]){
  if (argc < 3) {
    std::cout << "Usage: " << argv[0] << " output_root_name input_root_name1 [input_root_name2]..." << std::endl;
    return 1;
  }
  
  const Int_t max_num_of_files = 256;
  TString input_file_name[max_num_of_files];
  TString output_file_name = argv[1];
  Int_t   num_of_files     = argc-2;
  std::cout << "Output filename    : " << output_file_name << std::endl;
  for (Int_t i = 0; i < num_of_files; i++) {
    input_file_name[i] = argv[i+2];
    std::cout << "Input filename #" << std::setfill('0') << std::setw(2) << i
              << " : " << input_file_name[i] << std::endl;
  }
  std::cout << std::setfill(' ');
  TFile *input_file[max_num_of_files];
  TTree *input_tree[max_num_of_files];
  ULong64_t input_RawDataOffset[max_num_of_files];
  UShort_t  input_BoardID[max_num_of_files];		   
  UShort_t  input_Channel[max_num_of_files];		   
  UShort_t  input_HeaderType[max_num_of_files];		   
  ULong64_t input_Timestamp[max_num_of_files];		   
  Double_t  input_DiscTime[max_num_of_files];		   
  ULong64_t input_PrevDiscTime[max_num_of_files];	   
  UInt_t    input_PreRiseE[max_num_of_files];		   
  UInt_t    input_PostRiseE[max_num_of_files];		   
  ULong64_t input_Flags[max_num_of_files];		   
  UShort_t  input_BaseSample[max_num_of_files];		   
  UInt_t    input_SampledBaseline[max_num_of_files];	   
  UShort_t  input_PrevPostRiseBeginSample[max_num_of_files];
  UShort_t  input_PreRiseEndSample[max_num_of_files];	   
  UShort_t  input_PreRiseBeginSample[max_num_of_files];
  for (Int_t i = 0; i < num_of_files; i++) {
    input_file[i] = new TFile(input_file_name[i], "read");
    input_tree[i] = (TTree *)(input_file[i]->Get("tr"));
    input_tree[i]->SetBranchAddress("RawDataOffset",            &(input_RawDataOffset[i]));
    input_tree[i]->SetBranchAddress("BoardID",	                &(input_BoardID[i]));		   
    input_tree[i]->SetBranchAddress("Channel",	                &(input_Channel[i]));		   
    input_tree[i]->SetBranchAddress("HeaderType",               &(input_HeaderType[i]));		   
    input_tree[i]->SetBranchAddress("Timestamp",       	        &(input_Timestamp[i]));		   
    input_tree[i]->SetBranchAddress("DiscTime",	                &(input_DiscTime[i]));		   
    input_tree[i]->SetBranchAddress("PrevDiscTime",             &(input_PrevDiscTime[i]));	   
    input_tree[i]->SetBranchAddress("PreRiseE",	                &(input_PreRiseE[i]));		   
    input_tree[i]->SetBranchAddress("PostRiseE",       	        &(input_PostRiseE[i]));		   
    input_tree[i]->SetBranchAddress("Flags",	                &(input_Flags[i]));		   
    input_tree[i]->SetBranchAddress("BaseSample",               &(input_BaseSample[i]));		   
    input_tree[i]->SetBranchAddress("SampledBaseline",          &(input_SampledBaseline[i]));	   
    input_tree[i]->SetBranchAddress("PrevPostRiseBeginSample",  &(input_PrevPostRiseBeginSample[i]));
    input_tree[i]->SetBranchAddress("PreRiseEndSample",         &(input_PreRiseEndSample[i]));	   
    input_tree[i]->SetBranchAddress("PreRiseBeginSample",       &(input_PreRiseBeginSample[i]));
  }
  
  TFile *output_file = new TFile(output_file_name, "new");
  TTree *output_tree = new TTree("tr", "tr");
  ULong64_t RawDataOffset;
  UShort_t  BoardID;		   
  UShort_t  Channel;		   
  UShort_t  HeaderType;		   
  ULong64_t Timestamp;		   
  Double_t  DiscTime;		   
  ULong64_t PrevDiscTime;	   
  UInt_t    PreRiseE;		   
  UInt_t    PostRiseE;		   
  ULong64_t Flags;		   
  UShort_t  BaseSample;		   
  UInt_t    SampledBaseline;	   
  UShort_t  PrevPostRiseBeginSample;
  UShort_t  PreRiseEndSample;	   
  UShort_t  PreRiseBeginSample;     
  output_tree->Branch("RawDataOffset",           &RawDataOffset);
  output_tree->Branch("BoardID",                 &BoardID);
  output_tree->Branch("Channel",                 &Channel);
  output_tree->Branch("HeaderType",              &HeaderType);
  output_tree->Branch("Timestamp",               &Timestamp);
  output_tree->Branch("DiscTime",                &DiscTime);
  output_tree->Branch("PrevDiscTime",            &PrevDiscTime);
  output_tree->Branch("PreRiseE",                &PreRiseE);
  output_tree->Branch("PostRiseE",               &PostRiseE);
  output_tree->Branch("Flags",                   &Flags);
  output_tree->Branch("BaseSample",              &BaseSample);
  output_tree->Branch("SampledBaseline",         &SampledBaseline);
  output_tree->Branch("PrevPostRiseBeginSample", &PrevPostRiseBeginSample);
  output_tree->Branch("PreRiseEndSample",        &PreRiseEndSample);
  output_tree->Branch("PreRiseBeginSample",      &PreRiseBeginSample);
  
  ULong64_t GetEntryPointer[max_num_of_files];
  ULong64_t NumOfEntries[max_num_of_files];
  for (Int_t i = 0; i < num_of_files; i++) {
    GetEntryPointer[i] = 0; /* For C++ beginners*/
    NumOfEntries[i] = input_tree[i]->GetEntries();
    std::cout << "input_tree[" << i << "]->GetEntries(): " << NumOfEntries[i] << std::endl;
  }
  for (Int_t i = 0; i < num_of_files; i++) {
    if (NumOfEntries[i] > 0) {
      input_tree[i]->GetEntry(0);
    }
  }
  ULong64_t FilesWithMinTS[max_num_of_files];
  //for (Int_t ite = 0; ite < 100; ite++) {
  while (true) {
    for (Int_t i = 0; i < num_of_files; i++) {
      FilesWithMinTS[i] = 0;
    }
    /* Find events with minimum TS */
    ULong64_t min_Timestamp = 0xffffffffffffffff;
    for (Int_t i = 0; i < num_of_files; i++) {
      /* std::cout << "i: "<< i << ", input_Timestamp[i]: " << input_Timestamp[i] << std::endl; */
      if (GetEntryPointer[i] >= NumOfEntries[i]) { continue; }
      if (input_Timestamp[i] < min_Timestamp) {
	min_Timestamp = input_Timestamp[i];
	for (Int_t j = 0; j < i; j++) {
	  FilesWithMinTS[j] = 0;
	}
	FilesWithMinTS[i] = 1;
      }else if (input_Timestamp[i] == min_Timestamp) {
	FilesWithMinTS[i] = 1;
      }
    }
    for (Int_t i = 0; i < num_of_files; i++) {
      if (FilesWithMinTS[i] == 0) { continue; }
      while (input_Timestamp[i]  == min_Timestamp) {
	RawDataOffset           = input_RawDataOffset[i];
	BoardID                 = input_BoardID[i];               
	Channel                 = input_Channel[i];                   
	HeaderType              = input_HeaderType[i];                   
	Timestamp               = input_Timestamp[i];              
	DiscTime                = input_DiscTime[i];              
	PrevDiscTime            = input_PrevDiscTime[i];                 
	PreRiseE                = input_PreRiseE[i];              
	PostRiseE               = input_PostRiseE[i];                      
	Flags                   = input_Flags[i];                        
	BaseSample              = input_BaseSample[i];                             
	SampledBaseline         = input_SampledBaseline[i];                    
	PrevPostRiseBeginSample = input_PrevPostRiseBeginSample[i];                 
	PreRiseEndSample        = input_PreRiseEndSample[i];              
	PreRiseBeginSample      = input_PreRiseBeginSample[i];
	/*
	std::cout << "Fill i: "<< i << ", input_Timestamp[i]: " << input_Timestamp[i] << std::endl;
	std::cout << "RawDataOffset: "<< RawDataOffset << std::endl;
	std::cout << "PreRiseE: "<< PreRiseE << std::endl;
	std::cout << "Channel: "<< Channel << std::endl; */
	output_tree->Fill();
	GetEntryPointer[i]++;
	if (GetEntryPointer[i] >= NumOfEntries[i]) { break; }
	input_tree[i]->GetEntry(GetEntryPointer[i]);
	if (GetEntryPointer[i]%100000==0){
	  std::cout << "File number  :   " << std::dec << std::setw(16) << i  << " / "
		    << std::setw(16) << num_of_files << std::endl;
	  std::cout << "Event numner :   " << std::dec << std::setw(16) << GetEntryPointer[i]
		    << " / " <<  std::setw(16) << NumOfEntries[i] << std::endl;
	  std::cout << "Timestamp[i] : 0x" << std::setfill('0') << std::hex << std::setw(16) << input_Timestamp[i] << std::endl;
	  std::cout << std::setfill(' ');
	}
      }
    }
    Int_t continue_flag = 0;
    for (Int_t i = 0; i < num_of_files; i++) {
      if (GetEntryPointer[i] < NumOfEntries[i]) {
	continue_flag++;
      }
    }
    if (continue_flag == 0) { break; }
  }
  
  /* Ending transactions */
  for (Int_t i = 0; i < num_of_files; i++) {
    input_file[i]->Close();
  }
  output_file->Write();
  output_file->Close();
  std::cout << "Sorting was finished." << std::endl;
  return 0;
}
