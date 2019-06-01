#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>

int print_bytes (std::ifstream &ifs,
                 unsigned long start_pos,
                 unsigned long unit_size) {
  ifs.seekg(start_pos);
  const int buffer_size = 16;
  unsigned char buffer[buffer_size];
  unsigned long cur_pos = start_pos;
  unsigned long end_pos = start_pos + unit_size;
  std::cout << std::hex << std::setfill('0');
  while (!ifs.eof()) {
    std::cout << "0x" << std::setw(8) << cur_pos << ": ";
    int read_size = buffer_size;
    if ((end_pos - cur_pos) < buffer_size) {
      read_size = end_pos - cur_pos;
    }
    ifs.read((char*)buffer, read_size);
    for (int i=0; i < read_size; i++){
      std::cout << std::setw(2) << (int)buffer[i] << " ";
    }
    for (int i=read_size; i<buffer_size; i++){
      std::cout << "   ";
    }
    std::cout << "| ";
    for (int i = 0; i < read_size; i++){
      if (((int)buffer[i] >= 0x20) && ((int)buffer[i] <= 0x7e)) {
        std::cout << buffer[i];
      }else{
        std::cout << " ";
      }
    }
    for (int i = read_size; i<buffer_size; i++){
      std::cout << " ";
    }
    std::cout << std::endl;
    cur_pos += read_size;
    if (cur_pos == end_pos) { break; }
  }
  return 0;
}

unsigned int SwapInt(unsigned int datum) {
  unsigned int t1 = 0, t2 = 0, t3 = 0;
  t1 = (datum&0x000000ff);
  t2 = (datum&0x0000ff00);
  t3 = (datum&0x00ff0000);
  return (t1<<24) + (t2<<8) + (t3>>8) + (datum>>24);
}

struct bld_header {
  unsigned char  bld_version[4];
  unsigned int   block_num;
  unsigned int   size1;
  unsigned int   size2;
  unsigned int   val1;
  unsigned int   val2;
  unsigned int   val3;
  unsigned int   size3;
};

int main(int argc, char* argv[]){
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " [-n] file_name" << std::endl;
    return 1;
  }
  std::cout << "Opening filename: " << argv[argc-1] << std::endl;
  std::ifstream ifs(argv[argc-1], std::ios::in | std::ios::binary);
  if (!ifs){
    std::cout << "The file was not opened." << std::endl;
    return 1;
  }
  unsigned int num = 10;
  if (argc == 3) { /* getopt() in unistd.h is not used for code portability. */
    if (argv[1][0] != '-') {
      std::cout << "Usage: " << argv[0] << " [-n] file_name" << std::endl;
    }
    std::stringstream ss((char*)argv[1]+1);
    ss >> num;
  }
  
  bld_header header;
  for (int i = 0; i < num; i++) {
    ifs.read((char*)&header, sizeof(header));
    char bld_version_with_null[5] = "vers";
    for (int j = 0; j < 4; j++){
      bld_version_with_null[j] = header.bld_version[j];
    }
    header.block_num = SwapInt(header.block_num);
    header.size1     = SwapInt(header.size1);
    header.size2     = SwapInt(header.size2);
    header.val1      = SwapInt(header.val1);
    header.val2      = SwapInt(header.val2);
    header.val3      = SwapInt(header.val3);
    header.size3     = SwapInt(header.size3);
    std::cout << std::hex;
    std::cout << "header.bld_version : "   << bld_version_with_null << std::endl;
    std::cout << "header.block_num   : 0x" << header.block_num      << std::endl;
    std::cout << "header.size1       : 0x" << header.size1          << std::endl;
    std::cout << "header.size2       : 0x" << header.size2          << std::endl;
    std::cout << "header.val1        : 0x" << header.val1           << std::endl;
    std::cout << "header.val2        : 0x" << header.val2           << std::endl;
    std::cout << "header.val3        : 0x" << header.val3           << std::endl;
    std::cout << "header.size3       : 0x" << header.size3          << std::endl;
    print_bytes(ifs,ifs.tellg(),header.size1+header.size2-sizeof(header));
    //print_bytes(ifs,ifs.tellg(),0x200);
    ifs.seekg(header.size3);
    std::cout << std::endl;
  }
  ifs.close();
  return 0;
}
