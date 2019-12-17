void start_refresh_canvas_func() {
  std::cout << "here timer func." << std::endl;
  TCanvas *canvas = gPad->GetCanvas();
  TList *listofpri = canvas->GetListOfPrimitives();
  Int_t npad = 0;
  TObject *obj;
  TIter next(listofpri);
  while ((obj = next())) {
    if (obj->InheritsFrom(TPad::Class())) {
      npad++;
    }
  }
  Int_t cur_pad = 1;
  if(npad == 0){cur_pad = 0;}
  for (Int_t i = cur_pad; i<=npad;i++) {
    TVirtualPad * pad = canvas->cd(i);
    pad->Modified();
    pad->Update();
  }
  return;
}

void start_refresh_canvas(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx==0) {std::cout << "No gBrowserEx." << std::endl; return;}
  TTimer * timer = gBrowserEx->GetTimer();
  if (timer) {
    std::cout << "Already timer is used." << std::endl; return;
  }
  TTimer *timer = new TTimer();
  gBrowserEx->SetTimer(timer);
  timer->Connect("Timeout()", 0, 0, "start_refresh_canvas_func()");
  timer->Timeout();
  timer->Start(1000,kFALSE);
  return;
}
