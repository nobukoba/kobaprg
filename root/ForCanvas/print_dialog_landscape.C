void print_dialog_landscape(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx == 0) {return;}
  gBrowserEx->PrintCanvasWithOption("Landscape");
  return;
}
