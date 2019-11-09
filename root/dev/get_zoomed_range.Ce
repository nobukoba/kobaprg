void get_zoomed_range(){
  std::cout << "Macro: get_zoomed_range.C" << std::endl;
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("gBrowserEx->GetHistListTree();");
  
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
	cur_ListTreeItem = gBrowserEx->NextItem(cur_ListTreeItem);
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
    cur_ListTreeItem = gBrowserEx->NextItem(cur_ListTreeItem);
  }
  return;
}
