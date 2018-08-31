void photo_peak_fit_clear(){
  std::cout << std::endl << "Macro: kobamac/root/photo_peak_fit_clear.C" << std::endl;
  TCanvas* canvas = gPad->GetCanvas();
  if (canvas == 0) {
    std::cout << "There is no canvas. The script is terminated." << std::endl;
    return;
  }
  TVirtualPad *sel_pad = canvas->GetPad(gPad->GetNumber());
  if (sel_pad == 0) {
    std::cout << "There is no sel_pad. The script is terminated." << std::endl;
    return;
  }
  TList *listofpri = sel_pad->GetListOfPrimitives();
  if (listofpri == 0) {
    std::cout << "The pad includes nothing. The script is terminated." << std::endl;
    return;
  }
  TIter next(listofpri);
  TObject *obj;
  TH1 *hist = 0;
  while (obj = next()){
    if (obj->InheritsFrom("TH2")) {
      std::cout << "This script can not handle TH2 histograms." << std::endl;
      return;
    }
    if (obj->InheritsFrom("TH1")) {
      hist = (TH1*)obj;
      break;
    }
  }
  if(hist == 0){
    std::cout << "TH1 histogram was not found in this pad. The script is terminated." << std::endl;
    return;
  }
  TList *funclist = hist->GetListOfFunctions();
  if(funclist == 0){
    std::cout << "The GetListOfFunctions() is null. The script is terminated." << std::endl;
    return;
  }

  TIter nextfunc(funclist);
  TF1 *funcobj = 0;
  while (funcobj = (TF1*)nextfunc()){
    TString funcname = funcobj->GetName();
    std::cout << "funcname: "<<funcname<< std::endl;
    if(funcname.BeginsWith("photo_peak_fit")){
      std::cout << "funcname in the if: "<<funcname<< std::endl;
      funcobj->Delete();
    }
  }
  gPad->Modified();
  gPad->Update();
  return;
}
