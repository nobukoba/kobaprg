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

void get_zoomed_range(){
  std::cout << "Macro: get_zoomed_range.C" << std::endl;
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");
  if (!hist_fListTree) {
    std::cout << std::endl << "hist_fListTree is null. Exit." << std::endl;
    return;
  }
  
  TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetFirstItem();
  while(cur_ListTreeItem){
    if(cur_ListTreeItem->IsActive()){
      TObject *userdata = (TObject*)cur_ListTreeItem->GetUserData();
      if (userdata->InheritsFrom("TKey")){
	userdata = ((TKey*)userdata)->ReadObj();
	cur_ListTreeItem->SetUserData(userdata);
      }
      if (userdata->InheritsFrom("TH2")){
	std::cout << "TH2 histogram can not be handled." << std::endl;
	cur_ListTreeItem = NextItem(cur_ListTreeItem);
	continue;
      }
      if (userdata->InheritsFrom("TH1")){
	TH1* hist = (TH1*) userdata;
	Double_t lw = hist->GetXaxis()->GetBinLowEdge(hist->GetXaxis()->GetFirst());
	Double_t up = hist->GetXaxis()->GetBinUpEdge(hist->GetXaxis()->GetLast());
	std::cout << "lw: " << lw << std::endl;
	std::cout << "up: " << up << std::endl;
      }
    }
    cur_ListTreeItem = NextItem(cur_ListTreeItem);
  }
  return;
}
