void slicex_sel_pad(){
  if (!gPad) {
    std::cout << "There is no gPad." << std::endl;
    return;
  }
  TList *listofpri = gPad->GetListOfPrimitives();
  TIter next(listofpri);
  TObject *obj;
  TH2 *hist = 0;
  while ((obj = next())){
    if (obj->InheritsFrom("TH2")) {
      hist = (TH2*)obj;
      std::cout << "TH2 hist was found." << std::endl;
      break;
    }
  }
  if(hist == 0){
    std::cout << "TH2 histogram was not found in this pad. This script is terminated." << std::endl;
    return;
  }

  gROOT->ProcessLine(".L ../cui/slicex.C");
  gROOT->ProcessLine(Form("slicex((TH1*)%p)",hist));

  return;
}
