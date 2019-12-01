void print_list_of_files(){
  std::cout << std::endl << "Macro: print_list_of_files.C" << std::endl;
  TCollection *lst = gROOT->GetListOfFiles(); 
  TIter next(lst);
  TObject *obj;
  while ((obj=next())) {
    std::cout<<  obj->GetName() << std::endl;
  }
  return;
}
