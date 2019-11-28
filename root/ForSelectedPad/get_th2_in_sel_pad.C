TH2* get_th2_in_sel_pad(){
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
    std::cout << "TH2 histogram was not found in this pad." << std::endl;
  }
  return hist;
}
