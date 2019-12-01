void print_with_saved_printer(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx == 0) {return;}
  gBrowserEx->PrintCanvasWithSavedPrinter();
  return;
}
