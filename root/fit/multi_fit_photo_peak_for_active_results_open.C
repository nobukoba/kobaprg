void multi_fit_photo_peak_for_active_results_open() {
  TNamed *named = (TNamed*)gROOT->FindObjectAny("initial_working_dir");
  if (named) {gSystem->cd(named->GetTitle());}
  std::cout << "gSystem->pwd(): " << gSystem->pwd() << std::endl;
  //gSystem->Exec("acroread root.pdf &");
  gSystem->Exec("evince fit_results.pdf &");
  return;
}
