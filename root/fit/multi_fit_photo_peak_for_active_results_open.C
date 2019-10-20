void multi_fit_photo_peak_for_active_results_open() {
  HistBrowser *pHistBrowser_tmp = (HistBrowser *)gROOT->ProcessLine("pHistBrowser;");
  if (pHistBrowser_tmp) {
    gSystem->cd((pHistBrowser_tmp->GetInitialWorkingDir()).Data());
  }else{return;}
  std::cout << "gSystem->pwd(): " << gSystem->pwd() << std::endl;
  //gSystem->Exec("acroread root.pdf &");
  gSystem->Exec("evince fit_results.pdf &");
  return;
}
