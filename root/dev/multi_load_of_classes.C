#include <iostream>
class TestClassNobu{
public:
  TestClassNobu(){
    std::cout << "TestClassNobu" << std::endl;
  }
  ~TestClassNobu(){
  }
  //ClassDef(TestClassNobu,1);
};
//ClassImp(TestClassNobu)

void multi_load_of_classes(){
  std::cout << "aa" << std::endl;
  
  return;
}


