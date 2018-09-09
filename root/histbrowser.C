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
#include "TTimer.h"
#include "TClassMenuItem.h"
#include "TKey.h"
#include "TFolder.h"
#include "TH1.h"
#include "TH2.h"
#include "KeySymbols.h"

class TGFileBrowserMod : public TGFileBrowser {
public:
  TGFileBrowserMod(const TGWindow *p, TBrowser* b, UInt_t w, UInt_t h) : 
    TGFileBrowser(p, b, w, h){}
  TGListTree* GetListTree(){return fListTree;}
  TGListTreeItem* GetListLevel(){return fListLevel;}
  void ResetListLevel(){fListLevel = 0;}
  TGComboBox* GetDrawOptionPointer(){return fDrawOption;}
  ClassDef(TGFileBrowserMod,0)
};

class TimerManager{
public:
  ~TimerManager(){
    delete timer_add_o_l;
    delete timer_gpad;
    delete timer_dshot;
  }
  TimerManager() :
    timer_add_o_l(0),
    timer_gpad(0),
    timer_dshot(0)
  {
    timer_add_o_l = new TTimer();
    timer_gpad    = new TTimer();
    timer_dshot   = new TTimer();
    timer_add_o_l->Connect("Timeout()","TimerManager",this,"add_obj_to_list()");
    timer_gpad->   Connect("Timeout()","TimerManager",this,"UpdatingGPad()");
    timer_dshot->  Connect("Timeout()","TimerManager",this,"DelayedShot()");
    timer_add_o_l->Start(1000,kFALSE);
    timer_gpad->   Start(1000,kFALSE);
    timer_dshot->  Start(1000,1);
  }
  
  void UpdatingGPad(){
    if (!gROOT->FindObjectAny("shm_flag")) {return;}
    if (!gPad) {
      timer_gpad->Stop();
      return;
    }
    TCanvas* canvas = gPad->GetCanvas();
    canvas->Modified();
    canvas->Update();
    TList* list     = canvas->GetListOfPrimitives();
    if (list->At(0)==0) {return;}
    if (strcmp(list->At(0)->GetName(),"Canvas_1_1")==0){
      Int_t    no_pads  = list->GetSize();
      for (Int_t i=0; i<no_pads; i++){
	((TVirtualPad*)list->At(i))->Modified();
	((TVirtualPad*)list->At(i))->Update();
	//printf("gpad timer\n");
      }
    }
  }
  
  void add_obj_to_list(){
    TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");
    TGListTreeItem *item_root_memo = hist_fListTree->FindChildByName(0, "ROOT_Memory");
    if(!item_root_memo){
      TFolder *tf = new TFolder("ROOT_Memory","ROOT_Memory");
      const TGPicture *pic_folder  = gClient->GetPicture("folder_t.xpm");
      const TGPicture *pic_ofolder = gClient->GetPicture("ofolder_t.xpm");
      item_root_memo =
	hist_fListTree->AddItem(0,tf->GetName(),tf,pic_ofolder,pic_folder,0);
      hist_fListTree->DoubleClicked(item_root_memo,1); item_root_memo->SetOpen(1);
      item_root_memo->CheckItem(0);
    }

    TGListTreeItem *item_root_file = hist_fListTree->FindChildByName(0, "ROOT_Files");
    if(!item_root_file){
      TFolder *tf = new TFolder("ROOT_Files","ROOT_Files");
      const TGPicture *pic_folder  = gClient->GetPicture("folder_t.xpm");
      const TGPicture *pic_ofolder = gClient->GetPicture("ofolder_t.xpm");
      item_root_file =
	hist_fListTree->AddItem(0,tf->GetName(),tf,pic_ofolder,pic_folder,0);
      hist_fListTree->DoubleClicked(item_root_file,1); item_root_file->SetOpen(1);
      item_root_file->CheckItem(0);
    }
    
    TObject *obj = 0;
    Int_t open_cnt = 0;
    TList *lst = gROOT->GetList();
    TIter next(lst);
    while (obj=next()) {
      //printf("obj address: 0x%x\n",obj);
      //printf("here1\n");
      //printf("obj->GetName(): %s\n",obj->GetName());
      //printf("here2\n");
      if(hist_fListTree->FindChildByName(item_root_memo, obj->GetName())){
	//printf("converted found %s\n",obj->GetName());
	continue;
      }
      open_cnt = 0;
      AddObj(obj, item_root_memo, open_cnt);
    }
    TCollection *lst2 = gROOT->GetListOfFiles(); 
    TIter next2(lst2);
    while (obj=next2()) {
      if(hist_fListTree->FindChildByName(item_root_file, obj->GetName())){
	//printf("converted found %s\n",obj->GetName());
	continue;
      }
      open_cnt = 0;
      AddObj(obj, item_root_file, open_cnt);
    }
  }
  
  void AddObj(TObject *obj_in, TGListTreeItem *fListLevel, Int_t open_cnt){
    open_cnt++;
    TObject *obj = obj_in;
    TClass *cl = 0;
    if (obj_in->InheritsFrom(TKey::Class())) {
      const char *classname = ((TKey*)obj_in)->GetClassName();
      cl = gROOT->GetClass(classname);
      if(cl->InheritsFrom(TDirectory::Class())){
	obj = ((TKey*)obj_in)->ReadObj();
      }
    }else{
      const char *classname = obj_in->ClassName();
      cl = gROOT->GetClass(classname);
    }
    TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");
    if (cl->InheritsFrom(TDirectory::Class())) {
      TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");
      TFolder *tf = new TFolder(obj->GetName(),obj->GetName());
      const TGPicture *pic_folder  = gClient->GetPicture("folder_t.xpm");
      const TGPicture *pic_ofolder = gClient->GetPicture("ofolder_t.xpm");
      TGListTreeItem *cur_ListTreeItem =
	hist_fListTree->AddItem(fListLevel, obj->GetName(), tf, pic_ofolder, pic_folder, 0);
      cur_ListTreeItem->CheckItem(0);
      //cur_ListTreeItem->SetUserData(cur_ListTreeItem);
      if (open_cnt <=1) {
	hist_fListTree->DoubleClicked(cur_ListTreeItem,1); cur_ListTreeItem->SetOpen(1);
      }
      TList *lst = ((TDirectory *)obj)->GetListOfKeys();
      if ((lst==0)||(lst->At(0)==0)) {
	lst = ((TDirectory *)obj)->GetList();
      }
      if (lst==0) return;
      TIter nextobj(lst);
      TObject *subobj=0;
      while ((subobj = nextobj())) {
	AddObj(subobj, cur_ListTreeItem, open_cnt);
      }
      /*} else { */
    } else if((cl->InheritsFrom(TH1::Class())) || (cl->InheritsFrom(TTree::Class()))){
      const TGPicture *pic=0;
      TGFileBrowser *hist_browser = (TGFileBrowser *) gROOT->ProcessLine("pHistBrowser->GetHistBrowser();");
      if (cl->InheritsFrom(TH2::Class())) {
	pic = gClient->GetPicture("h2_t.xpm");
      }else if (cl->InheritsFrom(TH1::Class())){
	pic = gClient->GetPicture("h1_t.xpm");
      }else{
	hist_browser->GetObjPicture(&pic, obj);
      }
      //printf("pic name %s\n",pic->GetName());
      TString str = obj->GetName();
      str += "; ";
      str += obj->GetTitle();
      TGListTreeItem *cur_ListTreeItem =
	hist_fListTree->AddItem(fListLevel, str, obj, pic, pic, 0);
      cur_ListTreeItem->SetCheckBox(1);
      cur_ListTreeItem->CheckItem(0);
    }
  }

  void DelayedShot(){
    gROOT->ProcessLine("((TRootBrowser*)pHistBrowser->GetBrowserImp())->GetTabLeft()->SetTab(2,0);");
  }
  

  TTimer *GetTimerAddO2L(){return timer_add_o_l;}
  TTimer *GetTimerUpdatingGPad(){return timer_gpad;}
  TTimer *GetTimerDelayedShot(){return timer_dshot;}
private:
  TTimer *timer_add_o_l;
  TTimer *timer_gpad;
  TTimer *timer_dshot;
  ClassDef(TimerManager,0)
};

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
  while (obj=next()) {
    obj->Write();
  }
  //delete local;
  return;
}

void writeTFolder(TObject* c){
  TGListTree *hist_fListTree = (TGListTree *) gROOT->ProcessLine("pHistBrowser->GetHistListTree();");  
  TGListTreeItem *item = hist_fListTree->FindItemByObj(hist_fListTree->GetFirstItem(),c);
  TGFileBrowser *hist_browser = (TGFileBrowser *) gROOT->ProcessLine("pHistBrowser->GetHistBrowser();");
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
    while(file=(TFile*)next()){
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
  while (obj=nextobj()) {
    if (memo_file_flag==2) {
      objw = ((TKey*)obj)->ReadObj();
    }else{
      objw = obj;
    }
    objw->Write();
  }
  return;
}

class HistBrowser : public TBrowser {
public:
  HistBrowser() :
    TBrowser("tb","roothb browser",0,"FCI"),
    file_browser(0),
    macro_browser(0),
    hist_browser(0),
    macro_fListTree(0),
    hist_fListTree(0),
    hist_fListTree_old_item(0),
    timer_manager(0)
  {
    gROOT->GetListOfBrowsers()->Remove(this);
    //delete GetContextMenu();
    this->GetBrowserImp()->GetMainFrame()->Connect("CloseWindow()", "HistBrowser", this, "CloseWindow()");
    
    timer_manager = new TimerManager();
    
    StartEmbedding(TRootBrowser::kLeft,-1);
    macro_browser = new TGFileBrowserMod(gClient->GetRoot(), this, 200, 500);
    StopEmbedding("Macros");
    
    StartEmbedding(TRootBrowser::kLeft,-1);
    hist_browser = new TGFileBrowserMod(gClient->GetRoot(), this, 200, 500);
    StopEmbedding("Histos");
    
    macro_fListTree = macro_browser->GetListTree();
    macro_browser->AddFSDirectory("kobamac","kobamac","Add");
    macro_browser->ApplyFilter(2);
    SetDNDSourceRecursive(macro_fListTree,macro_fListTree->GetFirstItem(),0);
    TGListTreeItem *ltitem;
    ltitem = macro_fListTree->FindItemByPathname("/kobamac");
    macro_fListTree->DoubleClicked(ltitem,1); ltitem->SetOpen(1);
    ltitem = macro_fListTree->FindItemByPathname("/kobamac/root");
    macro_fListTree->DoubleClicked(ltitem,1); ltitem->SetOpen(1);
    ltitem = macro_fListTree->FindItemByPathname("/kobamac/root/plot_selected");
    macro_fListTree->DoubleClicked(ltitem,1); ltitem->SetOpen(1);
    macro_fListTree->ClearViewPort();
    
    macro_fListTree->Connect("Clicked(TGListTreeItem *, Int_t)",
			     "TGFileBrowserMod", macro_browser,
			     "DoubleClicked(TGListTreeItem *, Int_t)");
    macro_fListTree->Connect("Clicked(TGListTreeItem *, Int_t)",
			     "HistBrowser", this,
			     "MyClicked2(TGListTreeItem *, Int_t)");
    
    hist_fListTree = hist_browser->GetListTree();
    //hist_fListTree->Disconnect("Clicked(TGListTreeItem *, Int_t)");
    hist_fListTree->Connect("Clicked(TGListTreeItem *, Int_t)",
			    "HistBrowser", this,
			    "MyClicked(TGListTreeItem *, Int_t)");
    hist_fListTree->Connect("Clicked(TGListTreeItem *, Int_t)",
			    "HistBrowser", this,
			    "MyClicked2(TGListTreeItem *, Int_t)");
    hist_fListTree->Connect("Clicked(TGListTreeItem *, Int_t)",
			    "TGFileBrowserMod", hist_browser,
			    "DoubleClicked(TGListTreeItem *, Int_t)");
    hist_fListTree->Connect("Clicked(TGListTreeItem *, Int_t, UInt_t, Int_t, Int_t)",
			    "HistBrowser", this,
			    "MyClickedForHistFileBrowser(TGListTreeItem *, Int_t, UInt_t, Int_t, Int_t)");
    hist_fListTree->Connect("Clicked(TGListTreeItem *, Int_t)",
			    "HistBrowser", this,
			    "SetCannotMove(TGListTreeItem *, Int_t)");

    TQObject::Connect("TCanvas","ProcessedEvent(Int_t,Int_t,Int_t,TObject*)",
		      "HistBrowser", this, "change_canvas(Int_t,Int_t,Int_t,TObject*)");
    TQObject::Connect("TGFrame","ProcessedEvent(Event_t*)","HistBrowser", this, "HandleKey(Event_t*)");
    
    hist_browser->GetDrawOptionPointer()->GetTextEntry()->SetText("colz");
    cumtomTDirectoryFileMenu();
    cumtomTFolderMenu();
  }
  
  ~HistBrowser(){
    //delete hist_browser;
    //delete macro_browser;
    //delete timer_manager;
  }
  
  void CloseWindow(){
    delete timer_manager;
    gApplication->Terminate();
    //this->GetBrowserImp()->GetMainFrame()->CloseWindow();
    
    //this->Delete();
    //delete this;
    //gClient->Delete();
  }
  
  void HandleKey(Event_t* event){
    char   input[10];
    UInt_t keysym;
    if ((event->fType == kGKeyPress)) {
      gVirtualX->LookupString(event, input, sizeof(input), keysym);
      //printf("(EKeySym)keysym %d\n", (EKeySym)keysym);

      TString gTQSender_name = ((TObject*)gTQSender)->GetName();
      if (gTQSender_name.BeginsWith("fCompositeFrame")){
	return;
      }

      Bool_t inside_of_filebrowser = false;
      if (gTQSender_name.BeginsWith("fListTree")){
	inside_of_filebrowser = true;
      }

      //std::cout << "event->fX" << event->fX<< std::endl;
      //std::cout << "event->fY" << event->fY<< std::endl;
      //std::cout << "((TObject*)gTQSender)->GetName()" << ((TObject*)gTQSender)->GetName()<< std::endl;

      TGTab * ptab = ((TRootBrowser*)this->GetBrowserImp())->GetTabLeft();
      
      Int_t ntab = ptab->GetNumberOfTabs();
      if (event->fState & kKeyMod1Mask){
 	if (keysym == kKey_f) { ptab->SetTab(0,0); }
	if (keysym == kKey_m) { ptab->SetTab(1,0); }
	if (keysym == kKey_h) { ptab->SetTab(2,0); }
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
      prev_keysym = keysym;

      if (event->fState & kKeyControlMask) {
	if (keysym == kKey_p) {
	  PrintCanvas();
	}
      }     
      
      if (keysym == kKey_g) {
	gROOT->ProcessLine(".x fit_p1g.C");
      }
      if (keysym == kKey_c) {
	gROOT->ProcessLine(".x fit_p1g_clear.C");
      }
      
      if ((keysym == kKey_Up)||
	  (keysym == kKey_Down)||
	  (keysym == kKey_Return)||
	  (keysym == kKey_Enter)||
	  (keysym == kKey_Space)) {
	TGListTree *cur_ListTree = 0; 
	TGFileBrowser *cur_FileBrowser = 0; 
	if(strcmp(ptab->GetCurrentTab()->GetText()->Data(),"Macros")==0){
	  cur_ListTree    = macro_fListTree;
	  cur_FileBrowser = macro_browser;
	}else if(strcmp(ptab->GetCurrentTab()->GetText()->Data(),"Histos")==0){
	  cur_ListTree    = hist_fListTree;
	  cur_FileBrowser = hist_browser;
	}else{
	  return;
	}

	if ((keysym == kKey_Space)) {
	  TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetFirstItem();
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
	    TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetCurrent();
	    cur_ListTree->CheckItem(cur_ListTreeItem, !cur_ListTreeItem->IsChecked());
	  }
	  if((first_found)&&
	     inside_of_filebrowser&&
	     (!hist_fListTree->GetCurrent()->IsActive())){
	    TGListTreeItem *cur_ListTreeItem = hist_fListTree->GetCurrent();
	    cur_ListTree->CheckItem(cur_ListTreeItem, !cur_ListTreeItem->IsChecked());
	  }
	  cur_ListTree->ClearViewPort();
	}
	
	//std::cout << "cur_ListTree->GetBelowMouse(): " << cur_ListTree->GetBelowMouse() << std::endl;
	
	if ((!(event->fState & kKeyShiftMask))&&
	    (!(event->fState & kKeyControlMask))) {
	  if ((keysym == kKey_Up)||(keysym == kKey_Down)) {
	    if((cur_ListTree->GetSelected())){
	      cur_ListTree->GetSelected()->SetActive(kFALSE);
	    }
	    cur_ListTree->UnselectAll(kTRUE);
	    hist_fListTree_old_active_item.Clear();
	    cur_ListTree->ClearViewPort();
	  }
	}

	if (inside_of_filebrowser) {
	  if (event->fState & kKeyShiftMask) {
	    if ((keysym == kKey_Up)||(keysym == kKey_Down)) {
	      TGListTreeItem * old_item = 0;
	      if(keysym == kKey_Up){
		old_item = NextItem(cur_ListTree->GetCurrent());
	      }
	      if(keysym == kKey_Down){
		old_item = PrevItem(cur_ListTree->GetCurrent());
	      }
	      if(old_item){
		cur_ListTree->HighlightItem(old_item,kTRUE,kTRUE);
	      }
	      TGListTreeItem * cur_item = cur_ListTree->GetCurrent();
	      cur_ListTree->HighlightItem(cur_item,kTRUE,kTRUE);
	      cur_ListTree->ClearViewPort();

	      if(strcmp(ptab->GetCurrentTab()->GetText()->Data(),"Histos")==0){
		TGListTreeItem *cur_ListTreeItem = 0;
		hist_fListTree_old_active_item.Clear();
		cur_ListTreeItem = hist_fListTree->GetFirstItem();
		while(cur_ListTreeItem){
		  if(cur_ListTreeItem->IsActive()){
		    hist_fListTree_old_active_item.Add((TObject*)cur_ListTreeItem);
		  }
		  cur_ListTreeItem = NextItem(cur_ListTreeItem);
		}
	      }
	    }
	  } 
	  return;
	}
	
	//std::cout << "cur_ListTree->GetCurrent():" << cur_ListTree->GetCurrent()<< std::endl;
	//if(cur_ListTree->GetCurrent()){
	//  std::cout << "cur_ListTree->GetCurrent():" << cur_ListTree->GetCurrent()->GetText()Name()<< std::endl;
	//}
	
	if (!cur_ListTree->GetCurrent()) {
	  TGPosition pos = cur_ListTree->GetPagePosition();
	  Event_t event_tmp; 
	  event_tmp.fY = pos.fY + 2;
	  cur_ListTree->HandleMotion(&event_tmp);
	  event_tmp.fY = -1;
	  cur_ListTree->HandleMotion(&event_tmp);
	}
	
	if ((keysym == kKey_Up)||(keysym == kKey_Down)) {
	  TGListTreeItem * cur_item = cur_ListTree->GetCurrent();
	  if (event->fState & kKeyShiftMask) {
	    cur_ListTree->HighlightItem(cur_item,kTRUE,kTRUE);
	  }
	  if (keysym == kKey_Down) {
	    cur_ListTree->LineDown(1);
	  }
	  if (keysym == kKey_Up) {
	    cur_ListTree->LineUp(1);
	  }
	  if (event->fState & kKeyShiftMask) {
	    cur_item = cur_ListTree->GetCurrent();
	    cur_ListTree->HighlightItem(cur_item,kTRUE,kTRUE);
	  }
	  cur_ListTree->ClearViewPort();
	}
	
	if ((keysym == kKey_Return)||
	    (keysym == kKey_Enter)) {
	  TGListTreeItem * cur = cur_ListTree->GetCurrent();
	  cur_ListTree->DoubleClicked(cur, 1);
	  cur_ListTree->HighlightItem(cur,kTRUE,kTRUE);
	  cur_ListTree->ClearViewPort();
	}

	if(strcmp(ptab->GetCurrentTab()->GetText()->Data(),"Histos")==0){
	  TGListTreeItem *cur_ListTreeItem = 0;
	  hist_fListTree_old_active_item.Clear();
	  cur_ListTreeItem = hist_fListTree->GetFirstItem();
	  while(cur_ListTreeItem){
	    if(cur_ListTreeItem->IsActive()){
	      hist_fListTree_old_active_item.Add((TObject*)cur_ListTreeItem);
	    }
	    cur_ListTreeItem = NextItem(cur_ListTreeItem);
	  }
	}
      }
    }
  }

  TGListTreeItem *NextItem(TGListTreeItem *cur_item){
    if(strcmp(cur_item->GetPicture()->GetName(),"ofolder_t.xpm")==0){
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
    if(strcmp(cur_item->GetPicture()->GetName(),"ofolder_t.xpm")==0){
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
    // Print the canvas.
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

  
  void KeyPressed(Int_t key){
    //printf("%d\n",key);
  }

  void KeyPressed(TGListTreeItem *entry, UInt_t keysym, UInt_t mask){
    printf("keysym %d \n", keysym);
  }

  void KeyPressed(TGFrame*,UInt_t,UInt_t){
    printf("TGFrame \n");
  }
  
  void SetDNDSourceRecursive(TGListTree* lt, TGListTreeItem* item, Bool_t bl){
    TGListTreeItem* cur_item = item;
    while (cur_item){
      if(strcmp(cur_item->GetPicture()->GetName(),"folder_t.xpm")==0 ||
	 strcmp(cur_item->GetPicture()->GetName(),"ofolder_t.xpm")==0){
	lt->DoubleClicked(cur_item,1);
	cur_item->SetOpen(1);
      }
      //printf("cur_item %s\n",cur_item->GetText());
      cur_item->SetDNDSource(bl);
      SetDNDSourceRecursive(lt, cur_item->GetFirstChild(), bl);
      if(strcmp(cur_item->GetPicture()->GetName(),"folder_t.xpm")==0 ||
	 strcmp(cur_item->GetPicture()->GetName(),"ofolder_t.xpm")==0){
	lt->DoubleClicked(cur_item,1);
	cur_item->SetOpen(0);
      }
      cur_item = cur_item->GetNextSibling();
    }
  }

  void cumtomTDirectoryFileMenu(){
    TClass *cl = gROOT->GetClass("TDirectoryFile");
    TList  *ml = cl->GetMenuList();
    TClassMenuItem *n = new TClassMenuItem(TClassMenuItem::kPopupUserFunction,cl,
					   "Write","writeTDirectoryFile",0,"TObject*",2);
    ml->AddFirst(n);
  }
  
  void cumtomTFolderMenu(){
    TClass *cl = gROOT->GetClass("TFolder");
    TList  *ml = cl->GetMenuList();
    TClassMenuItem *n = new TClassMenuItem(TClassMenuItem::kPopupUserFunction,cl,
					   "Write","writeTFolder",0,"TObject*",2);
    ml->AddFirst(n);
  }
  
  void MyClicked2(TGListTreeItem *item, Int_t /*btn*/){
    item->SetOpen(!item->IsOpen());
  }
  
  void MyClicked(TGListTreeItem *item, Int_t /*btn*/){
    TGListTree *lt = (TGListTree*)gTQSender;
    TObject *userdata = (TObject*)item->GetUserData();
    if (userdata->InheritsFrom("TKey")){
      item->SetUserData(((TKey*)userdata)->ReadObj());
    }
    if(strcmp(item->GetPicture()->GetName(),"folder_t.xpm")==0 ||
       strcmp(item->GetPicture()->GetName(),"ofolder_t.xpm")==0
       ){return;}
    TString opt = lt->GetParent()->GetParent()->GetParent()->GetDrawOption();
    if (opt == "same") {return;}
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
    if(mask & kKeyShiftMask){
      hist_fListTree->UnselectAll(kTRUE);
      //std::cout << "mask & kKeyShiftMask" <<std::endl;
      //std::cout << "entry->GetText():" << entry->GetText() << std::endl;
      if (hist_fListTree_old_item) {
	TGListTreeItem  *cur_item = hist_fListTree_old_item;
	//std::cout << "cur_item->GetText():" << cur_item->GetText() << std::endl;
	Bool_t go_up   = false;
	Bool_t go_down = false;
	while (cur_item){
	  if(cur_item == entry){
	    go_up = true;
	    break;
	  }
	  cur_item = PrevItem(cur_item);
	}
	cur_item = hist_fListTree_old_item;
	while (cur_item){
	  if(cur_item == entry){
	    go_down = true;
	    break;
	  }
	  cur_item = NextItem(cur_item);
	}
	if(go_up || go_down){
	  cur_item = hist_fListTree_old_item;
	  while (cur_item){
	    hist_fListTree->HighlightItem(cur_item,kTRUE,kTRUE);
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
      hist_fListTree->SetSelected(entry);
    }else{
      hist_fListTree_old_item = hist_fListTree->GetSelected();
    }

    TGListTreeItem *cur_ListTreeItem = 0;
    if(mask & kKeyControlMask){
      TIter next(&hist_fListTree_old_active_item);
      while(cur_ListTreeItem = (TGListTreeItem *)next()){
	hist_fListTree->HighlightItem(cur_ListTreeItem,kTRUE,kTRUE);
      }
    }

    hist_fListTree_old_active_item.Clear();
    cur_ListTreeItem = hist_fListTree->GetFirstItem();
    while(cur_ListTreeItem){
      if(cur_ListTreeItem->IsActive()){
	hist_fListTree_old_active_item.Add((TObject*)cur_ListTreeItem);
      }
      cur_ListTreeItem = NextItem(cur_ListTreeItem);
    }
    hist_fListTree->ClearViewPort();
    return;
  }
  
  void SetCannotMove(TGListTreeItem *item, Int_t /* no use */){
    TCanvas* canvas = gPad->GetCanvas();
    if (canvas == 0) {
      std::cout << "There is no canvas." << std::endl;
      return;
    }
    TVirtualPad *sel_pad = canvas->GetPad(gPad->GetNumber());
    if (sel_pad == 0) {
      std::cout << "There is no sel_pad." << std::endl;
      return;
    }
    sel_pad->GetFrame()->SetBit(TBox::kCannotMove);
  }
  
  void change_canvas(Int_t event, Int_t x, Int_t y, TObject* selected) {
    TCanvas *c = (TCanvas*)gTQSender;
    //    printf("Canvas %s: event=%d, x=%d, y=%d, selected=%s\n", c->GetName(),
    //	   event, x, y, selected->IsA()->GetName());
    if (event==kButton1Down) { 
      if (selected->InheritsFrom("TPad")||
	  selected->InheritsFrom("TFrame")||
	  selected->InheritsFrom("TH2F")){
	TVirtualPad *cur_pad  = c->GetSelectedPad();
	cur_pad->cd();
	gPad->Update();
      }
    }
  }

  TGFileBrowserMod *GetFileBrowser(){return file_browser;}
  TGFileBrowserMod *GetHistBrowser(){return hist_browser;}
  TGFileBrowserMod *GetMacroBrowser(){return macro_browser;}
  TGListTree *GetMacroListTree(){return macro_fListTree;}
  TGListTree *GetHistListTree(){return hist_fListTree;}
  TTimer *GetTimerAddO2L(){return timer_manager->GetTimerAddO2L();}
  TTimer *GetTimerUpdatingGPad(){return timer_manager->GetTimerUpdatingGPad();}
  TTimer *GetTimerDelayedShot(){return timer_manager->GetTimerDelayedShot();}
  void  ResetListLevel(){hist_browser->ResetListLevel();}
protected:
  TGFileBrowserMod *file_browser;
  TGFileBrowserMod *macro_browser;
  TGFileBrowserMod *hist_browser;
  TGListTree       *macro_fListTree;
  TGListTree       *hist_fListTree;
  TGListTreeItem   *hist_fListTree_old_item;
  TList            hist_fListTree_old_active_item;
  TimerManager     *timer_manager;
  ClassDef(HistBrowser,0)
};

HistBrowser *pHistBrowser;
void histbrowser(){
#ifdef __CINT__
  printf("This macro should be compiled by ACLiC\n");
  return;
#endif
  if (gROOT->FindObjectAny("histbrowser_flag")){
    printf("Warning: alredy histbrowser is there!\n");
    return;
  }
  gROOT->Add(new TNamed("histbrowser_flag","histbrowser_flag"));
  pHistBrowser = new HistBrowser();
}
