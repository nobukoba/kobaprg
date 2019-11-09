#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

UShort_t SwapShort(UShort_t datum) {
  UShort_t temp = 0;
  temp = (datum&0x00ff);
  return (temp<<8) + (datum>>8);
}

UInt_t SwapInt(UInt_t datum) {
  UInt_t t1 = 0, t2 = 0, t3 = 0;
  t1 = (datum&0x000000ff);
  t2 = (datum&0x0000ff00);
  t3 = (datum&0x00ff0000);
  return (t1<<24) + (t2<<8) + (t3>>8) + (datum>>24);
}

struct GEBArgonneHead {
  UShort_t GA_packetlength;
  UShort_t ud_channel;
  UInt_t   disc_low;
  UShort_t hdrlength_evttype_hdrtype;
  UShort_t disc_high;
};

struct GEBArgonneCFDv18 {
  UShort_t cfd_low_prev;
  UShort_t flags;
  UShort_t cfd_sample0; // signed
  UShort_t cfd_mid_prev; // bits 16:29
  UInt_t   sampled_baseline;
  UShort_t cfd_sample2;
  UShort_t cfd_sample1;
  UInt_t   postrise_sum_low_prerise_sum;
  UShort_t timestamp_peak_low;
  UShort_t postrise_sum_high;
  UShort_t timestamp_trigger_low;
  UShort_t last_postrise_enter_sample;
  UShort_t postrise_end_sample;
  UShort_t postrise_begin_sample;
  UShort_t prerise_end_sample;
  UShort_t prerise_begin_sample;
  UShort_t base_sample;
  UShort_t peak_sample;
};

void gtd_to_tree(const char* filename){
  std::cout << "Macro: gtd_to_tree(char* filname)" << std::endl;
  std::cout << "Opening filename: " << filename << std::endl;
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  
  if (!ifs){
    std::cout << "The file was not opened." << std::endl;
    std::cout << "Push [Enter] key to end this macro." << std::endl;
    std::cin.get();
    return;
  }

  ULong_t unit = 0;
  ULong_t pre_pos = 0;
  ULong_t cur_pos = 0;
  //const Int_t buffer_size = 256;
  //UChar_t buffer[buffer_size];
  
  UInt_t  ival1, ival2;
  ULong_t lts;
  
  GEBArgonneHead   header;
  GEBArgonneCFDv18 event;
  
  std::cout << std::hex << std::setfill('0');
  
  while(!ifs.eof()){
    std::cout << "unit: 0x" << setw(8) << unit << std::endl;
    std::cout << "cur_pos: 0x" << setw(8) << cur_pos
	      << " (" << std::dec << cur_pos << ") bytes"
	      << std::hex << std::endl;
    ifs.read((char*)&ival1, sizeof(ival1)); cur_pos += sizeof(ival1);
    ifs.read((char*)&ival2, sizeof(ival2)); cur_pos += sizeof(ival2);
    ifs.read((char*)&lts,   sizeof(lts));   cur_pos += sizeof(lts);
    cur_pos += ival2;

    ifs.read((char*)&header, sizeof(header));
    header.GA_packetlength           = SwapShort(header.GA_packetlength);
    header.ud_channel                = SwapShort(header.ud_channel);
    header.disc_low                  = SwapInt(header.disc_low);
    header.hdrlength_evttype_hdrtype = SwapShort(header.hdrlength_evttype_hdrtype);
    header.disc_high                 = SwapShort(header.disc_high);
    UShort_t fGA           = (header.GA_packetlength & 0xf800) >> 11;
    UShort_t fLength       = header.GA_packetlength & 0x7ff;
    UShort_t fBoardID      = (header.ud_channel & 0xfff0) >> 4;
    UShort_t fChannel      = (header.ud_channel & 0xf);
    UInt_t   fHeaderType   = header.hdrlength_evttype_hdrtype & 0xf;
    UShort_t fEventType    = (header.hdrlength_evttype_hdrtype & 0x380) >> 7;
    UShort_t fHeaderLength = (header.hdrlength_evttype_hdrtype & 0xfc00) >> 10;
    ULong_t  fDisc         = (((ULong_t)header.disc_high) << 32) + ((ULong_t)header.disc_low);

    ifs.read((char*)&event,  sizeof(event));
    event.cfd_low_prev                  = SwapShort(event.cfd_low_prev);                 
    event.flags	                        = SwapShort(event.flags);			
    event.cfd_sample0			= SwapShort(event.cfd_sample0);			
    event.cfd_mid_prev			= SwapShort(event.cfd_mid_prev);			
    event.sampled_baseline		= SwapInt  (event.sampled_baseline);		
    event.cfd_sample2			= SwapShort(event.cfd_sample2);			
    event.cfd_sample1			= SwapShort(event.cfd_sample1);			
    event.postrise_sum_low_prerise_sum	= SwapInt  (event.postrise_sum_low_prerise_sum);	
    event.timestamp_peak_low		= SwapShort(event.timestamp_peak_low);		
    event.postrise_sum_high             = SwapShort(event.postrise_sum_high);		
    event.timestamp_trigger_low	        = SwapShort(event.timestamp_trigger_low);	
    event.last_postrise_enter_sample	= SwapShort(event.last_postrise_enter_sample);	
    event.postrise_end_sample		= SwapShort(event.postrise_end_sample);		
    event.postrise_begin_sample	        = SwapShort(event.postrise_begin_sample);	
    event.prerise_end_sample		= SwapShort(event.prerise_end_sample);		
    event.prerise_begin_sample		= SwapShort(event.prerise_begin_sample);		
    event.base_sample			= SwapShort(event.base_sample);			
    event.peak_sample			= SwapShort(event.peak_sample);

    UShort_t fGetLastPostRiseEnterSample = event.last_postrise_enter_sample & 0x3fff;
    UShort_t fGetPostRiseSampleBegin     = event.postrise_begin_sample & 0x3fff;
    UShort_t fGetPostRiseSampleEnd       = event.postrise_end_sample & 0x3fff;
    UShort_t fGetPreRiseSampleBegin      = event.prerise_begin_sample & 0x3fff;
    UShort_t fGetPreRiseSampleEnd        = event.prerise_end_sample & 0x3fff;
    UShort_t fGetBaseSample              = event.base_sample & 0x3fff;
    UShort_t fGetPeakSample              = event.peak_sample & 0x3fff;
    UShort_t fWriteFlag                  = (event.flags & 0x20)>>5;
    UShort_t fVetoFlag                   = (event.flags & 0x40)>>6;
    UShort_t fTSMatchFlag                = (event.flags & 0x80)>>7;
    UShort_t fExternalDiscFlag           = (event.flags & 0x100)>>8;
    UShort_t fPeakValidFlag              = (event.flags & 0x200)>>9;
    UShort_t fOffsetFlag                 = (event.flags & 0x400)>>10;
    UShort_t fCFDValidFlag               = (event.flags & 0x800)>>11;
    UShort_t fSyncErrorFlag              = (event.flags & 0x1000)>>12;
    UShort_t fGeneralErrorFlag           = (event.flags & 0x2000)>>13;
    UShort_t fPileUpOnlyFlag             = (event.flags & 0x4000)>>14;
    UShort_t fPileUpFlag                 = (event.flags & 0x8000)>>15;

    std::cout << "ival1                               : 0x" << setw(8)  << ival1                               << std::endl;
    std::cout << "ival2                               : 0x" << setw(8)  << ival2                               << std::endl;
    std::cout << "lts                                 : 0x" << setw(16) << lts                                 << std::endl;
    std::cout << "header.GA_packetlength              : 0x" << setw(4) << header.GA_packetlength               << std::endl;
    std::cout << "header.ud_channel                   : 0x" << setw(4) << header.ud_channel                    << std::endl;
    std::cout << "header.disc_low                     : 0x" << setw(8) << header.disc_low                      << std::endl;
    std::cout << "header.hdrlength_evttype_hdrtype    : 0x" << setw(8) << header.hdrlength_evttype_hdrtype     << std::endl;
    std::cout << "header.disc_high                    : 0x" << setw(4) << header.disc_high                     << std::endl;
    std::cout << "fGA                                 : 0x" << setw(4) << fGA                                  << std::endl;
    std::cout << "fLength                             : 0x" << setw(4) << fLength                              << std::endl;
    std::cout << "fBoardID                            : 0x" << setw(4) << fBoardID                             << std::endl;
    std::cout << "fChannel                            : 0x" << setw(4) << fChannel                             << std::endl;
    std::cout << "fHeaderType                         : 0x" << setw(4) << fHeaderType                          << std::endl;
    std::cout << "fEventType                          : 0x" << setw(4) << fEventType                           << std::endl;
    std::cout << "fHeaderLength                       : 0x" << setw(4) << fHeaderLength                        << std::endl;
    std::cout << "fDisc                               : 0x" << setw(4) << fDisc                                << std::endl;
    std::cout << "event.cfd_low_prev                  : 0x" << setw(4) << event.cfd_low_prev                   << std::endl;
    std::cout << "event.flags                         : 0x" << setw(4) << event.flags                          << std::endl;
    std::cout << "event.cfd_sample0                   : 0x" << setw(4) << event.cfd_sample0                    << std::endl;
    std::cout << "event.cfd_mid_prev                  : 0x" << setw(4) << event.cfd_mid_prev                   << std::endl;
    std::cout << "event.sampled_baseline              : 0x" << setw(8) << event.sampled_baseline               << std::endl;
    std::cout << "event.cfd_sample2                   : 0x" << setw(4) << event.cfd_sample2                    << std::endl;
    std::cout << "event.cfd_sample1                   : 0x" << setw(4) << event.cfd_sample1                    << std::endl;
    std::cout << "event.postrise_sum_low_prerise_sum  : 0x" << setw(8) << event.postrise_sum_low_prerise_sum   << std::endl;
    std::cout << "event.timestamp_peak_low            : 0x" << setw(4) << event.timestamp_peak_low             << std::endl;
    std::cout << "event.postrise_sum_high             : 0x" << setw(4) << event.postrise_sum_high              << std::endl;
    std::cout << "event.timestamp_trigger_low         : 0x" << setw(4) << event.timestamp_trigger_low          << std::endl;
    std::cout << "event.last_postrise_enter_sample    : 0x" << setw(4) << event.last_postrise_enter_sample     << std::endl;
    std::cout << "event.postrise_end_sample           : 0x" << setw(4) << event.postrise_end_sample            << std::endl;
    std::cout << "event.postrise_begin_sample         : 0x" << setw(4) << event.postrise_begin_sample          << std::endl;
    std::cout << "event.prerise_end_sample            : 0x" << setw(4) << event.prerise_end_sample             << std::endl;
    std::cout << "event.prerise_begin_sample          : 0x" << setw(4) << event.prerise_begin_sample           << std::endl;
    std::cout << "event.base_sample                   : 0x" << setw(4) << event.base_sample                    << std::endl;
    std::cout << "event.peak_sample                   : 0x" << setw(4) << event.peak_sample                    << std::endl;
    std::cout << "fGetLastPostRiseEnterSample         : 0x" << setw(4) << fGetLastPostRiseEnterSample          << std::endl;
    std::cout << "fGetPostRiseSampleBegin             : 0x" << setw(4) << fGetPostRiseSampleBegin              << std::endl;
    std::cout << "fGetPostRiseSampleEnd               : 0x" << setw(4) << fGetPostRiseSampleEnd                << std::endl;
    std::cout << "fGetPreRiseSampleBegin              : 0x" << setw(4) << fGetPreRiseSampleBegin               << std::endl;
    std::cout << "fGetPreRiseSampleEnd                : 0x" << setw(4) << fGetPreRiseSampleEnd                 << std::endl;
    std::cout << "fGetBaseSample                      : 0x" << setw(4) << fGetBaseSample                       << std::endl;
    std::cout << "fGetPeakSample                      : 0x" << setw(4) << fGetPeakSample                       << std::endl;
    std::cout << "fWriteFlag                          : 0x" << setw(4) << fWriteFlag                           << std::endl;
    std::cout << "fVetoFlag                           : 0x" << setw(4) << fVetoFlag                            << std::endl;
    std::cout << "fTSMatchFlag                        : 0x" << setw(4) << fTSMatchFlag                         << std::endl;
    std::cout << "fExternalDiscFlag                   : 0x" << setw(4) << fExternalDiscFlag                    << std::endl;
    std::cout << "fPeakValidFlag                      : 0x" << setw(4) << fPeakValidFlag                       << std::endl;
    std::cout << "fOffsetFlag                         : 0x" << setw(4) << fOffsetFlag                          << std::endl;
    std::cout << "fCFDValidFlag                       : 0x" << setw(4) << fCFDValidFlag                        << std::endl;
    std::cout << "fSyncErrorFlag                      : 0x" << setw(4) << fSyncErrorFlag                       << std::endl;
    std::cout << "fGeneralErrorFlag                   : 0x" << setw(4) << fGeneralErrorFlag                    << std::endl;
    std::cout << "fPileUpOnlyFlag                     : 0x" << setw(4) << fPileUpOnlyFlag                      << std::endl;
    std::cout << "fPileUpFlag                         : 0x" << setw(4) << fPileUpFlag                          << std::endl;

    ifs.seekg(cur_pos);
    
    //if (ival2 > buffer_size) {
    //  std::cout << "buffer_size is larger than the read size" << std::endl;
    //  return;
    //}
    //ifs.read((char*)buffer, ival2-sizeof(sval2)-sizeof(sval2)); pre_pos = cur_pos; cur_pos += ival2;
    //for (unsigned int i=0; i<(ival2-sizeof(sval2)-sizeof(sval2)); i++) {
    //  if ((i%16)==0){
    //	std::cout << "0x" << setw(8) << i+pre_pos << ": ";
    //  }
    //  std::cout << setw(2) << (int)buffer[i] << " ";
    //  if ((i%16)==15) {
    //	std::cout << std::endl;
    //  }
    //}
    unit++;
    std::cout << std::endl;
    //if (ival2 != 0x40) {return;}
    //if (ival1 != 0xe) {return;}
    //if (unit > 1000000) {return;}
  }
  
  std::cout << "Push [Enter] key to end this macro." << std::endl;
  std::cin.get();
  return;
}

void gtd_to_tree(){
  std::cout << "Macro: gtd_to_tree()" << std::endl;
  //gtd_to_tree("./data/run_2100.gtd05_000_0120");
  gtd_to_tree("./data/cagra/E450/run_2100.gtd05_001_0120");
  //gtd_to_tree("./data/run_2100.save");
  return;
}
