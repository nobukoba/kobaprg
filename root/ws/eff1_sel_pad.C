void eff1_sel_pad(){
  TVirtualPad *sel_pad = gROOT->GetSelectedPad();
  TList *listofpri = sel_pad->GetListOfPrimitives();
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
    std::cout << "TH1 histogram was not found in this pad." << std::endl;
    return;
  }
  gROOT->ProcessLine(".L ./eff1.C");
  gROOT->ProcessLine(Form("eff1((TH1*)%p)",hist));
  return;
}
