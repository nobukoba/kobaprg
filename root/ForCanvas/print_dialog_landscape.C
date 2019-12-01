void print_dialog_lansdcape(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx == 0) {return;}
  gBrowserEx->PrintCanvasWithOption("Portrait");
  return;
}
