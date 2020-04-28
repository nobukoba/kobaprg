#include <iostream>
class TestClassNobu;
void compiling_with_user_class(){
  std::cout << "aa" << std::endl;
  /* After .L multi_load_of_classes.C+,
     the following does not work!!
     .L compiling_with_user_class.C+
     ACLiC needs to include the file of TestClassNobu (not forward declaration)!!
  */
  TestClassNobu * tc = new TestClassNobu();
  return;
}
