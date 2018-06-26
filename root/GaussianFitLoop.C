#include "TROOT.h"
#include "TH1.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TList.h"
#include "TTimer.h"
#include "TLatex.h"
#include "TMath.h"
#include "TGMsgBox.h"
class MyMainFrame : public TGMainFrame {
public:
  MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h) :
    TGMainFrame(p, w, h) {
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
    TGHorizontalFrame *hframe = new TGHorizontalFrame(this, 150, 20, kFixedWidth);
    TGTextButton *show = new TGTextButton(hframe, "&Stop");
    show->Connect("Pressed()", "TGMainFrame", this, "CloseWindow()");
    hframe->AddFrame(show, new TGLayoutHints(kLHintsExpandX, 5, 5, 3, 4));
    AddFrame(hframe, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
    SetWindowName("Script is runing!!");
    MapSubwindows();
    Resize(GetDefaultSize());
    MapWindow();
    gClient->WaitFor(this);
  }
  ~MyMainFrame() { Cleanup();}
  void ChangeText() {
    fVal->SetText("gg");
  }
private:
  TGLabel *fVal;
  TGLabel *fMsg;
  ClassDef(MyMainFrame, 0)
};

class TGMsgBoxMod : public TGMsgBox {
public:
  TGMsgBoxMod(const TGWindow *p, const TGWindow *main,
	      const char *title, const char *msg, EMsgBoxIcon icon,
	      Int_t buttons = kMBDismiss, Int_t *ret_code = 0,
	      UInt_t options = kVerticalFrame,
	      Int_t text_align = kTextCenterX | kTextCenterY) :
    TGMsgBox(p, main, title, msg, icon, buttons, ret_code, options, text_align){
    printf("here\n");
  }
  ~TGMsgBoxMod() {
    delete msgbox;
  }
private:
  ClassDef(TGMsgBoxMod,0)
};

void fitting(){
  gROOT->ProcessLine("GaussianFit()");
}

void GaussianFitLoop(){
  gROOT->ProcessLine(".L GaussianFit.C");
  TTimer *timer = new TTimer();
  timer->Connect("Timeout()",0,0,"fitting()");
  timer->Timeout();
  timer->Start(1000,kFALSE);
  //TGMsgBoxMod *msg = 
  //new TGMsgBoxMod(gClient->GetRoot(),0, "Script is running!!", "GaussianFitLoop.C is now running!!\nDo you stop it?", kMBIconAsterisk, kMBYes);
  //TGMsgBoxMod aa(gClient->GetRoot(),0, "Script is running!!", "GaussianFitLoop.C is now running!!\nDo you stop it?", kMBIconAsterisk, kMBYes);
  //delete msg;
  MyMainFrame *m = new MyMainFrame(gClient->GetRoot(), 200, 200);
  m->ChangeText();

  delete timer;
}
