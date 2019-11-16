#if defined(__CINT__) && !defined(__MAKECINT__)
{
  gSystem->CompileMacro("./kobaprg/root/tbrowserex.C","k");
  tbrowserex();
}
#else

#include <iostream>
#include <string.h>

#include "TROOT.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TBox.h"
#include "TFrame.h"
#include "TEnv.h"
#include "TBrowser.h"
#include "TRootBrowser.h"
#include "TApplication.h"
#include "GuiTypes.h"
#include "TGClient.h"
#include "TGObject.h"
#include "TGFileBrowser.h"
#include "TGListTree.h"
#include "TGTab.h"
#include "TCanvas.h"
#include "TGComboBox.h"
#include "TGTextEntry.h"
#include "TGPicture.h"
#include "TGDimension.h"
#include "TFile.h"
#include "TList.h"
#include "TClass.h"
#include "TTree.h"
#include "TClassMenuItem.h"
#include "TKey.h"
#include "TFolder.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "KeySymbols.h"

class TGFileBrowserEx : public TGFileBrowser {
public:
  TGFileBrowserEx(const TGWindow *p, TBrowser* b, UInt_t w, UInt_t h) : 
    TGFileBrowser(p, b, w, h){}
  TGListTree* GetListTree(){return fListTree;}
  TGListTreeItem* GetListLevel(){return fListLevel;}
  void ResetListLevel(){fListLevel = 0;}
  TGComboBox* GetDrawOptionPointer(){return fDrawOption;}
  TGPictureButton* GetRefreshButtonPointer(){return fRefreshButton;}
  ClassDef(TGFileBrowserEx,0)
};

class TGListTreeItemEx : public TGListTreeItem {
public:
  TGListTreeItemEx(TGClient *client) :
    TGListTreeItem(client){}
  Int_t GetY(){return this->fY;}
  ClassDef(TGListTreeItemEx,0)
};

class TBrowserEx : public TBrowser {
public:
  TBrowserEx() :
    TBrowser("kobabrowser","ROOT Object Browser Extended",800,1000,0,"CI"),
    /* if option="FCI" is used, segv will occur for
       typing ".q" command for this TBrowserEx and also TBrowser.
       Confirmed with root v5 & v6. */
    file_browser(0),
    macro_browser(0),
    hist_browser(0),
    macro_fListTree(0),
    hist_fListTree(0)
  {
    /* gROOT->GetListOfBrowsers()->Remove(this);
       delete this->GetContextMenu();
       this->GetBrowserImp()->GetMainFrame()->Connect("CloseWindow()", "TBrowserEx", this, "CloseWindow()");
       gROOT->GetListOfCleanups()->Remove(this);
       gROOT->GetListOfCleanups()->Remove(macro_browser);
       gROOT->GetListOfCleanups()->Remove(hist_browser);
       StartEmbedding(TRootBrowser::kLeft,-1);
       macro_browser = new TGFileBrowserEx(gClient->GetRoot(), this, 200, 500);
       StopEmbedding("Macros");
       StartEmbedding(TRootBrowser::kLeft,-1);
       hist_browser = new TGFileBrowserEx(gClient->GetRoot(), this, 200, 500);
       StopEmbedding("Histos"); */
    gROOT->ProcessLine(Form("TBrowserEx *gBrowserEx = (TBrowserEx *)0x%lx;",(ULong_t)this));
    initial_working_dir = gSystem->pwd();
    
    TString cmd;
    cmd.Form("new TGFileBrowserEx(gClient->GetRoot(), (TBrowser *)0x%lx, 200, 500);", (ULong_t)this);
    macro_browser = (TGFileBrowserEx*) this->ExecPlugin("Macros", 0, cmd.Data(), 0);
  
    cmd.Form("new TGFileBrowserEx(gClient->GetRoot(), (TBrowser *)0x%lx, 200, 500);", (ULong_t)this);
    hist_browser = (TGFileBrowserEx*)  this->ExecPlugin("Histos", 0, cmd.Data(), 0);
    
    TFolder *fld = new TFolder();
    fld->SetName("fld");
    fld->AddFolder("ROOT_Memory","List of Objects in the gROOT Directory",gDirectory->GetList());
    fld->AddFolder("ROOT_Files","List of Connected ROOT Files", gROOT->GetListOfFiles());
    fld->Browse(this);
    
    macro_fListTree = macro_browser->GetListTree();
    macro_browser->AddFSDirectory("kobaprg/root","kobaprg/root","Add");
    macro_browser->ApplyFilter(2);
    SetDNDSourceRecursive(macro_fListTree,macro_fListTree->GetFirstItem(),0);
    TGListTreeItem *ltitem;
    ltitem = macro_fListTree->GetFirstItem();
    macro_fListTree->DoubleClicked(ltitem,1); ltitem->SetOpen(1);
    ltitem = macro_fListTree->FindChildByName(ltitem,"fit");
    macro_fListTree->DoubleClicked(ltitem,1); ltitem->SetOpen(1);
    macro_fListTree->ClearViewPort();
    macro_fListTree->Connect("Clicked(TGListTreeItem *, Int_t, UInt_t, Int_t, Int_t)",
			     "TBrowserEx", this,
			     "MyDoubleClicked(TGListTreeItem *, Int_t, UInt_t, Int_t, Int_t)");
    macro_fListTree->Connect("Clicked(TGListTreeItem *, Int_t, UInt_t, Int_t, Int_t)",
			     "TBrowserEx", this,
			     "MyClicked2(TGListTreeItem *, Int_t, UInt_t, Int_t, Int_t)");
    
    /* hist_browser->Add((TFolder *)(((TFolder *)gROOT->GetListOfBrowsables()->FindObject("root"))->FindObject("ROOT Memory"))); */
    hist_browser->GetRefreshButtonPointer()->Connect("Clicked()", "TBrowserEx", this, "TBrowserExRefresh()");
    hist_fListTree = hist_browser->GetListTree();
    /* hist_fListTree->Disconnect("Clicked(TGListTreeItem *, Int_t)"); */
    hist_fListTree->Connect("Clicked(TGListTreeItem *, Int_t)",
			    "TBrowserEx", this,
			    "MyClicked(TGListTreeItem *, Int_t)");
    hist_fListTree->Connect("Clicked(TGListTreeItem *, Int_t, UInt_t, Int_t, Int_t)",
			    "TBrowserEx", this,
			    "MyClicked2(TGListTreeItem *, Int_t, UInt_t, Int_t, Int_t)");
    hist_fListTree->Connect("Clicked(TGListTreeItem *, Int_t, UInt_t, Int_t, Int_t)",
			    "TBrowserEx", this,
			    "MyClicked3(TGListTreeItem *, Int_t, UInt_t, Int_t, Int_t)");
    hist_fListTree->Connect("Clicked(TGListTreeItem *, Int_t, UInt_t, Int_t, Int_t)",
			    "TBrowserEx", this,
			    "MyDoubleClicked(TGListTreeItem *, Int_t, UInt_t, Int_t, Int_t)");
    hist_fListTree->Connect("Clicked(TGListTreeItem *, Int_t, UInt_t, Int_t, Int_t)",
			    "TBrowserEx", this,
			    "MyClickedForHistFileBrowser(TGListTreeItem *, Int_t, UInt_t, Int_t, Int_t)");
   hist_fListTree->Connect("Clicked(TGListTreeItem *, Int_t, UInt_t, Int_t, Int_t)",
			    "TBrowserEx", this,
			    "SetCannotMove(TGListTreeItem *, Int_t, UInt_t, Int_t, Int_t)");
   hist_fListTree->Connect("Clicked(TGListTreeItem *, Int_t, UInt_t, Int_t, Int_t)",
			    "TBrowserEx", this,
			    "RemoveAndSetText(TGListTreeItem *, Int_t, UInt_t, Int_t, Int_t)");


   TQObject::Connect("TCanvas","ProcessedEvent(Int_t,Int_t,Int_t,TObject*)",
		      "TBrowserEx", this, "change_canvas(Int_t,Int_t,Int_t,TObject*)");
    //this->GetBrowserImp()->GetMainFrame()->Connect("ProcessedEvent(Event_t*)","TBrowserEx", this, "HandleKey(Event_t*)");

    TQObject::Connect("TGFrame","ProcessedEvent(Event_t*)","TBrowserEx", this, "HandleKeyEx(Event_t*)");

    
    Int_t nentry = hist_browser->GetDrawOptionPointer()->GetNumberOfEntries() + 1;
    hist_browser->GetDrawOptionPointer()->AddEntry("",    nentry++); /* line 15 */
    hist_browser->GetDrawOptionPointer()->AddEntry("*",   nentry++); /* line 16 */
    hist_browser->GetDrawOptionPointer()->AddEntry("p",   nentry++);
    hist_browser->GetDrawOptionPointer()->AddEntry("l",   nentry++);
    hist_browser->GetDrawOptionPointer()->AddEntry("c",   nentry++);
    hist_browser->GetDrawOptionPointer()->AddEntry("l*",  nentry++);
    hist_browser->GetDrawOptionPointer()->AddEntry("c*",  nentry++);
    hist_browser->GetDrawOptionPointer()->AddEntry("lp",  nentry++);
    hist_browser->GetDrawOptionPointer()->AddEntry("cp",  nentry++);
    hist_browser->GetDrawOptionPointer()->AddEntry("a",   nentry++);
    hist_browser->GetDrawOptionPointer()->AddEntry("a*",  nentry++);
    hist_browser->GetDrawOptionPointer()->AddEntry("ap",  nentry++);
    hist_browser->GetDrawOptionPointer()->AddEntry("al",  nentry++);
    hist_browser->GetDrawOptionPointer()->AddEntry("ac",  nentry++);
    hist_browser->GetDrawOptionPointer()->AddEntry("al*", nentry++);
    hist_browser->GetDrawOptionPointer()->AddEntry("ac*", nentry++);
    hist_browser->GetDrawOptionPointer()->AddEntry("alp", nentry++); /* line 31 */
    hist_browser->GetDrawOptionPointer()->AddEntry("acp", nentry++); /* line 32 */
    hist_browser->GetDrawOptionPointer()->AddEntry("",    nentry++); /* line 33 */
    hist_browser->GetDrawOptionPointer()->AddEntry("same",nentry++); /* line 34 */
    hist_browser->GetDrawOptionPointer()->AddEntry("l",   nentry++);
    hist_browser->GetDrawOptionPointer()->AddEntry("c",   nentry++);
    hist_browser->GetDrawOptionPointer()->AddEntry("fc",  nentry++); /* line 37 */
    hist_browser->GetDrawOptionPointer()->Select(3,1);
    hist_browser->GetDrawOptionPointer()->
      GetTextEntry()->SetText(hist_browser->GetDrawOptionPointer()->GetSelectedEntry()->GetTitle());
    
    customTListMenu();
    customTDirectoryFileMenu();
    customTFolderMenu();
    SetDNDSourceRecursive(hist_fListTree,hist_fListTree->GetFirstItem(),0);
    ltitem = hist_fListTree->GetFirstItem();
    hist_fListTree->DoubleClicked(ltitem,1); ltitem->SetOpen(1);
    ltitem = hist_fListTree->FindChildByName(0,"ROOT_Files");
    hist_fListTree->DoubleClicked(ltitem,1); ltitem->SetOpen(1);
    gROOT->GetListOfCleanups()->Add(&list_of_active_histos);
    gROOT->GetListOfCleanups()->Add(&list_of_ordered_active_histos);
    /* see https://sft.its.cern.ch/jira/browse/ROOT-9262
       for gROOT->GetListOfCleanups()*/
  }
  
  ~TBrowserEx(){
    gROOT->GetListOfBrowsers()->Remove(this);
    /* printf("~TBrowserEx");
       gROOT->GetListOfBrowsers()->Remove(this);
       delete hist_browser;
       delete macro_browser; */
  }
  /* void CloseWindow(){
     gApplication->Terminate();
     this->GetBrowserImp()->GetMainFrame()->CloseWindow();
     
     this->Delete();
     delete this;
     gClient->Delete();
     } */

    void HandleKeyEx(Event_t* event){
    if (event->fType != kGKeyPress) {
      return;
    }
    char   input[10];
    UInt_t keysym;
    gVirtualX->LookupString(event, input, sizeof(input), keysym);
    /*printf("(EKeySym)keysym %d\n", (EKeySym)keysym);*/
    TString gTQSender_name = ((TObject*)gTQSender)->GetName();
    if (gTQSender_name.BeginsWith("fCompositeFrame")){
      return;
    }
    TGListTree *cur_ListTree = 0; 
    TGFileBrowserEx *cur_FileBrowser = 0; 
    TGTab * ptab = ((TRootBrowser*)this->GetBrowserImp())->GetTabLeft();
    Int_t ntab = ptab->GetNumberOfTabs();
    if(strcmp(ptab->GetCurrentTab()->GetText()->Data(),"Macros")==0){
      cur_ListTree    = macro_fListTree;
      cur_FileBrowser = macro_browser;
    }else if(strcmp(ptab->GetCurrentTab()->GetText()->Data(),"Histos")==0){
      cur_ListTree    = hist_fListTree;
      cur_FileBrowser = hist_browser;
    }else{
      return;
    }
    if (!cur_ListTree->GetCurrent()) {
      Event_t event_tmp;
      TGListTreeItem * item_tmp = 0;
      if(strcmp(ptab->GetCurrentTab()->GetText()->Data(),"Macros")==0){
        item_tmp = GetActiveMacro();
      }else{
        if (hist_fListTree_active_items.Last()) {
          item_tmp = (TGListTreeItem *) (((TObjString*)hist_fListTree_active_items.Last())->GetString().Atoll());
        }
      }
      if(item_tmp){
        event_tmp.fY = ((TGListTreeItemEx*)item_tmp)->GetY();
      }else{
        TGPosition pos = cur_ListTree->GetPagePosition();
        event_tmp.fY = pos.fY + 2;
      }
      cur_ListTree->HandleMotion(&event_tmp);
    }
    TGListTreeItem * cur_item = cur_ListTree->GetCurrent();
    Bool_t inside_of_filebrowser = false;
    if (gTQSender_name.BeginsWith("fListTree")){
      inside_of_filebrowser = true;
    }else{
      cur_ListTree->HandleKey(event);
    }
    static UInt_t prev_keysym = 0;
    static Int_t  prev_tab    = 1;
    if (event->fState & kKeyControlMask) {
      if (keysym == kKey_Tab) {
	Int_t new_tab = ptab->GetCurrent();
	if((prev_keysym == kKey_Tab)||
	   (new_tab==prev_tab)){
	  new_tab++;
	  if (new_tab > (ntab-1)) {new_tab = 0;}
	  ptab->SetTab(new_tab,0);
	}else{
	  ptab->SetTab(prev_tab,0);
	  prev_tab = new_tab;
	}
      }
    }
    /*if (keysym == kKey_g) {
      gROOT->ProcessLine(".x fit_p1g.C");
      }
      if (keysym == kKey_f) {
      gROOT->ProcessLine(".x fit_photo_peak.C");
      }
      if (keysym == kKey_c) {
      gROOT->ProcessLine(".x fit_photo_peak_clear.C");
      }
    */    
    if ((event->fState & kKeyControlMask) &&
	(keysym == kKey_p)) {
      PrintCanvas();
    }
    if (keysym == kKey_F5) {
      cur_FileBrowser->GetRefreshButtonPointer()->Clicked();
    }
    if (keysym == kKey_Space) {
      TGListTreeItem *cur_ListTreeItem = cur_ListTree->GetFirstItem();
      Bool_t status = false;
      Bool_t first_found = false;
      while(cur_ListTreeItem){
	if(cur_ListTreeItem->IsActive()){
	  if(!first_found){
	    first_found = true;
	    if (inside_of_filebrowser&&
		(cur_ListTree->GetCurrent() == cur_ListTreeItem)) {
	      status = cur_ListTreeItem->IsChecked();
	    }else{
	      status = !cur_ListTreeItem->IsChecked();
	    }
	  }
	  cur_ListTree->CheckItem(cur_ListTreeItem, status);
	}
	cur_ListTreeItem = NextItem(cur_ListTreeItem);
      }
      if((!first_found)&&
	 (!inside_of_filebrowser)){
	TGListTreeItem *cur_ListTreeItem = cur_ListTree->GetCurrent();
	cur_ListTree->CheckItem(cur_ListTreeItem, !cur_ListTreeItem->IsChecked());
      }
      if((first_found)&&
	 inside_of_filebrowser&&
	 (!cur_ListTree->GetCurrent()->IsActive())){
	TGListTreeItem *cur_ListTreeItem = cur_ListTree->GetCurrent();
	cur_ListTree->CheckItem(cur_ListTreeItem, !cur_ListTreeItem->IsChecked());
      }
      cur_ListTree->ClearViewPort();
    }
    if(strcmp(ptab->GetCurrentTab()->GetText()->Data(),"Histos")==0){
      if(event->fState & kKeyShiftMask) {
	TGListTreeItem * cur_item = cur_ListTree->GetCurrent();
	if(prev_keysym == kKey_Shift) {
	  if (keysym == kKey_Up) {
	    TObjString objstr_tmp(Form("%lld",(unsigned long long)NextItem(cur_item)));
	    if (!hist_fListTree_active_items.FindObject(&objstr_tmp)) {
	      cur_ListTree->HighlightItem(NextItem(cur_item),kTRUE,kTRUE);
	      
	      hist_fListTree_active_items.Add(new TObjString(Form("%lld",(unsigned long long)NextItem(cur_item))));
	    }
	  }
	  if (keysym == kKey_Down) {
	    TObjString objstr_tmp(Form("%lld",(unsigned long long)PrevItem(cur_item)));
	    if (!hist_fListTree_active_items.FindObject(&objstr_tmp)) {
	      cur_ListTree->HighlightItem(PrevItem(cur_item),kTRUE,kTRUE);
	      hist_fListTree_active_items.Add(new TObjString(Form("%lld",(unsigned long long)PrevItem(cur_item))));
	    }
	  }
	}
	if ((keysym == kKey_Up)||(keysym == kKey_Down)) {
	  cur_ListTree->HighlightItem(cur_item,kTRUE,kTRUE);
	  TObjString objstr_tmp(Form("%lld",(unsigned long long)cur_item));
	  if (!hist_fListTree_active_items.FindObject(&objstr_tmp)) {
	    hist_fListTree_active_items.Add(new TObjString(Form("%lld",(unsigned long long)cur_item)));
	  }
	}
      }
      static TGListTreeItem *prev_item_w_cs = 0;
      if ((keysym == kKey_Return)||
	  (keysym == kKey_Enter)) {
	  if(strcmp(cur_item->GetPicture()->GetName(),"folder_t.xpm")==0 ||
	     strcmp(cur_item->GetPicture()->GetName(),"ofolder_t.xpm")==0 ||
	     strcmp(cur_item->GetPicture()->GetName(),"rootdb_t.xpm__16x16")==0 ||
	     strcmp(cur_item->GetPicture()->GetName(),"tree_t.xpm__16x16")==0 ||
	     strcmp(cur_item->GetPicture()->GetName(),"ntuple_t.xpm__16x16")==0 ){
	  }else{
	    if (prev_item_w_cs == cur_item){
	      if ((event->fState & kKeyControlMask) ||
		  (event->fState & kKeyShiftMask)){
		/*cur_ListTree->Clicked(cur_item, 0, event->fState, 0, 0);*/
	      }
	    }
	  }
      }
      prev_item_w_cs = cur_item;
      cur_ListTree->ClearViewPort();
    }
    prev_keysym = keysym;
  }

  void MyDoubleClicked(TGListTreeItem *item, Int_t, UInt_t mask, Int_t, Int_t){
    TGListTree *lt = (TGListTree*)gTQSender;
    if((!(mask & kKeyShiftMask))&&
      (!(mask & kKeyControlMask))){
      /* lt->DoubleClicked(item,1); */
      lt->DoubleClicked(item,0);
    }
  }
  
  void MyClicked2(TGListTreeItem *item, Int_t, UInt_t mask, Int_t, Int_t){
    /* printf("item pic name %s", item->GetPicture()->GetName()); */
    if((!(mask & kKeyShiftMask))&&
       (!(mask & kKeyControlMask))){
      if(strcmp(item->GetPicture()->GetName(),"folder_t.xpm") ==0 ||
	 strcmp(item->GetPicture()->GetName(),"ofolder_t.xpm")==0 ||
         strcmp(item->GetPicture()->GetName(),"rootdb_t.xpm__16x16")==0 ||
         strcmp(item->GetPicture()->GetName(),"tree_t.xpm__16x16")==0 ||
         strcmp(item->GetPicture()->GetName(),"ntuple_t.xpm__16x16")==0 ){
        item->SetOpen(!item->IsOpen());
      }
    }
  }
  
  void MyClicked3(TGListTreeItem *item, Int_t, UInt_t mask, Int_t, Int_t){
    if((!(mask & kKeyShiftMask))&&
       (!(mask & kKeyControlMask))){
      TObject *userdata = (TObject*)item->GetUserData();
      Int_t isel = hist_browser->GetDrawOptionPointer()->GetSelected();
      if (userdata->InheritsFrom("TGraphErrors")){
	if (isel <= 14 || isel >=34) {
	  hist_browser->GetDrawOptionPointer()->Select(31,1);
	  hist_browser->GetDrawOptionPointer()->
	    GetTextEntry()->SetText(hist_browser->GetDrawOptionPointer()->GetSelectedEntry()->GetTitle());
	}
	TString opt = hist_browser->GetDrawOptionPointer()->GetTextEntry()->GetText();
	userdata->Draw(opt.Data());

      } else if (userdata->InheritsFrom("TF1")){
	if (isel <= 32) {
	  hist_browser->GetDrawOptionPointer()->Select(35,1);
	  hist_browser->GetDrawOptionPointer()->
	    GetTextEntry()->SetText(hist_browser->GetDrawOptionPointer()->GetSelectedEntry()->GetTitle());
	}
      } else if (userdata->InheritsFrom("TH1")){
	if (isel >= 16) {
	  hist_browser->GetDrawOptionPointer()->Select(3,1);
	  hist_browser->GetDrawOptionPointer()->
	    GetTextEntry()->SetText(hist_browser->GetDrawOptionPointer()->GetSelectedEntry()->GetTitle());
	}
      }
    }
  }
  
  void MyClicked(TGListTreeItem *item, Int_t /*btn*/){
    TGListTree *lt = (TGListTree*)gTQSender;
    TObject *userdata = (TObject*)item->GetUserData();
    /* if (userdata->InheritsFrom("TKey")){
      item->SetUserData(((TKey*)userdata)->ReadObj());
      } */
    if(strcmp(item->GetPicture()->GetName(),"folder_t.xpm")==0 ||
       strcmp(item->GetPicture()->GetName(),"ofolder_t.xpm")==0 ||
       strcmp(item->GetPicture()->GetName(),"rootdb_t.xpm__16x16")==0 ||
       strcmp(item->GetPicture()->GetName(),"tree_t.xpm__16x16")==0 ||
       strcmp(item->GetPicture()->GetName(),"ntuple_t.xpm__16x16")==0 )
      {return;}
    TString opt = lt->GetParent()->GetParent()->GetParent()->GetDrawOption();
    Int_t isel = hist_browser->GetDrawOptionPointer()->GetSelected();
    if ((opt == "same")||
	((isel<=16)&&(isel>=23))){
      return;
    }
    TCanvas* canvas = gPad->GetCanvas();
    if(canvas->GetClickSelectedPad()){
      canvas->SetClickSelectedPad(0);
      return;
    }
    if (canvas->GetPad(1)==0) {return;}
    if (strcmp(canvas->GetPad(1)->GetName(),"Canvas_1_1")==0){
      Int_t  cur_pad  = gPad->GetNumber();
      TList* selpad_list = canvas->GetPad(cur_pad)->GetListOfPrimitives();
      if(selpad_list->At(0)){
	Int_t next_pad = cur_pad + 1;
	if (canvas->GetPad(next_pad) == 0) {
	  next_pad = 1;
	}
	canvas->cd(next_pad);
      }
    }
  }
  
  void MyClickedForHistFileBrowser(TGListTreeItem *entry, Int_t btn, UInt_t mask, Int_t x, Int_t y){
    if((!(mask & kKeyShiftMask))&&
       (!(mask & kKeyControlMask))){
      hist_fListTree_active_items.Delete();
      hist_fListTree_active_items.Add(new TObjString(Form("%lld", (unsigned long long)entry)));
    }
    if(mask & kKeyShiftMask){
      TGListTreeItem  *last_item;
      if (hist_fListTree_active_items.Last()) {
        last_item = (TGListTreeItem *) (((TObjString*)hist_fListTree_active_items.Last())->GetString().Atoll());
      }else{
	last_item = entry;
      }
      if (last_item) {
	TGListTreeItem  *cur_item = last_item;
	Bool_t go_up   = false;
	Bool_t go_down = false;
	while (cur_item){
	  if(cur_item == entry){
	    go_up = true;
	    break;
	  }
	  cur_item = PrevItem(cur_item);
	}
	cur_item = last_item;
	while (cur_item){
	  if(cur_item == entry){
	    go_down = true;
	    break;
	  }
	  cur_item = NextItem(cur_item);
	}
	if(go_up || go_down){
	  cur_item = last_item;
	  while (cur_item){
	    hist_fListTree->HighlightItem(cur_item,kTRUE,kTRUE);
	    TObjString objstr_tmp(Form("%lld",(unsigned long long)cur_item));
	    if (!hist_fListTree_active_items.FindObject(&objstr_tmp)) {
	      hist_fListTree_active_items.Add(new TObjString(Form("%lld",(unsigned long long)cur_item)));
	    }
	    if(cur_item == entry){
	      break;
	    }
	    if(go_up){
	      cur_item = PrevItem(cur_item);
	    }else{
	      cur_item = NextItem(cur_item);
	    }
	  }
	}
      }
      TGListTreeItem *cur_ListTreeItem = 0;
      TIter next(&hist_fListTree_active_items);
      TObject * obj;
      while((obj = next())){
      	cur_ListTreeItem = (TGListTreeItem *) (((TObjString*)obj)->GetString().Atoll());
      	hist_fListTree->HighlightItem(cur_ListTreeItem,kTRUE,kTRUE);
      }
      hist_fListTree->SetSelected(entry);
    }    
    TGListTreeItem *cur_ListTreeItem = 0;
    if(mask & kKeyControlMask){
      TObjString objstr_tmp(Form("%lld",(unsigned long long)entry));
      TObjString *objstr_ptr;
      if ((objstr_ptr = (TObjString *)hist_fListTree_active_items.FindObject(&objstr_tmp))) {
	hist_fListTree_active_items.Remove(&objstr_tmp);
	entry->SetActive(kFALSE);
	delete objstr_ptr;
      }else{
	hist_fListTree_active_items.Add(new TObjString(Form("%lld", (unsigned long long)entry)));
      	hist_fListTree->HighlightItem(cur_ListTreeItem,kTRUE,kTRUE);
      }
      TIter next(&hist_fListTree_active_items);
      TObject * obj;
      while((obj = next())){
      	cur_ListTreeItem = (TGListTreeItem *) (((TObjString*)obj)->GetString().Atoll());
      	hist_fListTree->HighlightItem(cur_ListTreeItem,kTRUE,kTRUE);
      }
      hist_fListTree->SetSelected(entry);
    }
    hist_fListTree->ClearViewPort();
    return;
  }
  
  void TBrowserExRefresh() {
    RefreshDir(hist_fListTree, hist_fListTree->GetFirstItem());
  }
  
  void RefreshDir(TGListTree* lt, TGListTreeItem* item){
    TGListTreeItem* cur_item = item;
    while (cur_item){
      TClass *cl = TClass::GetClass(((TObject*)cur_item->GetUserData())->ClassName());
      if (cl->InheritsFrom("TKey")) {
        cl = TClass::GetClass(((TKey*)(cur_item->GetUserData()))->GetClassName());
      }
      if (cl->InheritsFrom("TFolder")||cl->InheritsFrom("TDirectory")||cl->InheritsFrom("TTree")) {
        if (cur_item->IsOpen()) {
          lt->DoubleClicked(cur_item,1);
	  RemoveAndSetText(cur_item,0,0,0,0);
	  lt->DoubleClicked(cur_item,1);
        }
      }
      RefreshDir(lt, cur_item->GetFirstChild());
      cur_item = cur_item->GetNextSibling();
    }
  }
  
  void SetCannotMove(TGListTreeItem *item, Int_t, UInt_t, Int_t, Int_t){
    if (!gPad) {std::cout << "There is no gPad." << std::endl; return;}
    /* TCanvas* canvas = gPad->GetCanvas(); */
    gPad->GetFrame()->SetBit(TBox::kCannotMove);
    return;
  }
  
  void change_canvas(Int_t event, Int_t x, Int_t y, TObject* selected) {
    TCanvas *c = (TCanvas*)gTQSender;
    /*    printf("Canvas %s: event=%d, x=%d, y=%d, selected=%s\n", c->GetName(),
          event, x, y, selected->IsA()->GetName()); */
    if (event == kButton1Down) {
      /* std::cout << "selected->ClassName()" << selected->ClassName() << std::endl; */
      if (selected->InheritsFrom("TPad")||
	  selected->InheritsFrom("TFrame")||
	  selected->InheritsFrom("TH2")||
	  selected->InheritsFrom("TPaveStats")||
	  selected->InheritsFrom("TPaveText")||
	  selected->InheritsFrom("TPaletteAxis")||
	  selected->InheritsFrom("TAxis")){
	TVirtualPad *cur_pad  = c->GetSelectedPad();
	cur_pad->cd();
	gPad->Update();
      }
    }
  }

  TGListTreeItem *NextItem(TGListTreeItem *cur_item){
    if(cur_item->IsOpen()){
      if(cur_item->GetFirstChild()){
	return cur_item->GetFirstChild();
      }
    }
    return SearchNextItem(cur_item);
  }
  TGListTreeItem *SearchNextItem(TGListTreeItem *cur_item){
    if(cur_item->GetNextSibling()){
      return cur_item->GetNextSibling();
    } else if (cur_item->GetParent()){
      return SearchNextItem(cur_item->GetParent());
    }else{
      return 0;
    }
  }
  TGListTreeItem *PrevItem(TGListTreeItem *cur_item){
    if(cur_item->GetPrevSibling()){
      return SearchLastItem(cur_item->GetPrevSibling());
    }else if(cur_item->GetParent()){
      return cur_item->GetParent();
    }
    return 0;
  }
  TGListTreeItem *SearchLastItem(TGListTreeItem *cur_item){
    if(cur_item->IsOpen()){
      if(cur_item->GetLastChild()){
	return SearchLastItem(cur_item->GetLastChild());
      }else{
	return cur_item;
      }
    }else{
      return cur_item;
    }
  }
  
  void PrintCanvas(){
    /* Print the canvas. */
    gStyle->SetPaperSize(20,26);
    TString sprinter  = gEnv->GetValue("Print.Printer", "");
    TString sprintCmd = gEnv->GetValue("Print.Command", "lpr");
    Bool_t pname = kTRUE;
    if (sprinter == "")
      pname = kFALSE;
    TString fn = "rootprint";
    FILE *f = gSystem->TempFileName(fn, gEnv->GetValue("Print.Directory", gSystem->TempDirectory()));
    if (f) fclose(f);
    fn += TString::Format(".%s",gEnv->GetValue("Print.FileType", "ps"));
    TCanvas* canvas = gPad->GetCanvas();
    canvas->Print(fn);
    
    TString cmd = sprintCmd;
    if (cmd.Contains("%p"))
      cmd.ReplaceAll("%p", sprinter);
    else if (pname) {
      cmd += " "; cmd += sprinter; cmd += " ";
    }
    
    if (cmd.Contains("%f"))
      cmd.ReplaceAll("%f", fn);
    else {
      cmd += " "; cmd += fn; cmd += " ";
    }
    gSystem->Exec(cmd);
    gSystem->Unlink(fn);
  }
  
  void SetDNDSourceRecursive(TGListTree* lt, TGListTreeItem* item, Bool_t bl){
    TGListTreeItem* cur_item = item;
    while (cur_item){
      /*
        printf("here cur_item %p",cur_item);
        printf("here cur_item %s",cur_item->GetText());
        TClass *cl = TClass::GetClass(((TObject*)(cur_item->GetUserData()))->ClassName());
        if (cl->InheritsFrom("TKey")) {
        cl = TClass::GetClass(((TKey*)(cur_item->GetUserData()))->GetClassName());
        }
        if (cl->InheritsFrom("TFolder")||cl->InheritsFrom("TDirectory")||cl->InheritsFrom("TTree")) {
        lt->DoubleClicked(cur_item,1);
        cur_item->SetOpen(1);
        }
      */
      if(strcmp(cur_item->GetPicture()->GetName(),"folder_t.xpm")==0 ||
	 strcmp(cur_item->GetPicture()->GetName(),"ofolder_t.xpm")==0 ||
         strcmp(cur_item->GetPicture()->GetName(),"rootdb_t.xpm__16x16")==0 ||
         strcmp(cur_item->GetPicture()->GetName(),"tree_t.xpm__16x16")==0 ||
         strcmp(cur_item->GetPicture()->GetName(),"ntuple_t.xpm__16x16")==0 ){
        lt->DoubleClicked(cur_item,1);
        cur_item->SetOpen(1);
      }
      /* printf("cur_item %s\n",cur_item->GetText()); */
      cur_item->SetDNDSource(bl);
      SetDNDSourceRecursive(lt, cur_item->GetFirstChild(), bl);
      if(strcmp(cur_item->GetPicture()->GetName(),"folder_t.xpm")==0 ||
	 strcmp(cur_item->GetPicture()->GetName(),"ofolder_t.xpm")==0 ||
         strcmp(cur_item->GetPicture()->GetName(),"rootdb_t.xpm__16x16")==0 ||
         strcmp(cur_item->GetPicture()->GetName(),"tree_t.xpm__16x16")==0 ||
         strcmp(cur_item->GetPicture()->GetName(),"ntuple_t.xpm__16x16")==0 ){
	lt->DoubleClicked(cur_item,1);
	cur_item->SetOpen(0);
      }
      cur_item = cur_item->GetNextSibling();
    }
  }
  
  void RemoveAndSetText(TGListTreeItem *item, Int_t, UInt_t, Int_t, Int_t){
    TGListTree *lt = (TGListTree*)gTQSender;
    TGListTreeItem* cur_item = item->GetFirstChild();
    while (cur_item){
      /* printf("cur_item %s\n",cur_item->GetText()); */
      TObject *userdata = (TObject*)cur_item->GetUserData();
      if(userdata->InheritsFrom("TKey")){
        TString str = "KEY: ";
        str += userdata->GetName();
        str += "; ";
        str += userdata->GetTitle();
        cur_item->SetText(str);
      }
      if(userdata->InheritsFrom("TH1")){
        TString str = userdata->GetName();
        str += "; ";
        str += userdata->GetTitle();
        cur_item->SetText(str);
      }
      if(strcmp(cur_item->GetPicture()->GetName(),"ofolder_t.xpm")==0 ||
         strcmp(cur_item->GetPicture()->GetName(),"rootdb_t.xpm__16x16")==0 ||
         strcmp(cur_item->GetPicture()->GetName(),"tree_t.xpm__16x16")==0 ||
         strcmp(cur_item->GetPicture()->GetName(),"ntuple_t.xpm__16x16")==0 ) {
        RemoveAndSetText(cur_item,0,0,0,0);
      }
      cur_item = cur_item->GetNextSibling();
    }
  }
  
  void customTListMenu(){
    TClass *cl = gROOT->GetClass("TList");
    TList  *ml = cl->GetMenuList();
    TClassMenuItem *n = new TClassMenuItem(TClassMenuItem::kPopupUserFunction,cl,
					   "Write","writeTList",this,"TObject*",2);
    ml->AddFirst(n);
  }

  void customTDirectoryFileMenu(){
    TClass *cl = gROOT->GetClass("TDirectoryFile");
    TList  *ml = cl->GetMenuList();
    TClassMenuItem *n = new TClassMenuItem(TClassMenuItem::kPopupUserFunction,cl,
					   "Write","writeTDirectoryFile",this,"TObject*",2);
    ml->AddFirst(n);
  }
  
  void customTFolderMenu(){
    TClass *cl = gROOT->GetClass("TFolder");
    TList  *ml = cl->GetMenuList();
    TClassMenuItem *n = new TClassMenuItem(TClassMenuItem::kPopupUserFunction,cl,
					   "Write","writeTFolder",this,"TObject*",2);
    ml->AddFirst(n);
  }

  void writeTList(TObject* c){
    TString file_in_str = c->GetName();
    if(file_in_str.EndsWith(".hb")){
      file_in_str.Resize(file_in_str.Length()-3);
    }
    if(file_in_str.EndsWith(".hbk")){
      file_in_str.Resize(file_in_str.Length()-4);
    }
    if(file_in_str.EndsWith(".hbook")){
      file_in_str.Resize(file_in_str.Length()-6);
    }
    file_in_str += ".root";
    
    TFile *local = TFile::Open(file_in_str,"recreate");
    TIter next((TList*)c);
    TObject *obj;
    while ((obj=next())) {
      obj->Write();
    }
    return;
  }
  
  
  void writeTDirectoryFile(TObject* c){
    TString file_in_str = c->GetTitle();
    if(file_in_str.EndsWith(".hb")){
      file_in_str.Resize(file_in_str.Length()-3);
    }
    if(file_in_str.EndsWith(".hbk")){
      file_in_str.Resize(file_in_str.Length()-4);
    }
    if(file_in_str.EndsWith(".hbook")){
      file_in_str.Resize(file_in_str.Length()-6);
    }
    file_in_str += ".root";
    
    TFile *local = TFile::Open(file_in_str,"recreate");
    TIter next(((TDirectoryFile*)c)->GetList());
    TObject *obj;
    while ((obj=next())) {
      obj->Write();
    }
    /* delete local; */
    return;
  }
  
  void writeTFolder(TObject* c){
    TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("gBrowserEx->GetHistListTree();");  
    TGListTreeItem *item = hist_fListTree->FindItemByObj(hist_fListTree->GetFirstItem(),c);
    TGFileBrowser *hist_browser = (TGFileBrowser *) gROOT->ProcessLine("gBrowserEx->GetHistBrowser();");
    TString fullpath = hist_browser->FullPathName(item);
    
    TDirectory *cur_dir = 0;
    Int_t memo_file_flag = 0;
    TString filename = "";
    if (fullpath.EqualTo("ROOT_Memory")){
      memo_file_flag = 1;
      cur_dir = gROOT;
      filename = fullpath;
    }else if (fullpath.BeginsWith("ROOT_Memory/")){
      memo_file_flag = 1;
      fullpath.Replace(0,12,"");
      cur_dir = (TDirectory *)gROOT->Get(fullpath);
      filename = fullpath;
    }else if(fullpath.BeginsWith("ROOT_Files/")){
      memo_file_flag = 2;
      fullpath.Replace(0,11,"");
      TCollection *lst = gROOT->GetListOfFiles(); 
      TIter next(lst);
      TFile *file;
      while((file=(TFile*)next())){
        filename = file->GetName();
        if(fullpath.BeginsWith(filename)){
          if(fullpath.Length()==filename.Length()){
            cur_dir = (TDirectory *)file;
	}else{
            fullpath.Replace(0,filename.Length()+1,"");
	  cur_dir = (TDirectory *)file->Get(fullpath);
          }
          break;
        }
      }
    }
    printf("Fullpath: %s\n",fullpath.Data());
    printf("Filename: %s\n",filename.Data());
    if (cur_dir==0) {
      return;
    }
    
    TString file_in_str = filename;
    if(file_in_str.EndsWith(".hb")){
      file_in_str.Resize(file_in_str.Length()-3);
    }
    if(file_in_str.EndsWith(".hbk")){
      file_in_str.Resize(file_in_str.Length()-4);
    }
    if(file_in_str.EndsWith(".hbook")){
      file_in_str.Resize(file_in_str.Length()-6);
    }
    file_in_str += ".root";
    
    TFile *local = TFile::Open(file_in_str,"recreate");
    TCollection* col = 0;
    if (memo_file_flag==1) {
      col = ((TDirectory*)cur_dir)->GetList();
    }else if (memo_file_flag==2){
      col = ((TDirectory*)cur_dir)->GetListOfKeys();
    }
    TIter nextobj(col);
    TObject *obj, *objw;
    while ((obj=nextobj())) {
      if (memo_file_flag==2) {
        objw = ((TKey*)obj)->ReadObj();
      }else{
        objw = obj;
      }
      objw->Write();
    }
    return;
  }

  TGListTreeItem *GetActiveMacro(){
    TGListTreeItem *cur_ListTreeItem = macro_fListTree->GetFirstItem();
    while(cur_ListTreeItem){
      if(cur_ListTreeItem->IsActive()){
        return cur_ListTreeItem;
      }
      cur_ListTreeItem = NextItem(cur_ListTreeItem);
    }
    return 0;
  }

  TList *GetListOfActiveHistos(){
    TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetFirstItem();
    list_of_active_histos.Clear();
    while(cur_ListTreeItem){
      if(cur_ListTreeItem->IsActive()){
        TObject *userdata = (TObject*)cur_ListTreeItem->GetUserData();
        if (userdata->InheritsFrom("TKey")){
          userdata = ((TKey*)userdata)->ReadObj();
        }
        if (userdata->InheritsFrom("TH1")){
          list_of_active_histos.Add(userdata);
        }
      }
      cur_ListTreeItem = NextItem(cur_ListTreeItem);
    }
    return &list_of_active_histos;
  }
  
  TList *GetListOfOrderedActiveHistos(){
    list_of_ordered_active_histos.Clear();
    TIter next(&hist_fListTree_active_items);
    TObject * obj;
    while((obj = next())){
      TGListTreeItem *cur_ListTreeItem = (TGListTreeItem *) (((TObjString*)obj)->GetString().Atoll());
      TObject *userdata = (TObject*)cur_ListTreeItem->GetUserData();
      if (userdata->InheritsFrom("TKey")){
        userdata = ((TKey*)userdata)->ReadObj();
      }
      if (userdata->InheritsFrom("TH1")){
        list_of_ordered_active_histos.Add(userdata);
      }
    }
    return &list_of_ordered_active_histos;
  }

  TList *GetHistListTreeActiveHistos(){
    hist_fListTree_active_histos.Clear();
    TIter next(&hist_fListTree_active_items);
    TObject * obj;
    while((obj = next())){
      TGListTreeItem *cur_item = (TGListTreeItem *) (((TObjString*)obj)->GetString().Atoll());
      TClass *cl = TClass::GetClass(((TObject*)(cur_item->GetUserData()))->ClassName());
      if (cl->InheritsFrom("TKey")) {
        cl = TClass::GetClass(((TKey*)(cur_item->GetUserData()))->GetClassName());
      }
      if (cl->InheritsFrom("TH1")){
        hist_fListTree_active_histos.Add(new TObjString(Form("%lld",(unsigned long long)cur_item)));
      }
    }
    return &hist_fListTree_active_histos;
  }

  TGFileBrowserEx *GetFileBrowser(){return file_browser;}
  TGFileBrowserEx *GetHistBrowser(){return hist_browser;}
  TGFileBrowserEx *GetMacroBrowser(){return macro_browser;}
  TGListTree *GetMacroListTree(){return macro_fListTree;}
  TGListTree *GetHistListTree(){return hist_fListTree;}
  TList  *GetHistListTreeActiveItems(){return &hist_fListTree_active_items;}
  void  ResetListLevel(){hist_browser->ResetListLevel();}
  TString GetInitialWorkingDir(){return initial_working_dir;}
  
protected:
  TGFileBrowserEx  *file_browser;
  TGFileBrowserEx  *macro_browser;
  TGFileBrowserEx  *hist_browser;
  TGListTree       *macro_fListTree;
  TGListTree       *hist_fListTree;
  /* This causes sedv when .q was input!
     TGListTreeItem   *hist_fListTreeItem;
  */
  TList            hist_fListTree_active_items;
  TList            hist_fListTree_active_histos;
  TList            list_of_active_histos;
  TList            list_of_ordered_active_histos;
  TString          initial_working_dir;
  ClassDef(TBrowserEx,0)
};

void tbrowserex(){
    gROOT->ProcessLine("TBrowserEx tbrowserex_obj;");
}
#endif
