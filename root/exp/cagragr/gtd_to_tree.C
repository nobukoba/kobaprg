#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

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
  
  unsigned int  ival1, ival2;
  unsigned long lts;
  unsigned long unit = 0;
  unsigned long pre_pos = 0;
  unsigned long cur_pos = 0;
  const int buffer_size = 256;
  unsigned char buffer[buffer_size];
  unsigned long val1, val2;
  
  std::cout << std::hex << std::setfill('0');
  
  while(!ifs.eof()){
    std::cout << "unit: 0x" << setw(8) << unit << std::endl;
    std::cout << "cur_pos: 0x" << setw(8) << cur_pos << std::endl;
    //std::cout << "cur_pos: " << std::dec << setw(8) << cur_pos << std::endl;
    ifs.read((char*)&ival1, sizeof(ival1)); cur_pos += sizeof(ival1);
    ifs.read((char*)&ival2, sizeof(ival2)); cur_pos += sizeof(ival2);
    ifs.read((char*)&lts,   sizeof(lts));   cur_pos += sizeof(lts);
    std::cout << "ival1: 0x" << setw(8)  << ival1 << std::endl;
    std::cout << "ival2: 0x" << setw(8)  << ival2 << std::endl;
    std::cout << "lts: 0x"   << setw(16) << lts << std::endl;
    if (ival2 > buffer_size) {
      std::cout << "buffer_size is larger than the read size" << std::endl;
      return;
    }
    ifs.read((char*)buffer, ival2); pre_pos = cur_pos; cur_pos += ival2;
    for (unsigned int i=0; i<ival2; i++) {
      if ((i%16)==0){
	std::cout << "0x" << setw(8) << i+pre_pos << ": ";
      }
      std::cout << setw(2) << (int)buffer[i] << " ";
      if ((i%16)==15) {
	std::cout << std::endl;
      }
    }
    unit++;
    std::cout << std::endl;
    //if (ival2 != 0x40) {return;}
    if (ival1 != 0xe) {return;}
    //if (unit > 1000000) {return;}
  }
  
  std::cout << "Push [Enter] key to end this macro." << std::endl;
  std::cin.get();
  return;
}

void gtd_to_tree(){
  std::cout << "Macro: gtd_to_tree()" << std::endl;
  //gtd_to_tree("./data/run_2100.gtd05_000_0120");
  gtd_to_tree("./data/run_2100.gtd05_001_0120");
  //gtd_to_tree("./data/run_2100.save");
  return;
}
