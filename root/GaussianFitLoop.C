#include "TROOT.h"
#include "TH1.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TList.h"
#include "TTimer.h"
#include "TLatex.h"
#include "TGLabel.h"
#include "TMath.h"
#include "TGMsgBox.h"
#include "TGFrame.h"
#include "TString.h"
#include "TGButton.h"
#include "TGObject.h"
#include "TGString.h"
#include "TGWidget.h"
 
class MyMainFrame : public TGTransientFrame {
public:
  MyMainFrame(const TGWindow *p,  const TGWindow *main, UInt_t w, UInt_t h) :
    TGTransientFrame(p, main, w, h) {
    // Create main frame
    fVal = new TGLabel(this, "aa\n");
    TGFont *fLabelFont = gClient->GetFont("-*-*-*-*-*-20-*-*-*-*-*-*-*", kFALSE);
    Pixel_t red;
    gClient->GetColorByName("red", red);
    fVal->SetTextColor(red);
    fVal->SetTextFont(fLabelFont);
    AddFrame(fVal, new TGLayoutHints(kLHintsTop | kLHintsLeft |
				     kLHintsExpandX | kLHintsExpandY, 
				     5, 5, 5, 5));
    fMsg = new TGLabel(this, "Now GaussianFitLoop.C is runing!!\nDo you stop it?");
    AddFrame(fMsg, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,
				     5, 5, 5, 5));
    hframe = new TGHorizontalFrame(this, 150, 20, kFixedWidth);
    show = new TGTextButton(hframe, new TGHotString("&Stop"));
    //show->Connect("Pressed()", "TGMainFrame", this, "DeleteWindow()");
    show->Associate(this);
    hframe->AddFrame(show, new TGLayoutHints(kLHintsExpandX, 5, 5, 3, 4));
    AddFrame(hframe, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
    SetWindowName("Script is runing!!");
    MapSubwindows();
    Resize(GetDefaultSize());
    //MapWindow();
    MapRaised();

    fClient->WaitFor(this);
  }
  ~MyMainFrame(){
    delete fVal;
    delete fMsg;
    delete hframe;
    delete show;
  };
  //CloseWindow(){ Cleanup();}
  void ChangeText() {
    fVal->SetText("gg");
  }

 void CloseWindow()
 {
    if (fRetCode) *fRetCode = kMBClose;
    DeleteWindow();
 }
 
 ////////////////////////////////////////////////////////////////////////////////
 /// Process message dialog box event.
 
 Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t)
 {
    switch (GET_MSG(msg)) {
       case kC_COMMAND:
          switch (GET_SUBMSG(msg)) {
             case kCM_BUTTON:
                if (fRetCode) *fRetCode = (Int_t) parm1;
                DeleteWindow();
                break;
 
             default:
                break;
          }
          break;
       default:
          break;
    }
    return kTRUE;
 }
 
private:
  TGLabel *fVal;
  TGLabel *fMsg;
  TGHorizontalFrame *hframe;
  TGTextButton *show;
  Int_t *fRetCode; // address to store return code
  ClassDef(MyMainFrame, 0)
};

class TGMsgBoxMod : public TGMsgBox {
public:
  TGMsgBoxMod(const TGWindow *p, const TGWindow *main,
	      const char *title, const char *msg, EMsgBoxIcon icon,
	      Int_t buttons = kMBDismiss) :
    TGMsgBox(p, main, title, msg, icon, buttons){
    printf("here\n");
  }
  ~TGMsgBoxMod() {
  }
private:
  ClassDef(TGMsgBoxMod,0)
};

void fitting(){
  gROOT->ProcessLine("GaussianFit()");
}

void GaussianFitLoop(){
  gROOT->ProcessLine(".L kobamac/root/GaussianFit.C");
  TTimer *timer = new TTimer();
  timer->Connect("Timeout()",0,0,"fitting()");
  timer->Timeout();
  timer->Start(1000,kFALSE);
  //TGMsgBoxMod *msg = 
  new TGMsgBoxMod(gClient->GetRoot(),0, "Script is running!!", "GaussianFitLoop.C is now running!!\nDo you stop it?", kMBIconAsterisk, kMBYes);
  //TGMsgBoxMod aa(gClient->GetRoot(),0, "Script is running!!", "GaussianFitLoop.C is now running!!\nDo you stop it?", kMBIconAsterisk, kMBYes);
  //delete msg;
  //MyMainFrame *m = new MyMainFrame(gClient->GetRoot(), 0, 200, 200);
  //m->ChangeText();

  delete timer;
}
