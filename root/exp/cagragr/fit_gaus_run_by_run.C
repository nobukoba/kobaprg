void fit_gaus_run_by_run(){
  std::cout << std::endl << "Macro: fit_gaus_run_by_run.C "<< std::endl;
  TList * lof = gROOT->GetListOfFiles();
  TIter next(lof);
  TFile *file;
  while (file = (TFile*)next()){
    std::cout << "file->GetName(): " << file->GetName() << std::endl;
    file->GetListOfKeys();

  }
  return;
}
