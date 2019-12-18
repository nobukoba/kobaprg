void start_plot_active_func() {
  /*std::cout << "start_plot_active_func." << std::endl;*/

  TCanvas *canvas = gPad->GetCanvas();
  if (!canvas) {
    std::cout << "There is no canvas. This script is terminated." << std::endl;
    return;
  }
  TList *listofpri = canvas->GetListOfPrimitives();
  if (!listofpri) {
    std::cout << "There is nothing in the canvas. This script is terminated." << std::endl;
    return;
  }
  Int_t npad = 0;
  TObject *obj;
  TIter next(listofpri);
  while ((obj = next())) {
    if (obj->InheritsFrom(TPad::Class())) {
      npad++;
    }
  }
  
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TGListTree *hist_fListTree = (TGListTree *)gBrowserEx->GetHistListTree();
  if (!hist_fListTree) {
    std::cout << "hist_fListTree is null. Maybe gBrowserEx is also null. This script is terminated." << std::endl;
    return;
  }
  
  Int_t cur_pad = 1;
  if(npad == 0){cur_pad = 0;}
  for (Int_t i = cur_pad; i<=npad;i++) {
    TVirtualPad * pad = canvas->cd(i);
    pad->Clear("");
  }
  
  TList * loa = gBrowserEx->GetHistListTreeActiveHistos();
  Int_t num_hist = loa->GetEntries();
  Int_t cur_hist = gBrowserEx->GetCurrentHistNumber();
  TIter next2(loa);
  Int_t k = 0;
  for (Int_t i = cur_pad; i <=npad; i++){
    TObjString* objstr = (TObjString*)loa->At(cur_hist + k);
    TGListTreeItem *cur_ListTreeItem = (TGListTreeItem *) objstr->GetString().Atoll();
    canvas->cd(i);
    hist_fListTree->DoubleClicked(cur_ListTreeItem,1);
    k++;
    if ((cur_hist+k) == num_hist){
      break;
    }
  }
  cur_hist += k;
  if(cur_hist == num_hist){
    cur_hist = 0;
  }
  gBrowserEx->SetCurrentHistNumber(cur_hist);
  
  canvas->Modified();
  canvas->Update();
  gPad->Update();
  TIter next3(listofpri);
  while ((obj = next3())) {
    if (obj->InheritsFrom(TPad::Class())) {
      ((TPad*)obj)->Modified();
      ((TPad*)obj)->Update();
      ((TPad*)obj)->Modified();
    }
  }
  return;
}

void start_plot_active(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx==0) {std::cout << "No gBrowserEx." << std::endl; return;}
  TTimer * timer = gBrowserEx->GetTimer();
  if (timer) {
    std::cout << "Already timer is used." << std::endl; return;
  }
  timer = new TTimer();
  gBrowserEx->SetTimer(timer);
  timer->Connect("Timeout()", 0, 0, "start_plot_active_func()");
  timer->Timeout();
  timer->Start(5000,kFALSE);
  return;
}
