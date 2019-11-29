void print_dialog(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  gStyle->SetPaperSize(20,26);
  ((TRootCanvas*)gBrowserEx->GetBrowserImp()->GetMainFrame())->PrintCanvas(); 
  return;
}
