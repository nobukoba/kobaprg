void start_auto_reading(){
  TClass *cl = (TClass*)gROOT->ProcessLine("TClass::GetClass(\"MyTFile\");");
  if (cl){
    gROOT->ProcessLine("MyTFile::StartConversion();");
  }else{
    std::cout <<  "MyTFile class is not defined."<< std::endl;
  }
  return;
}
