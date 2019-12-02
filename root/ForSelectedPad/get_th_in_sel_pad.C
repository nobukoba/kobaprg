TH1* get_th_in_sel_pad(){
  if (!gPad) {
    std::cout << "There is no gPad." << std::endl;
    return 0;
  }
  TList *listofpri = gPad->GetListOfPrimitives();
  TIter next(listofpri);
  TObject *obj;
  TH1 *hist = 0;
  while ((obj = next())){
    if (obj->InheritsFrom("TH1")) {
      hist = (TH1*)obj;
      break;
    }
  }
  if(hist == 0){
    std::cout << "The histogram was not found in this pad." << std::endl;
  }
  return hist;
}
