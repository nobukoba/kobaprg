TGListTreeItem *SearchNextItem(TGListTreeItem *cur_item){
  if(cur_item->GetNextSibling()){
    return cur_item->GetNextSibling();
  } else if (cur_item->GetParent()){
    return SearchNextItem(cur_item->GetParent());
  }else{
    return 0;
  }
}
TGListTreeItem *NextItem(TGListTreeItem *cur_item){
  if(cur_item->GetFirstChild()){
    return cur_item->GetFirstChild();
  }
  return SearchNextItem(cur_item);
}
void GetHistActiveItems(TList *items){
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");
  if (!hist_fListTree) {
    std:: cout << "pHistBrowser->GetHistListTree() is null." << std::endl;
    return;
  }
  TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetFirstItem();
  while(cur_ListTreeItem){
    if(cur_ListTreeItem->IsActive()){
      items->Add(new TObjString(Form("%lld", (unsigned long long)cur_ListTreeItem)));
    }
    cur_ListTreeItem = NextItem(cur_ListTreeItem);
  }
  return;
}

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
  
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");
  if (!hist_fListTree) {
    std::cout << "hist_fListTree is null. Maybe pHistBrowser is also null. This script is terminated." << std::endl;
    return;
  }

  Int_t cur_pad = 1;
  if(npad == 0){cur_pad == 0;}

  TList *ordered_items = (TList *) gROOT->ProcessLine("pHistBrowser->GetHistListTreeActiveItems();");
  TList items_ins;
  TList *items = &items_ins;
  if (ordered_items) {
    items = ordered_items;
  }else{
    GetHistActiveItems(items);
  }

  TGListTreeItem *cur_ListTreeItem;
  TIter next(items);
  TObject * obj;
  while(obj = next()){
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
