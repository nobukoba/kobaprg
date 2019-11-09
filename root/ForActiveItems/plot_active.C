void plot_active() {
  TCanvas *canvas = gPad->GetCanvas();
  if (!canvas) {
    std::cout << "There is no canvas. This script is terminated." << std::endl;
    return;
  }
  TList *listofpri = canvas->GetListOfPrimitives();
  if (!listofpri) {
    std::cout << "There is nothing in the canvas. This script is terminated." << std::endl;
    return;
  }
  Int_t npad = 0;
  TObject *obj;
  TIter next(listofpri);
  while ((obj = next())) {
    if (obj->InheritsFrom(TPad::Class())) {
      npad++;
    }
  }
  
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TGListTree *hist_fListTree = (TGListTree *)gBrowserEx->GetHistListTree();
  if (!hist_fListTree) {
    std::cout << "hist_fListTree is null. Maybe gBrowserEx is also null. This script is terminated." << std::endl;
    return;
  }

  Int_t cur_pad = 1;
  if(npad == 0){cur_pad == 0;}
  TList *ordered_items = (TList *)gBrowserEx->GetHistListTreeActiveItems();
  TGListTreeItem *cur_ListTreeItem;
  TIter next(ordered_items);
  TObject * obj;
  while((obj = next())){
    cur_ListTreeItem = (TGListTreeItem *) (((TObjString*)obj)->GetString().Atoll());
    if(((npad == 0) && (cur_pad == 0))||
       ((npad > 0)  && (cur_pad == 1))) {
      canvas->Clear("D");
    }
    canvas->cd(cur_pad);
    TObject *userdata = (TObject*)cur_ListTreeItem->GetUserData();
    if (userdata->InheritsFrom("TKey")){
      userdata = ((TKey*)userdata)->ReadObj();
      cur_ListTreeItem->SetUserData(userdata);
    }
    if (!userdata->InheritsFrom("TH1")){
      continue;
    }
    hist_fListTree->DoubleClicked(cur_ListTreeItem,1);
    cur_pad++;
    if (cur_pad > npad){
      canvas->Modified();
      canvas->Update();
      if(npad == 0) {
	cur_pad = 0;
      }else{
	cur_pad = 1;
      }
    }
  }
  canvas->Update();
  canvas->Modified();
  return;
}
