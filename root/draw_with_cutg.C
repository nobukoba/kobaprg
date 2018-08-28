void draw_with_cutg(){
  TCanvas* canvas = gPad->GetCanvas();
  if (canvas == 0) {
    std::cout << "There is no canvas." << std::endl;
    return;
  }
  TVirtualPad *sel_pad = canvas->GetPad(gPad->GetNumber());
  if (sel_pad == 0) {
    std::cout << "There is no sel_pad." << std::endl;
    return;
  }
  TList *listofpri = sel_pad->GetListOfPrimitives();
  if (listofpri == 0) {
    std::cout << "The pad includes nothing." << std::endl;
    return;
  }
  TIter next(listofpri);
  TObject *obj;
  TH1 *hist = 0;
  while (obj = next()){
    if (obj->InheritsFrom("TH1")) {
      hist = (TH1*)obj;
      std::cout << "hist was found." << std::endl;
      break;
    }
  }
  if(hist == 0){
    std::cout << "Histogram was not found in this pad." << std::endl;
    return;
  }

  TCutG *cutg = (TCutg*)listofpri->FindObject("CUTG");
  if (cutg != 0){
    cutg->Delete();
  }
  
  cutg  = (TCutG*)sel_pad->WaitPrimitive("CUTG","CutG");
  hist->Draw("[CUTG]");
}
