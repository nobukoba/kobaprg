void print_active_on_root_pdf(Option_t* option) {
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx) {
    gSystem->cd((gBrowserEx->GetInitialWorkingDir()).Data());
  }else{return;}
  std::cout << "gSystem->pwd(): " << gSystem->pwd() << std::endl;
  if (!gPad) {
    std::cout << "There is no gPad. Exit." << std::endl;
    return;
  }
  TCanvas *canvas = gPad->GetCanvas();
  TList *listofpri = canvas->GetListOfPrimitives();
  Int_t npad = 0;
  TObject *obj;
  TIter next(listofpri);
  while ((obj = next())) {
    if (obj->InheritsFrom(TPad::Class())) {
      npad++;
    }
  }
  std::cout << "npad: " << npad <<std::endl;
  
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("gBrowserEx->GetHistListTree();");
  if (!hist_fListTree) {
    std::cout << "hist_fListTree is null. Maybe gBrowserEx is also null. This script is terminated." << std::endl;
    return;
  }
  
  Int_t cur_pad = 1;
  if(npad == 0){cur_pad = 0;}
  TH1 *subtracted = 0;
  canvas->Print("root.pdf[", option);
  TIter next2(gBrowserEx->GetHistListTreeActiveHistos());
  TObjString* objstr;
  while((objstr = (TObjString*)next2())){
    TGListTreeItem *cur_ListTreeItem = (TGListTreeItem *) objstr->GetString().Atoll();
    if(((npad == 0) && (cur_pad == 0))||
       ((npad > 0)  && (cur_pad == 1))) {
      for (Int_t i = cur_pad; i<=npad;i++) {
        TVirtualPad * pad = canvas->cd(i);
        pad->Clear("");
      }
    }
    canvas->cd(cur_pad);
    hist_fListTree->DoubleClicked(cur_ListTreeItem,1);
    cur_pad++;
    if (cur_pad > npad){
      canvas->Print("root.pdf", option);
      if(npad == 0) {
	cur_pad = 0;
      }else{
	cur_pad = 1;
      }
    }
  }
  if(((npad == 0) && (cur_pad != 0))||
     ((npad > 0)  && (cur_pad != 1))) {
    canvas->Print("root.pdf", option);
  }
  canvas->Print("root.pdf]", option);
  return;
}

void print_active_on_root_pdf() {
  print_active_on_root_pdf("");
}
