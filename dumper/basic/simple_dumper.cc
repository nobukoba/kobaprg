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
      if (((int)buffer[i] >= 0x20) &&
          ((int)buffer[i] <= 0x7e)) {
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
  unsigned int num = 0x100;
  if (argc == 3) { /* getopt() in unistd.h is not used for code portability. */
    if (argv[1][0] != '-') {
      std::cout << "Usage: " << argv[0] << " [-n] file_name" << std::endl;
    }
    std::stringstream ss((char*)(argv[1]+1));
    ss >> num;
  }

  print_bytes(ifs,0,num);
  ifs.close();
  return 0;
}
