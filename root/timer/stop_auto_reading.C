void stop_auto_reading(){
  TClass *cl = (TClass*)gROOT->ProcessLine("TClass::GetClass(\"MyTFile\");");
  if (cl){
    gROOT->ProcessLine("MyTFile::GetShmTimer()->Stop();");
  }else{
    std::cout <<  "MyTFile class is not defined."<< std::endl;
  }
  return;
}
