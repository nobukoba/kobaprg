#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

void very_simple_raw_data_dumper(const char* filename){
  std::cout << "Macro: very_simple_raw_data_dumper(char* filname)" << std::endl;
  std::cout << "Opening filename: " << filename << std::endl;
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  
  if (!ifs){
    std::cout << "The file was not opened." << std::endl;
    std::cout << "Push [Enter] key to end this macro." << std::endl;
    std::cin.get();
    return;
  }
  
  const int buffer_size = 16;
  unsigned char buffer[buffer_size];
  unsigned long val1, val2;
  //for (int i = 0; i < 256; i ++) {
  //  std::cout << std::hex <<i << ": '" <<  std::setw(2) << (char)i << "'" << std::endl;
  //}
  
  
  std::cout << std::hex << std::setfill('0');
  for (int line=0; line<1000; line++){
    ifstream::pos_type pos;
    pos = ifs.tellg();
    std::cout << std::setw(8) << line  << ": ";
    ifs.read((char*)buffer, buffer_size);
    for (int i=0; i<buffer_size; i++){
      std::cout << std::setw(2) << (int)buffer[i] << " ";
    }
    std::cout << "| ";
    for (int i=0; i<buffer_size; i++){
      if ((int)buffer[i] >= 0x20 && (int)buffer[i] <= 0x7e) {
	std::cout << buffer[i];
      }else{
	std::cout << " ";
      }
    }
    std::cout << std::endl;
  }
  std::cout << "Push [Enter] key to end this macro." << std::endl;
  std::cin.get();
  return;
}

void very_simple_raw_data_dumper(){
  std::cout << "Macro: very_simple_raw_data_dumper()" << std::endl;
  //very_simple_raw_data_dumper("./data/run_2100.gtd05_000_0120");
  //very_simple_raw_data_dumper("./data/run_2100.gtd05_001_0120");
  very_simple_raw_data_dumper("./data/cagra/E450/run_2100.gtd05_001_0120");
  //very_simple_raw_data_dumper("./data/run_2100.save");
  return;
}
