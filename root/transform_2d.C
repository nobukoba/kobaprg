#include <TROOT.h>
#include <TClass.h>
#include <TApplication.h>
#include <TGClient.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGTextEdit.h>
#include <TGFrame.h>

const char *editortxt1 =
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
const char *editortxt2 =
"Mice with scroll-ball are properly supported.\n\n"
"This are the currently defined key bindings:\n"
"Left Arrow\n"
"    Move the cursor one character leftwards.\n"
"    Scroll when cursor is out of frame.\n"
"Right Arrow\n"
"    Move the cursor one character rightwards.\n"
"    Scroll when cursor is out of frame.\n"
"Backspace\n"
"    Deletes the character on the left side of the text cursor and moves the\n"
"    cursor one position to the left. If a text has been marked by the user"
;
const char *editortxt3 =
"    (e.g. by clicking and dragging) the cursor will be put at the beginning\n"
"    of the marked text and the marked text will be removed.\n"
"Home\n"
"    Moves the text cursor to the left end of the line. If mark is TRUE text\n"
"    will be marked towards the first position, if not any marked text will\n"
"    be unmarked if the cursor is moved.\n"
"End\n"
"    Moves the text cursor to the right end of the line. If mark is TRUE text\n"
"    will be marked towards the last position, if not any marked text will\n"
"    be unmarked if the cursor is moved.\n"
"Delete"
;
const char *editortxt4 =
"    Deletes the character on the right side of the text cursor. If a text\n"
"    has been marked by the user (e.g. by clicking and dragging) the cursor\n"
"    will be put at the beginning of the marked text and the marked text will\n"
"    be removed.\n"
"Shift - Left Arrow\n"
"    Mark text one character leftwards.\n"
"Shift - Right Arrow\n"
"    Mark text one character rightwards.\n"
"Control-A\n"
"    Select the whole text.\n"
"Control-B\n"
"    Move the cursor one character leftwards."
;
const char *editortxt5 =
"Control-C\n"
"    Copy the marked text to the clipboard.\n"
"Control-D\n"
"    Delete the character to the right of the cursor.\n"
"Control-E\n"
"    Move the cursor to the end of the line.\n"
"Control-F\n"
"    Start Search Dialog.\n"
"Control-H\n"
"    Delete the character to the left of the cursor.\n"
"Control-K\n"
"    Delete marked text if any or delete all\n"
"    characters to the right of the cursor.\n"
"Control-L\n"
"    Start GoTo Line Dialog"
;
const char *editortxt6 =
"Control-U\n"
"    Delete all characters on the line.\n"
"Control-V\n"
"    Paste the clipboard text into line edit.\n"
"Control-X\n"
"    Cut the marked text, copy to clipboard.\n"
"Control-Y\n"
"    Paste the clipboard text into line edit.\n"
"Control-Z\n"
"    Undo action.\n\n"
"All other keys with valid ASCII codes insert themselves into the line.";

class Editor {
private:
   TGTransientFrame *fMain;   // main frame of this widget
   TGHorizontalFrame *fHframe1, *fHframe2;
   TGTextEdit       *fEdit1, *fEdit2;
   TGTextButton     *fOK, *fClose;
   TGLayoutHints    *fL1, *fL2, *fL3;

public:
   Editor(const TGWindow *main, UInt_t w, UInt_t h);
   virtual ~Editor();
   void   LoadBuffer(const char *buffer);
   void   AddBuffer(const char *buffer);
   TGTextEdit *GetEditor1() const { return fEdit1; }
   TGTextEdit *GetEditor2() const { return fEdit2; }
   void   Popup();
   void   CloseWindow();
   void   DoOK();
   void   DoClose();
};

Editor::Editor(const TGWindow *main, UInt_t w, UInt_t h)
{
   // Create an editor in a dialog.
   fMain = new TGTransientFrame(gClient->GetRoot(), main, w, h);
   fMain->Connect("CloseWindow()", "Editor", this, "CloseWindow()");
   fMain->DontCallClose(); // to avoid double deletions.

   // use hierarchical cleaning
   fMain->SetCleanup(kDeepCleanup);

   fHframe1 = new TGHorizontalFrame(fMain, 0, 0, 0);

   fEdit1 = new TGTextEdit(fHframe1, w/2., h, kSunkenFrame | kDoubleBorder);
   fL1 = new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2);
   fHframe1->AddFrame(fEdit1, fL1);
   fEdit2 = new TGTextEdit(fHframe1, w/2., h, kSunkenFrame | kDoubleBorder);
   fHframe1->AddFrame(fEdit2, fL1);
   fL2 = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 3, 3, 3, 3);
   fMain->AddFrame(fHframe1,fL2);

   // set selected text colors
   Pixel_t pxl;
   gClient->GetColorByName("#3399ff", pxl);
   fEdit1->SetSelectBack(pxl);
   fEdit1->SetSelectFore(TGFrame::GetWhitePixel());
   fEdit2->SetSelectBack(pxl);
   fEdit2->SetSelectFore(TGFrame::GetWhitePixel());

   fHframe2 = new TGHorizontalFrame(fMain, 0, 0, 0);
   fOK = new TGTextButton(fHframe2, "  &OK  ");
   fOK->Connect("Clicked()", "Editor", this, "DoOK()");
   fClose = new TGTextButton(fHframe2, "  &Close  ");
   fClose->Connect("Clicked()", "Editor", this, "DoClose()");

   fHframe2->AddFrame(fOK, fL1);
   fHframe2->AddFrame(fClose, fL1);

   fL3 = new TGLayoutHints(kLHintsBottom | kLHintsCenterX, 0, 0, 5, 5);
   fMain->AddFrame(fHframe2, fL3);

   fMain->SetWindowName("f(x,y) g(x,y)");
   fMain->SetIconName("f(x,y) g(x,y)");
   fMain->MapSubwindows();
   fMain->Resize();
   // editor covers right half of parent window
   fMain->CenterOnParent(kTRUE, TGTransientFrame::kRight);
}

Editor::~Editor(){
   fMain->DeleteWindow();  // deletes fMain
}
void Editor::Popup(){
   fMain->MapWindow();
}
void Editor::LoadBuffer(const char *buffer){
   fEdit1->LoadBuffer(buffer);
}
void Editor::AddBuffer(const  char *buffer) {
   TGText txt;
   txt.LoadBuffer(buffer);
   fEdit1->AddText(&txt);
}
void Editor::CloseWindow(){ delete this; }
void Editor::DoOK(){ CloseWindow(); }
void Editor::DoClose(){ CloseWindow(); }

void transform_2d(){
   // Handle button click.
   Editor *ed = new Editor(0, 600, 400);
   ed->LoadBuffer(editortxt1);
   ed->AddBuffer(editortxt2);
   ed->AddBuffer(editortxt3);
   ed->AddBuffer(editortxt4);
   ed->AddBuffer(editortxt5);
   ed->AddBuffer(editortxt6);
   ed->Popup();
}
