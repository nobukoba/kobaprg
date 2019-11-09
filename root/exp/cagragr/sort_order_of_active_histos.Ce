void GetHistActiveItemsWithoutDuplication(TList *items){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("gBrowserEx->GetHistListTree();");
  if (!hist_fListTree) {
    std:: cout << std::endl << "gBrowserEx->GetHistListTree() is null." << std::endl;
    return;
  }
  TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetFirstItem();
  while(cur_ListTreeItem){
    if(cur_ListTreeItem->IsActive()){
      TGListTreeItem *tmp_ListTreeItem;
      TIter next(items);
      TObject * obj;
      
      while(obj = next()){
	tmp_ListTreeItem = (TGListTreeItem *) (((TObjString*)obj)->GetString().Atoll());
	TString tmp_str = tmp_ListTreeItem->GetText();
	if (tmp_str.EqualTo(cur_ListTreeItem->GetText())) {
	  cur_ListTreeItem = gBrowserEx->NextItem(cur_ListTreeItem);
	  continue;
	}
      }
      if (items->GetEntries() == 0) {
	items->Add(new TObjString(Form("%lld", (unsigned long long)cur_ListTreeItem)));
      }else{
	TIter next2(items);
	TObject * obj2;
	Int_t added = 0;
	while(obj2 = next2()){
	  tmp_ListTreeItem = (TGListTreeItem *) (((TObjString*)obj2)->GetString().Atoll());
	  std::string cur_name = cur_ListTreeItem->GetText();
	  std::string tmp_name = tmp_ListTreeItem->GetText();
	  if(tmp_name.compare(cur_name) > 0){
	    items->AddBefore(obj2, new TObjString(Form("%lld", (unsigned long long)cur_ListTreeItem)));
	    added = 1;
	    break;
	  }
	}
	if (added == 0) {
	  items->Add(new TObjString(Form("%lld", (unsigned long long)cur_ListTreeItem)));
	}
      }
    }
    cur_ListTreeItem = gBrowserEx->NextItem(cur_ListTreeItem);
  }
  return;
}

void sort_order_of_active_histos(){
  std::cout << std::endl << "Macro: sort_order_of_active_histos.C" << std::endl;
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("gBrowserEx->GetHistListTree();");
  TList items_ins;
  TList *items = &items_ins;
  GetHistActiveItemsWithoutDuplication(items);
  TList outitems;
  TH1 *hist = 0;
  TGListTreeItem *cur_ListTreeItem;
  TIter next(items);
  TObject * obj;
  while(obj = next()){
    cur_ListTreeItem = (TGListTreeItem *) (((TObjString*)obj)->GetString().Atoll());
    TObject *userdata = (TObject*)cur_ListTreeItem->GetUserData();
    if (userdata->InheritsFrom("TKey")){
      userdata = ((TKey*)userdata)->ReadObj();
      cur_ListTreeItem->SetUserData(userdata);
    }
    if (!userdata->InheritsFrom("TH1")){
      continue;
    }
    std::cout << "userdata->GetName(): " << userdata->GetName() << std::endl;
    gROOT->cd();
    userdata->Clone(userdata->GetName());
  }
  return;
}
