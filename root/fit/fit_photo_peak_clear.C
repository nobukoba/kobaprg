void fit_photo_peak_clear(){
  std::cout << std::endl << "Macro: fit_photo_peak_clear.C" << std::endl;
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
  while ((obj = next())){
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
  
  while ((obj = next())){
    TString objname = obj->GetName();
    if (objname.EqualTo("TLine")) {
      listofpri->Remove(obj);
    } 
  }
  
  TList *funclist = hist->GetListOfFunctions();
  if(funclist == 0){
    std::cout << "The GetListOfFunctions() is null. The script is terminated." << std::endl;
    return;
  }

  Int_t j = 0;
  while(funclist->FindObject(Form("fit_photo_peak_%d",j))){
    j++;
  }

  TIter nextfunc(funclist);
  TF1 *funcobj = 0;
  while ((funcobj = (TF1*)nextfunc())){
    TString funcname = funcobj->GetName();
    if(funcname.BeginsWith("fit_photo_peak")
       &&funcname.EndsWith(Form("_%d",j-1))){
      funclist->Remove(funcobj);
    }
  }
  gPad->Modified();
  gPad->Update();
  gPad->Update();
  return;
}
