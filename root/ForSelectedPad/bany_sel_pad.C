void bany_sel_pad(){
  if (!gPad) {
    std::cout << "There is no gPad." << std::endl;
    return 0;
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

  char retstr[256];
  new TGInputDialog(gClient->GetRoot(),0,
                    "Range: %f %f",
                    "0.0 1.0",retstr);
  if(retstr[0] == 0 && retstr[1] == 0){
    std::cout << "Cancel button was pushed. This script is terminated." << std::endl;
    return;
  }
  TString str = retstr;
  str.ReplaceAll(","," ");
  std::istringstream iss(str.Data());
  Double_t par0, par1;
  iss >> par0 >> par1;

  gROOT->ProcessLine(".L ../cui/bany.C");
  gROOT->ProcessLine(Form("bany((TH1*)%p,%f,%f)",hist,par0,par1));
  return;
}
