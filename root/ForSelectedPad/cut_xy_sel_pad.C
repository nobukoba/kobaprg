void cut_xy_sel_pad(){
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

  char retstr[256];
  new TGInputDialog(gClient->GetRoot(),0,
                    "Range x1 x2 y1 y2: %f %f %f %f",
                    "0.0 1.0 0.0 1.0",retstr);
  if(retstr[0] == 0 && retstr[1] == 0){
    std::cout << "Cancel button was pushed. This script is terminated." << std::endl;
    return;
  }
  TString str = retstr;
  str.ReplaceAll(","," ");
  std::istringstream iss(str.Data());
  Double_t x1, x2, y1, y2;
  iss >> x1 >> x2 >> y1 >> y2;

  gROOT->ProcessLine(".L ../cui/cut_xy.C");
  gROOT->ProcessLine(Form("cut_xy((TH1*)%p,%f,%f,%f,%f)",hist, x1, x2, y1, y2));

  return;
}
