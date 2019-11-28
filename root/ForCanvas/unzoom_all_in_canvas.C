void unzoom_all_in_canvas(){
  std::cout << std::endl << "Macro: unzoom_all_in_canvas.C" << std::endl;
  TCanvas* canvas = gPad->GetCanvas();
  if (canvas == 0) {
    std::cout << "There is no canvas." << std::endl;
    return;
  }
  TList* list  = canvas->GetListOfPrimitives();
  if (list->At(0)==0) {
    std::cout << "There is no ListOfPrimitive for canvas." << std::endl;
    return;
  }
  TVirtualPad *pad = 0;
  Int_t i = 0;
  while ((pad = canvas->GetPad(i))) {
    TList *listofpri = pad->GetListOfPrimitives();
    if (listofpri == 0) {
      std::cout << "The pad includes nothing." << std::endl;
      return;
    }
    TIter next(listofpri);
    TObject *obj;
    TH1 *hist = 0;
    while ((obj = next())){
      if (obj->InheritsFrom("TH1")) {
	hist = (TH1*)obj;
	//std::cout << "hist was found." << std::endl;
	break;
      }
    }
    if(hist == 0){
      std::cout << "Histogram was not found in this pad." << std::endl;
      i++;
      continue;
    }
    hist->GetXaxis()->UnZoom();
    hist->GetYaxis()->UnZoom();
    if (hist->GetZaxis()){
      hist->GetZaxis()->UnZoom();
    }
    pad->Modified();
    pad->Update();
    i++;
  }
  if (i == 0){
    std::cout << "There is no pad." <<std::endl;
  }
  return;
}
