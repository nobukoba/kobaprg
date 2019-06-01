#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

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
    std::cout << "Usage: " << argv[0] << " file_name1 [file_name2]..." << std::endl;
    return 0;
  }
  for (int i=1; i < argc; i++){
    std::cout << "Opening filename: " << argv[i] << std::endl;
    std::ifstream ifs(argv[i], std::ios::in | std::ios::binary);
  
    if (!ifs){
      std::cout << "The file was not opened." << std::endl;
      return 0;
    }

    unsigned long cur_pos = 0;
    unsigned long after_header_pos = 0;
    unsigned int  ival1, ival2;
    unsigned long lts;
  
    GEBArgonneHead   header;
    GEBArgonneCFDv18 event;

    std::cout << std::hex << std::setfill('0');

    for (int ievent=1; ievent < 10; ievent++){
      if (ifs.eof()) {break;}
      cur_pos = ifs.tellg();
      std::cout << "event number: " << std::setw(8) << ievent << std::endl;
      std::cout << "cur_pos: 0x"    << std::setw(8) << cur_pos
		<< " (" << std::dec << cur_pos << ") bytes"
		<< std::hex << std::endl;
      ifs.read((char*)&ival1, sizeof(ival1));
      ifs.read((char*)&ival2, sizeof(ival2));
      ifs.read((char*)&lts,   sizeof(lts));
      after_header_pos = ifs.tellg();

      ifs.read((char*)&header, sizeof(header));
      header.GA_packetlength           = SwapShort(header.GA_packetlength);
      header.ud_channel                = SwapShort(header.ud_channel);
      header.disc_low                  = SwapInt(header.disc_low);
      header.hdrlength_evttype_hdrtype = SwapShort(header.hdrlength_evttype_hdrtype);
      header.disc_high                 = SwapShort(header.disc_high);
      unsigned short fGA               = (header.GA_packetlength & 0xf800) >> 11;
      unsigned short fLength           = header.GA_packetlength & 0x7ff;
      unsigned short fBoardID          = (header.ud_channel & 0xfff0) >> 4;
      unsigned short fChannel          = (header.ud_channel & 0xf);
      unsigned int   fHeaderType       = header.hdrlength_evttype_hdrtype & 0xf;
      unsigned short fEventType        = (header.hdrlength_evttype_hdrtype & 0x380) >> 7;
      unsigned short fHeaderLength     = (header.hdrlength_evttype_hdrtype & 0xfc00) >> 10;
      unsigned long  fDisc             = (((unsigned long)header.disc_high) << 32) + ((unsigned long)header.disc_low);
  
      ifs.read((char*)&event,  sizeof(event));
      event.cfd_low_prev                 = SwapShort(event.cfd_low_prev);                 
      event.flags	                 = SwapShort(event.flags);			
      event.cfd_sample0			 = SwapShort(event.cfd_sample0);			
      event.cfd_mid_prev		 = SwapShort(event.cfd_mid_prev);			
      event.sampled_baseline		 = SwapInt  (event.sampled_baseline);		
      event.cfd_sample2			 = SwapShort(event.cfd_sample2);			
      event.cfd_sample1			 = SwapShort(event.cfd_sample1);			
      event.postrise_sum_low_prerise_sum = SwapInt  (event.postrise_sum_low_prerise_sum);	
      event.timestamp_peak_low		 = SwapShort(event.timestamp_peak_low);		
      event.postrise_sum_high            = SwapShort(event.postrise_sum_high);		
      event.timestamp_trigger_low	 = SwapShort(event.timestamp_trigger_low);	
      event.last_postrise_enter_sample	 = SwapShort(event.last_postrise_enter_sample);	
      event.postrise_end_sample		 = SwapShort(event.postrise_end_sample);		
      event.postrise_begin_sample	 = SwapShort(event.postrise_begin_sample);	
      event.prerise_end_sample		 = SwapShort(event.prerise_end_sample);		
      event.prerise_begin_sample	 = SwapShort(event.prerise_begin_sample);		
      event.base_sample			 = SwapShort(event.base_sample);			
      event.peak_sample			 = SwapShort(event.peak_sample);

      short          fGetCFD0                    = GetSigned14BitFromUShort(event.cfd_sample0);
      short          fGetCFD1                    = GetSigned14BitFromUShort(event.cfd_sample1);
      short          fGetCFD2                    = GetSigned14BitFromUShort(event.cfd_sample2);
      double         fGetCFD                     = (double) fDisc + (0. - fGetCFD2*2./(fGetCFD2-fGetCFD0));
      unsigned long  fGetPrevCFD;
      unsigned int   fGetBaseline                = ((event.sampled_baseline & 0x00FFFFFF) >> 0);
      unsigned int   fGetPreRiseE                = (event.postrise_sum_low_prerise_sum & 0xffffff);
      unsigned int   fGetPostRiseE               = ((event.postrise_sum_low_prerise_sum & 0xff000000)>>24) + (((unsigned int)event.postrise_sum_high) << 8);
      unsigned long  fGetTrigTimestamp           = ((unsigned long)event.timestamp_trigger_low) /*+ (((ULong_t)timestamp_trigger_high)<<16)*/; // not fully implemented
      unsigned short fGetLastPostRiseEnterSample = event.last_postrise_enter_sample & 0x3fff;
      unsigned short fGetPostRiseSampleBegin     = event.postrise_begin_sample & 0x3fff;
      unsigned short fGetPostRiseSampleEnd       = event.postrise_end_sample & 0x3fff;
      unsigned short fGetPreRiseSampleBegin      = event.prerise_begin_sample & 0x3fff;
      unsigned short fGetPreRiseSampleEnd        = event.prerise_end_sample & 0x3fff;
      unsigned short fGetBaseSample              = event.base_sample & 0x3fff;
      unsigned short fGetPeakSample              = event.peak_sample & 0x3fff;
      unsigned short fWriteFlag                  = (event.flags & 0x20)>>5;
      unsigned short fVetoFlag                   = (event.flags & 0x40)>>6;
      unsigned short fTSMatchFlag                = (event.flags & 0x80)>>7;
      unsigned short fExternalDiscFlag           = (event.flags & 0x100)>>8;
      unsigned short fPeakValidFlag              = (event.flags & 0x200)>>9;
      unsigned short fOffsetFlag                 = (event.flags & 0x400)>>10;
      unsigned short fCFDValidFlag               = (event.flags & 0x800)>>11;
      unsigned short fSyncErrorFlag              = (event.flags & 0x1000)>>12;
      unsigned short fGeneralErrorFlag           = (event.flags & 0x2000)>>13;
      unsigned short fPileUpOnlyFlag             = (event.flags & 0x4000)>>14;
      unsigned short fPileUpFlag                 = (event.flags & 0x8000)>>15;
      
      if (fTSMatchFlag == 1) {
	unsigned long current_cfd= fDisc;
	fGetPrevCFD = ((current_cfd & 0x00ffffc0000000) + (((unsigned long)(event.cfd_mid_prev & 0x3fff)) << 16) + (unsigned long)event.cfd_low_prev);
      } else {
	fGetPrevCFD = 0xffffffffffffffff;
      }
      
      std::cout << "ival1                               : 0x" << std::setw(8)  << ival1                               << std::endl;
      std::cout << "ival2                               : 0x" << std::setw(8)  << ival2                               << std::endl;
      std::cout << "lts                                 : 0x" << std::setw(16) << lts                                 << std::endl;
      std::cout << "header.GA_packetlength              : 0x" << std::setw(4)  << header.GA_packetlength              << std::endl;
      std::cout << "header.ud_channel                   : 0x" << std::setw(4)  << header.ud_channel                   << std::endl;
      std::cout << "header.disc_low                     : 0x" << std::setw(8)  << header.disc_low                     << std::endl;
      std::cout << "header.hdrlength_evttype_hdrtype    : 0x" << std::setw(8)  << header.hdrlength_evttype_hdrtype    << std::endl;
      std::cout << "header.disc_high                    : 0x" << std::setw(4)  << header.disc_high                    << std::endl;
      std::cout << "fGA                                 : 0x" << std::setw(4)  << fGA                                 << std::endl;
      std::cout << "fLength                             : 0x" << std::setw(4)  << fLength                             << std::endl;
      std::cout << "fBoardID                            : 0x" << std::setw(4)  << fBoardID                            << std::endl;
      std::cout << "fChannel                            : 0x" << std::setw(4)  << fChannel                            << std::endl;
      std::cout << "fHeaderType                         : 0x" << std::setw(4)  << fHeaderType                         << std::endl;
      std::cout << "fEventType                          : 0x" << std::setw(4)  << fEventType                          << std::endl;
      std::cout << "fHeaderLength                       : 0x" << std::setw(4)  << fHeaderLength                       << std::endl;
      std::cout << "fDisc                               : 0x" << std::setw(16) << fDisc                               << std::endl;
      std::cout << "event.cfd_low_prev                  : 0x" << std::setw(4)  << event.cfd_low_prev                  << std::endl;
      std::cout << "event.flags                         : 0x" << std::setw(4)  << event.flags                         << std::endl;
      std::cout << "event.cfd_sample0                   : 0x" << std::setw(4)  << event.cfd_sample0                   << std::endl;
      std::cout << "event.cfd_mid_prev                  : 0x" << std::setw(4)  << event.cfd_mid_prev                  << std::endl;
      std::cout << "event.sampled_baseline              : 0x" << std::setw(8)  << event.sampled_baseline              << std::endl;
      std::cout << "event.cfd_sample2                   : 0x" << std::setw(4)  << event.cfd_sample2                   << std::endl;
      std::cout << "event.cfd_sample1                   : 0x" << std::setw(4)  << event.cfd_sample1                   << std::endl;
      std::cout << "event.postrise_sum_low_prerise_sum  : 0x" << std::setw(8)  << event.postrise_sum_low_prerise_sum  << std::endl;
      std::cout << "event.timestamp_peak_low            : 0x" << std::setw(4)  << event.timestamp_peak_low            << std::endl;
      std::cout << "event.postrise_sum_high             : 0x" << std::setw(4)  << event.postrise_sum_high             << std::endl;
      std::cout << "event.timestamp_trigger_low         : 0x" << std::setw(4)  << event.timestamp_trigger_low         << std::endl;
      std::cout << "event.last_postrise_enter_sample    : 0x" << std::setw(4)  << event.last_postrise_enter_sample    << std::endl;
      std::cout << "event.postrise_end_sample           : 0x" << std::setw(4)  << event.postrise_end_sample           << std::endl;
      std::cout << "event.postrise_begin_sample         : 0x" << std::setw(4)  << event.postrise_begin_sample         << std::endl;
      std::cout << "event.prerise_end_sample            : 0x" << std::setw(4)  << event.prerise_end_sample            << std::endl;
      std::cout << "event.prerise_begin_sample          : 0x" << std::setw(4)  << event.prerise_begin_sample          << std::endl;
      std::cout << "event.base_sample                   : 0x" << std::setw(4)  << event.base_sample                   << std::endl;
      std::cout << "event.peak_sample                   : 0x" << std::setw(4)  << event.peak_sample                   << std::endl;
      std::cout << "fGetCFD0                            : 0x" << std::setw(4)  << fGetCFD0                            << std::endl;
      std::cout << "fGetCFD1                            : 0x" << std::setw(4)  << fGetCFD1                            << std::endl;
      std::cout << "fGetCFD2                            : 0x" << std::setw(4)  << fGetCFD2                            << std::endl;
      std::cout << "fGetCFD                             : "   << std::dec      << fGetCFD << std::hex                 << std::endl;
      std::cout << "fGetPrevCFD                         : 0x" << std::setw(16) << fGetPrevCFD                         << std::endl;
      std::cout << "fGetBaseline                        : 0x" << std::setw(8)  << fGetBaseline                        << std::endl;
      std::cout << "fGetPreRiseE                        : 0x" << std::setw(8)  << fGetPreRiseE                        << std::endl;
      std::cout << "fGetPostRiseE                       : 0x" << std::setw(8)  << fGetPostRiseE                       << std::endl;
      std::cout << "fGetTrigTimestamp                   : 0x" << std::setw(16) << fGetTrigTimestamp                   << std::endl;
      std::cout << "fGetLastPostRiseEnterSample         : 0x" << std::setw(4)  << fGetLastPostRiseEnterSample         << std::endl;
      std::cout << "fGetPostRiseSampleBegin             : 0x" << std::setw(4)  << fGetPostRiseSampleBegin             << std::endl;
      std::cout << "fGetPostRiseSampleEnd               : 0x" << std::setw(4)  << fGetPostRiseSampleEnd               << std::endl;
      std::cout << "fGetPreRiseSampleBegin              : 0x" << std::setw(4)  << fGetPreRiseSampleBegin              << std::endl;
      std::cout << "fGetPreRiseSampleEnd                : 0x" << std::setw(4)  << fGetPreRiseSampleEnd                << std::endl;
      std::cout << "fGetBaseSample                      : 0x" << std::setw(4)  << fGetBaseSample                      << std::endl;
      std::cout << "fGetPeakSample                      : 0x" << std::setw(4)  << fGetPeakSample                      << std::endl;
      std::cout << "fWriteFlag                          : 0x" << std::setw(4)  << fWriteFlag                          << std::endl;
      std::cout << "fVetoFlag                           : 0x" << std::setw(4)  << fVetoFlag                           << std::endl;
      std::cout << "fTSMatchFlag                        : 0x" << std::setw(4)  << fTSMatchFlag                        << std::endl;
      std::cout << "fExternalDiscFlag                   : 0x" << std::setw(4)  << fExternalDiscFlag                   << std::endl;
      std::cout << "fPeakValidFlag                      : 0x" << std::setw(4)  << fPeakValidFlag                      << std::endl;
      std::cout << "fOffsetFlag                         : 0x" << std::setw(4)  << fOffsetFlag                         << std::endl;
      std::cout << "fCFDValidFlag                       : 0x" << std::setw(4)  << fCFDValidFlag                       << std::endl;
      std::cout << "fSyncErrorFlag                      : 0x" << std::setw(4)  << fSyncErrorFlag                      << std::endl;
      std::cout << "fGeneralErrorFlag                   : 0x" << std::setw(4)  << fGeneralErrorFlag                   << std::endl;
      std::cout << "fPileUpOnlyFlag                     : 0x" << std::setw(4)  << fPileUpOnlyFlag                     << std::endl;
      std::cout << "fPileUpFlag                         : 0x" << std::setw(4)  << fPileUpFlag                         << std::endl;
      std::cout << std::endl;
      ifs.seekg(cur_pos+ival2);
    }
    ifs.close();
  }
  return 0;
}
