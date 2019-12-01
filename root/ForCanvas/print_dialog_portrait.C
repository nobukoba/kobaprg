void print_dialog_portrait(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx == 0) {return;}
  gBrowserEx->PrintCanvasWithOption("Portrait");
  return;
}
