#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"

unsigned short SwapShort(unsigned short datum) {
  unsigned short temp = 0;
  temp = (datum&0x00ff);
  return (temp<<8) + (datum>>8);
}

unsigned int SwapInt(unsigned int datum) {
  unsigned int t1 = 0, t2 = 0, t3 = 0;
  t1 = (datum&0x000000ff);
  t2 = (datum&0x0000ff00);
  t3 = (datum&0x00ff0000);
  return (t1<<24) + (t2<<8) + (t3>>8) + (datum>>24);
}

short GetSigned14BitFromUShort(unsigned short ushort) {
  return ((short)((ushort & 0x3fff) << 2))/4;
}

struct GEBArgonneHead {
  unsigned short GA_packetlength;
  unsigned short ud_channel;
  unsigned int   disc_low;
  unsigned short hdrlength_evttype_hdrtype;
  unsigned short disc_high;
};

struct GEBArgonneCFDv18 {
  unsigned short cfd_low_prev;
  unsigned short flags;
  unsigned short cfd_sample0; // signed
  unsigned short cfd_mid_prev; // bits 16:29
  unsigned int   sampled_baseline;
  unsigned short cfd_sample2;
  unsigned short cfd_sample1;
  unsigned int   postrise_sum_low_prerise_sum;
  unsigned short timestamp_peak_low;
  unsigned short postrise_sum_high;
  unsigned short timestamp_trigger_low;
  unsigned short last_postrise_enter_sample;
  unsigned short postrise_end_sample;
  unsigned short postrise_begin_sample;
  unsigned short prerise_end_sample;
  unsigned short prerise_begin_sample;
  unsigned short base_sample;
  unsigned short peak_sample;
};

int main(int argc, char* argv[]){
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " dat_file_name [output_root_name]" << std::endl;
    return 1;
  }
  
  TString output_file_name;
  if (argc == 2) {
    output_file_name = argv[1];
    output_file_name += ".root";
  }else{
    output_file_name = argv[2];
  }

  std::cout << "Input filename  : " << argv[1] << std::endl;
  std::cout << "Output filename : " << output_file_name << std::endl;
  
  TFile *output_file = new TFile(output_file_name.Data(), "recreate");
  TTree *tree = new TTree("tr", "tr");
  //tree->SetAutoFlush(0);
  
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

  tree->Branch("RawDataOffset",           &RawDataOffset            );
  tree->Branch("BoardID",                 &BoardID                  );
  tree->Branch("Channel",                 &Channel                  );
  tree->Branch("HeaderType",              &HeaderType               );
  tree->Branch("Timestamp",               &Timestamp                );
  tree->Branch("DiscTime",                &DiscTime                 );
  tree->Branch("PrevDiscTime",            &PrevDiscTime             );
  tree->Branch("PreRiseE",                &PreRiseE                 );
  tree->Branch("PostRiseE",               &PostRiseE                );
  tree->Branch("Flags",                   &Flags                    );
  tree->Branch("BaseSample",              &BaseSample               );
  tree->Branch("SampledBaseline",         &SampledBaseline          );
  tree->Branch("PrevPostRiseBeginSample", &PrevPostRiseBeginSample  );
  tree->Branch("PreRiseEndSample",        &PreRiseEndSample         );
  tree->Branch("PreRiseBeginSample",      &PreRiseBeginSample       );
  
  std::ifstream ifs(argv[1], std::ios::in | std::ios::binary);

  if (!ifs){
    std::cout << "The file was not opened." << std::endl;
    return 1;
  }
  
  unsigned long cur_pos = 0;
  unsigned long after_header_pos = 0;
  ifs.seekg(0, fstream::end);
  unsigned long file_size = ifs.tellg();
  ifs.seekg(0, fstream::beg);

  unsigned int  ival1, ival2;
  unsigned long lts;
  unsigned long ievent = 0;
  
  GEBArgonneHead   header;
  GEBArgonneCFDv18 event;
  
  std::cout << std::hex << std::setfill('0');
  while (!ifs.eof()) {
    ievent++;
    cur_pos = ifs.tellg();
    if (ievent%100000==0 && ievent < 1e12){
      std::cout << "Event: 0x" << std::setw(16) << ievent
    		<< ", Position: 0x"    << std::setw(16) << cur_pos
    		<< " (" << std::dec << cur_pos << ") bytes"
    		<< std::hex << std::endl;
    }
    ifs.read((char*)&ival1, sizeof(ival1));
    ifs.read((char*)&ival2, sizeof(ival2));
    ifs.read((char*)&lts,   sizeof(lts));
    after_header_pos = ifs.tellg();

    ifs.read((char*)&header, sizeof(header));
    header.ud_channel                = SwapShort(header.ud_channel);
    header.disc_low                  = SwapInt(header.disc_low);
    header.hdrlength_evttype_hdrtype = SwapShort(header.hdrlength_evttype_hdrtype);
    header.disc_high                 = SwapShort(header.disc_high);
    unsigned short fBoardID          = (header.ud_channel & 0xfff0) >> 4;
    unsigned short fChannel          = (header.ud_channel & 0xf);
    unsigned int   fHeaderType       = header.hdrlength_evttype_hdrtype & 0xf;
    unsigned long  fDisc             = (((unsigned long)header.disc_high) << 32) + ((unsigned long)header.disc_low);
    
    ifs.read((char*)&event,  sizeof(event));
    event.cfd_low_prev                  = SwapShort(event.cfd_low_prev);                 
    event.flags	                        = SwapShort(event.flags);			
    event.cfd_sample0			= SwapShort(event.cfd_sample0);			
    event.cfd_mid_prev			= SwapShort(event.cfd_mid_prev);			
    event.sampled_baseline		= SwapInt  (event.sampled_baseline);		
    event.cfd_sample2			= SwapShort(event.cfd_sample2);			
    event.cfd_sample1			= SwapShort(event.cfd_sample1);			
    event.postrise_sum_low_prerise_sum	= SwapInt  (event.postrise_sum_low_prerise_sum);	
    event.postrise_sum_high             = SwapShort(event.postrise_sum_high);		
    event.last_postrise_enter_sample	= SwapShort(event.last_postrise_enter_sample);	
    event.prerise_end_sample		= SwapShort(event.prerise_end_sample);		
    event.prerise_begin_sample		= SwapShort(event.prerise_begin_sample);		
    event.base_sample			= SwapShort(event.base_sample);			
    
    short          fGetCFD0                    = GetSigned14BitFromUShort(event.cfd_sample0);
    short          fGetCFD1                    = GetSigned14BitFromUShort(event.cfd_sample1);
    short          fGetCFD2                    = GetSigned14BitFromUShort(event.cfd_sample2);
    double         fGetCFD                     = (double) fDisc + (0. - fGetCFD2*2./(fGetCFD2-fGetCFD0));
    unsigned long  fGetPrevCFD;
    unsigned int   fGetBaseline                = ((event.sampled_baseline & 0x00FFFFFF) >> 0);
    unsigned int   fGetPreRiseE                = (event.postrise_sum_low_prerise_sum & 0xffffff);
    unsigned int   fGetPostRiseE               = ((event.postrise_sum_low_prerise_sum & 0xff000000)>>24) + (((unsigned int)event.postrise_sum_high) << 8);
    unsigned short fGetLastPostRiseEnterSample = event.last_postrise_enter_sample & 0x3fff;
    unsigned short fGetPreRiseSampleBegin      = event.prerise_begin_sample & 0x3fff;
    unsigned short fGetPreRiseSampleEnd        = event.prerise_end_sample & 0x3fff;
    unsigned short fGetBaseSample              = event.base_sample & 0x3fff;
    unsigned short fTSMatchFlag                = (event.flags & 0x80)>>7;
    
    if (fTSMatchFlag == 1) {
      unsigned long current_cfd= fDisc;
      fGetPrevCFD = ((current_cfd & 0x00ffffc0000000) + (((unsigned long)(event.cfd_mid_prev & 0x3fff)) << 16) + (unsigned long)event.cfd_low_prev);
    } else {
      fGetPrevCFD = 0xffffffffffffffff;
    }

    RawDataOffset           = cur_pos;
    BoardID                 = fBoardID;
    Channel                 = fChannel;
    HeaderType              = fHeaderType;
    Timestamp               = fDisc;
    DiscTime                = fGetCFD;
    PrevDiscTime            = fGetPrevCFD;
    PreRiseE                = fGetPreRiseE;
    PostRiseE               = fGetPostRiseE;
    Flags                   = event.flags;
    BaseSample              = fGetBaseSample;
    SampledBaseline         = fGetBaseline;
    PrevPostRiseBeginSample = fGetLastPostRiseEnterSample;
    PreRiseEndSample        = fGetPreRiseSampleEnd;
    PreRiseBeginSample      = fGetPreRiseSampleBegin;
    tree->Fill();
    
    /*
    std::cout << "fBoardID                            : 0x" << std::setw(4)  << fBoardID                            << std::endl;
    std::cout << "fChannel                            : 0x" << std::setw(4)  << fChannel                            << std::endl;
    std::cout << "fHeaderType                         : 0x" << std::setw(4)  << fHeaderType                         << std::endl;
    std::cout << "fDisc                               : 0x" << std::setw(16) << fDisc                               << std::endl;
    std::cout << "fGetCFD                             : "   << std::dec      << fGetCFD << std::hex                 << std::endl;
    std::cout << "fGetPrevCFD                         : 0x" << std::setw(16) << fGetPrevCFD                         << std::endl;
    std::cout << "fGetPreRiseE                        : 0x" << std::setw(8)  << fGetPreRiseE                        << std::endl;
    std::cout << "fGetPostRiseE                       : 0x" << std::setw(8)  << fGetPostRiseE                       << std::endl;
    std::cout << "event.flags                         : 0x" << std::setw(4)  << event.flags                         << std::endl;
    std::cout << "fGetBaseSample                      : 0x" << std::setw(4)  << fGetBaseSample                      << std::endl;
    std::cout << "fGetBaseline                        : 0x" << std::setw(8)  << fGetBaseline                        << std::endl;
    std::cout << "fGetLastPostRiseEnterSample         : 0x" << std::setw(4)  << fGetLastPostRiseEnterSample         << std::endl;
    std::cout << "fGetPreRiseSampleEnd                : 0x" << std::setw(4)  << fGetPreRiseSampleEnd                << std::endl;
    std::cout << "fGetPreRiseSampleBegin              : 0x" << std::setw(4)  << fGetPreRiseSampleBegin              << std::endl;
    std::cout << std::endl;*/
    if ((after_header_pos+ival2)==file_size) {
      std::cout << "Total event number: " << std::dec << ievent << std::endl;
      break;
    }else if((after_header_pos+ival2)>file_size){
      std::cout << "Total event number: " << std::dec << ievent << std::endl;
      std::cout << "The last event may be broken!" << std::endl;
      break;
    }
    ifs.seekg(after_header_pos+ival2);
  }
  ifs.close();
  output_file->Write();
  output_file->Close();
  return 0;
}
