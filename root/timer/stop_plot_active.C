void stop_plot_active(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx==0) {std::cout << "No gBrowserEx." << std::endl; return;}
  TTimer * timer = gBrowserEx->GetTimer();
  delete timer;
  gBrowserEx->SetTimer(0);
  return;
}
