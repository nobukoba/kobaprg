void start_refresh_canvas_func() {
  /*std::cout << "here timer func." << std::endl;*/
  TCanvas *canvas = gPad->GetCanvas();
  canvas->Modified();
  canvas->Update();
  gPad->Update();
  TList *listofpri = canvas->GetListOfPrimitives();
  TObject *obj;
  TIter next(listofpri);
  while ((obj = next())) {
    if (obj->InheritsFrom(TPad::Class())) {
      ((TPad*)obj)->Modified();
      ((TPad*)obj)->Update();
      ((TPad*)obj)->Modified();
    }
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
  timer = new TTimer();
  gBrowserEx->SetTimer(timer);
  timer->Connect("Timeout()", 0, 0, "start_refresh_canvas_func()");
  timer->Timeout();
  timer->Start(1000,kFALSE);
  return;
}
