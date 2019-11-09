void multi_fit_photo_peak_for_active_results_pdf_open() {
  TBrowserEx *gBrowserEx_tmp = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx_tmp) {
    gSystem->cd((gBrowserEx_tmp->GetInitialWorkingDir()).Data());
  }else{return;}
  std::cout << "gSystem->pwd(): " << gSystem->pwd() << std::endl;
  //gSystem->Exec("acroread root.pdf &");
  gSystem->Exec("evince fit_results.pdf &");
  return;
}
