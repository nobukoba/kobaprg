#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

int print_bytes (std::ifstream &ifs,
                 unsigned long start_pos,
                 unsigned long unit_size) {
  ifs.seekg(start_pos);
  const unsigned int buffer_size = 16;
  unsigned char buffer[buffer_size];
  unsigned long num_of_lines = unit_size / buffer_size;
  unsigned long add_bytes    = unit_size % buffer_size;
  if (add_bytes > 0) { num_of_lines++; }
  std::cout << std::hex << std::setfill('0');
  for (int line = 0; line < num_of_lines; line++){
    std::cout << "0x" << std::setw(8) << start_pos + line*buffer_size << ": ";
    unsigned int read_size = buffer_size;
    if ((line == (num_of_lines - 1)) && (add_bytes > 0)) {
      read_size = add_bytes;
    }
    ifs.read((char*)buffer, read_size);
    for (int i=0; i<read_size; i++){
      std::cout << std::setw(2) << (int)buffer[i] << " ";
    }
    for (int i=read_size; i<buffer_size; i++){
      std::cout << "   ";
    }
    std::cout << "| ";
    for (int i=0; i<read_size; i++){
      if (((int)buffer[i] >= 0x20) &&
          ((int)buffer[i] <= 0x7e)) {
        std::cout << buffer[i];
      }else{
        std::cout << " ";
      }
    }    
    for (int i=read_size; i<buffer_size; i++){
      std::cout << " ";
    }
    std::cout << std::endl;
  }
  return 0;
}

int main(int argc, char* argv[]){
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " file_name" << std::endl;
    return 1;
  }
  std::ifstream ifs(argv[1], std::ios::in | std::ios::binary);
  std::cout << "Opening filename: " << argv[1] << std::endl;
  if (!ifs){
    std::cout << "The file was not opened." << std::endl;
    return 1;
  }
  print_bytes(ifs,0,0x8e);
  ifs.close();
  return 0;
}
