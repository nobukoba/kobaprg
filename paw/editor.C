#include <string.h>
#include <stdio.h>
#include "TROOT.h"
#include "TString.h"
#include "TGFileBrowser.h"
#include "TGInputDialog.h"
#include "TGClient.h"

class Editor {
private:
  TGTransientFrame *fMain;   // main frame of this widget
  TGTextEdit       *fEdit;   // text edit widget
  TGTextButton     *fOK;     // OK button
  TGLayoutHints    *fL1;     // layout of TGTextEdit
  TGLayoutHints    *fL2;     // layout of OK button
public:
  Editor(const TGWindow *main, UInt_t w, UInt_t h){
    fMain = new TGTransientFrame(gClient->GetRoot(), main, w, h);
    fMain->Connect("CloseWindow()", "Editor", this, "CloseWindow()");
    fMain->DontCallClose(); // to avoid double deletions.

    // use hierarchical cleaning
    fMain->SetCleanup(kDeepCleanup);

    fEdit = new TGTextEdit(fMain, w, h, kSunkenFrame | kDoubleBorder);
    fL1 = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 3, 3, 3, 3);
    fMain->AddFrame(fEdit, fL1);
    fEdit->Connect("Opened()", "Editor", this, "DoOpen()");
    fEdit->Connect("Saved()",  "Editor", this, "DoSave()");
    fEdit->Connect("Closed()", "Editor", this, "DoClose()");

    // set selected text colors
    Pixel_t pxl;
    gClient->GetColorByName("#3399ff", pxl);
    fEdit->SetSelectBack(pxl);
    fEdit->SetSelectFore(TGFrame::GetWhitePixel());

    fOK = new TGTextButton(fMain, "  &OK  ");
    fOK->Connect("Clicked()", "Editor", this, "DoOK()");
    fL2 = new TGLayoutHints(kLHintsBottom | kLHintsCenterX, 0, 0, 5, 5);
    fMain->AddFrame(fOK, fL2);
    SetTitle();
    fMain->MapSubwindows();
    fMain->Resize();
    fMain->CenterOnParent(kTRUE, TGTransientFrame::kRight);
  }
  ~Editor()
  {
    fMain->DeleteWindow();  // deletes fMain
  }
  void SetTitle(){
    TGText *txt = GetEditor()->GetText();
    Bool_t untitled = !strlen(txt->GetFileName()) ? kTRUE : kFALSE;
    char title[256];
    if (untitled)
      sprintf(title, "ROOT Editor - Untitled");
    else
      sprintf(title, "ROOT Editor - %s", txt->GetFileName());
    fMain->SetWindowName(title);
    fMain->SetIconName(title);
  }
  void Popup(){
    fMain->MapWindow();
  }
  void LoadBuffer(const char *buffer){
    fEdit->LoadBuffer(buffer);
  }
  void LoadFile(const char *file){
    fEdit->LoadFile(file);
  }
  void AddBuffer(const  char *buffer){
    TGText txt;
    txt.LoadBuffer(buffer);
    fEdit->AddText(&txt);
  }
  void CloseWindow(){ delete this; }
  void DoOK(){ CloseWindow(); }
  void DoOpen(){ SetTitle();}
  void DoSave(){ SetTitle(); }
  void DoClose(){ CloseWindow(); }
  TGTextEdit *GetEditor() const { return fEdit; }
};

void zon_x_y(){
  char command[1024];
  strlcpy(command, "2 2",sizeof(command));
  new TGInputDialog(gClient->GetRoot(),tb->GetBrowserImp()->GetMainFrame(),
					   "Enter numer of colums and rows: nx ny",
					   command,command);


  //gClient->SetWaitForWindow(gClient->GetRoot()->GetId());
  //  TString str = command;
  //  printf("%s\n",str.Data());
  //  str.ReplaceAll(","," ");
  //  Int_t nx, ny;
  //  sscanf(str.Data(),"%d %d",&nx,&ny);
  //  printf("%d %d\n",nx,ny);
  //  TCanvas* canvas = gPad->GetCanvas();
  //  canvas->Clear();
  //  canvas->Divide(nx,ny);
  //  canvas->cd(1);
  //  canvas->Update();
  return;
}
