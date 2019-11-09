//#include <iostream>
//#include <sstream>
//#include <string>
//#include <TROOT.h>
//#include <TClass.h>
//#include <TApplication.h>
//#include <TCanvas.h>
//#include <TGClient.h>
//#include <TGLabel.h>
//#include <TGListTree.h>
//#include <TGButton.h>
//#include <TGTextEdit.h>
//#include <TGFrame.h>
//#include <TString.h>
//#include <TKey.h>
//#include <TH2.h>
//#include <TF2.h>
//#include <TMath.h>

char *editortxt1 =
"This is the ROOT text edit widget TGTextEdit. It is not intended as\n"
"a full developers editor, but it is relatively complete and can ideally\n"
"be used to edit scripts or to present users editable config files, etc.\n\n"
"The text edit widget supports standard emacs style ctrl-key navigation\n"
"in addition to the arrow keys. By default the widget has under the right\n"
"mouse button a popup menu giving access to several built-in functions.\n\n"
"Cut, copy and paste between different editor windows and any other\n"
"standard text handling application is supported.\n\n"
"Text can be selected with the mouse while holding the left button\n"
"or with the arrow keys while holding the shift key pressed. Use the\n"
"middle mouse button to paste text at the current mouse location."
;

class Editor2 {

private:
   TGTransientFrame *fMain2;   // main frame of this widget
   TGTextEdit       *fEdit2;   // text edit widget
   TGTextButton     *fOK;     // OK button
   TGLayoutHints    *fL1;     // layout of TGTextEdit
   TGLayoutHints    *fL2;     // layout of OK button

public:
   Editor2(TGWindow *main, UInt_t w, UInt_t h);
   virtual ~Editor2();

   void   LoadFile(char *file);
   void   LoadBuffer(char *buffer);
   void   AddBuffer(char *buffer);

   TGTextEdit *GetEditor2() { return fEdit2; }

   void   SetTitle();
   void   Popup();

   // slots
   void   CloseWindow2();
   void   DoOK();
   void   DoOpen();
   void   DoSave();
   void   DoClose2();
};

Editor2::Editor2(TGWindow *main, UInt_t w, UInt_t h)
{
   // Create an editor in a dialog.

   fMain2 = new TGTransientFrame(gClient->GetRoot(), main, w, h);
   //fMain2 = new TGTransientFrame(0, main, w, h);
   fMain2->Connect("CloseWindow()", "Editor2", this, "CloseWindow2()");
   fMain2->DontCallClose(); // to avoid double deletions.

   // use hierarchical cleaning
   fMain2->SetCleanup(kDeepCleanup);

   fEdit2 = new TGTextEdit(fMain2, w, h, kSunkenFrame | kDoubleBorder);
   fL1 = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 3, 3, 3, 3);
   fMain2->AddFrame(fEdit2, fL1);
   fEdit2->Connect("Opened()", "Editor2", this, "DoOpen()");
   fEdit2->Connect("Saved()",  "Editor2", this, "DoSave()");
   fEdit2->Connect("Closed()", "Editor2", this, "DoClose2()");

   // set selected text colors
   Pixel_t pxl;
   gClient->GetColorByName("#3399ff", pxl);
   fEdit2->SetSelectBack(pxl);
   fEdit2->SetSelectFore(TGFrame::GetWhitePixel());

   fOK = new TGTextButton(fMain2, "  &OK  ");
   fOK->Connect("Clicked()", "Editor2", this, "DoOK()");
   fL2 = new TGLayoutHints(kLHintsBottom | kLHintsCenterX, 0, 0, 5, 5);
   fMain2->AddFrame(fOK, fL2);

   SetTitle();

   fMain2->MapSubwindows();

   fMain2->Resize();

   // editor covers right half of parent window
   fMain2->CenterOnParent(kTRUE, TGTransientFrame::kRight);
}

Editor2::~Editor2()
{
   // Delete editor dialog.

   fMain2->DeleteWindow();  // deletes fMain2
}

void Editor2::SetTitle()
{
   // Set title in editor window.

   TGText *txt = GetEditor2()->GetText();
   Bool_t untitled = !strlen(txt->GetFileName()) ? kTRUE : kFALSE;

   char title[256];
   if (untitled)
      sprintf(title, "ROOT Editor2 - Untitled");
   else
      sprintf(title, "ROOT Editor2 - %s", txt->GetFileName());

   fMain2->SetWindowName(title);
   fMain2->SetIconName(title);
}

void Editor2::Popup()
{
   // Show editor.

   fMain2->MapWindow();
}

void Editor2::LoadBuffer(char *buffer)
{
   // Load a text buffer in the editor.

   fEdit2->LoadBuffer(buffer);
}

void Editor2::LoadFile(char *file)
{
   // Load a file in the editor.

   fEdit2->LoadFile(file);
}

void Editor2::AddBuffer(char *buffer)
{
   // Add text to the editor.

   TGText txt;
   txt.LoadBuffer(buffer);
   fEdit2->AddText(&txt);
}

void Editor2::CloseWindow2()
{
   // Called when closed via window manager action.

   delete this;
}

void Editor2::DoOK()
{
   // Handle ok button.

   this->CloseWindow2();
}

void Editor2::DoOpen()
{
   this->SetTitle();
}

void Editor2::DoSave()
{
   this->SetTitle();
}

void Editor2::DoClose2()
{
   // Handle close button.

   this->CloseWindow2();
}

void grtest(){
  //TGMainFrame * fMain2 = (TGMainFrame *) gROOT->ProcessLine("gBrowserEx->GetBrowserImp()->GetMainFrame();");
  //Editor2 *ed = new Editor2(fMain2, 600, 400);
  Editor2 *ed2 = new Editor2(0, 600, 400);
  ed2->LoadBuffer(editortxt1);
  ed2->Popup();
}
