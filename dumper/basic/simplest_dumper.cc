#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

int main(int argc, char* argv[]){
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " file_name" << std::endl;
    return 0;
  }
  std::ifstream ifs(argv[1], std::ios::in | std::ios::binary);
  std::cout << "Opening filename: " << argv[1] << std::endl;
  if (!ifs){
    std::cout << "The file was not opened." << std::endl;
    return 0;
  }
  
  const unsigned int buffer_size = 16;
  unsigned char buffer[buffer_size];  
  std::cout << "Bytes     :"
            << "  0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f |"
            << " 0123456789abcdef" << std::endl;
  std::cout << std::hex << std::setfill('0');
  for (int line=0; line<100000; line++){
    std::cout << "0x" << std::setw(8) << line*16 << ": ";
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
  ifs.close();
  return 1;
}
