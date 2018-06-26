#include "TGObject.h"
#include "TROOT.h"
#include "TGFrame.h"
#include "TGString.h"
#include "TGPicture.h"
#include "TGWidget.h"
#include "TGButton.h"
#include "TGIcon.h"
#include "TGLabel.h"
#include "TList.h"
#include "TGClient.h"
#include "KeySymbols.h"

class TGMsgBoxMod : public TGTransientFrame {
private:
  TGButton            *fClose;   // buttons in dialog box
  TGHorizontalFrame   *fButtonFrame;                // frame containing buttons
  TGVerticalFrame     *fLabelFrame;                 // frame containing text
  TGLayoutHints       *fL1, *fL2, *fL3, *fL4; // layout hints
  TList               *fMsgList;                    // text (list of TGLabels)
public:
  TGMsgBoxMod(const TGWindow *p, const TGWindow *main,
	      const char *title, const char *msg,
	      UInt_t options = kVerticalFrame,
	      Int_t text_align = kTextCenterX | kTextCenterY) :
    TGTransientFrame(p, main, 10, 10, options){
    UInt_t width, height;
    fClose = 0;
    fMsgList = new TList;
    width = 0;
    fButtonFrame = new TGHorizontalFrame(this, 60, 20, kFixedWidth);
    fL1 = new TGLayoutHints(kLHintsCenterY | kLHintsExpandX, 3, 3, 0, 0);
    fClose = new TGTextButton(fButtonFrame, new TGHotString("C&lose"));
    fClose->Connect("Pressed()", "TGTransientFrame", this, "DeleteWindow()");
    fButtonFrame->AddFrame(fClose, fL1);
    fL2 = new TGLayoutHints(kLHintsBottom | kLHintsCenterX, 0, 0, 5, 5);
    AddFrame(fButtonFrame, fL2);
    fLabelFrame = new TGVerticalFrame(this, 60, 20);
    fL3 = new TGLayoutHints(kLHintsCenterY | kLHintsLeft | kLHintsExpandX,
			    4, 2, 2, 2);
    fL4 = new TGLayoutHints(kLHintsTop | kLHintsExpandX, 10, 10, 7, 2);
    // make one label per line of the message
    TGLabel *label;
    TGFont *fLabelFont = gClient->GetFont("-*-*-*-*-*-20-*-*-*-*-*-*-*", kFALSE);
    Pixel_t red;
    gClient->GetColorByName("red", red);
    
    char *line;
    char *tmpMsg, *nextLine;
    int len = strlen(msg) + 1;
    tmpMsg = new char[len];
    nextLine = tmpMsg;
    line = tmpMsg;
    strlcpy(nextLine, msg, len);
    while ((nextLine = strchr(line, '\n'))) {
      *nextLine = 0;
      label = new TGLabel(fLabelFrame, line);
      label->SetTextColor(red);
      label->SetTextFont(fLabelFont);
      label->SetTextJustify(text_align);
      fMsgList->Add(label);
      fLabelFrame->AddFrame(label, fL3);
      line = nextLine + 1;
    }
    
    
    label = new TGLabel(fLabelFrame, line);
    label->SetTextColor(red);
    label->SetTextFont(fLabelFont);
    label->SetTextJustify(text_align);
    fMsgList->Add(label);
    fLabelFrame->AddFrame(label, fL3);
    delete [] tmpMsg;
    
    AddFrame(fLabelFrame, fL4);
    MapSubwindows();
    width  = GetDefaultWidth();
    height = GetDefaultHeight();
    Resize(width, height);
    AddInput(kKeyPressMask);
    CenterOnParent();
    SetWMSize(width, height);
    SetWMSizeHints(width, height, width, height, 0, 0);
    SetWindowName(title);
    MapRaised();
    gClient->WaitFor(this);
  }
  virtual ~TGMsgBoxMod(){
    delete fClose;
    delete fButtonFrame;
    delete fLabelFrame;
    fMsgList->Delete();
    delete fMsgList;
    delete fL1; delete fL2; delete fL3; delete fL4;
  }
  
  //void ChangeText() {
  //  ->SetText("gg");
  //}
  ClassDef(TGMsgBoxMod,0)  // A message dialog box
};
//ClassImp(TGMsgBoxMod);

void fitting(){
  gROOT->ProcessLine("GaussianFit()");
}

void GaussianFitLoop(){
  gROOT->ProcessLine(".L kobamac/root/GaussianFit.C");
  //TTimer *timer = new TTimer();
  //timer->Connect("Timeout()",0,0,"fitting()");
  //timer->Timeout();
  //timer->Start(1000,kFALSE);
  //TGMsgBoxModMod *msg = 

  new TGMsgBoxMod(gClient->GetRoot(),0, "Script is running!!", "GaussianFitLoop.C is now running!!\nDo you stop it?");
  
  
  
  //TGMsgBoxModMod aa(gClient->GetRoot(),0, "Script is running!!", "GaussianFitLoop.C is now running!!\nDo you stop it?", kMBIconAsterisk, kMBYes);
  //delete msg;
  //MyMainFrame *m = new MyMainFrame(gClient->GetRoot(), 0, 200, 200);
  //m->ChangeText();
  
  //delete timer;
}
