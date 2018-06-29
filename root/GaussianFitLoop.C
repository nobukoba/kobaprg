#include "TROOT.h"
//#include "RQ_OBJECT.h"
#include "TGClient.h"
#include "TGString.h"
#include "TGPicture.h"
#include "TGButton.h"
#include "TGLabel.h"
#include "TList.h"
#include "TCanvas.h"
#include "TTimer.h"
#include "TMath.h"
#include "TH1.h"
#include "TF1.h"

class TGMsgBoxMod {
  //RQ_OBJECT("TGMsgBoxMod")
  public:
  TGMsgBoxMod(const TGWindow *p, const TGWindow *main,
	      const char *title, const char *msg,
	      UInt_t options = kVerticalFrame,
	      Int_t text_align = kTextCenterX | kTextCenterY) {
    fMain = new TGTransientFrame(p, main, 10, 10, options);

    fMain->Connect("CloseWindow()", "TGMsgBoxMod", this, "CloseWindow()");
    fMain->DontCallClose(); // to avoid double deletions.
    
    // use hierarchical cleaning
    fMain->SetCleanup(kDeepCleanup);
    
    UInt_t width, height;
    width = 0;
    TGHorizontalFrame *fButtonFrame = new TGHorizontalFrame(fMain, 60, 20, kFixedWidth);
    TGLayoutHints *fL1 = new TGLayoutHints(kLHintsCenterY | kLHintsExpandX, 3, 3, 0, 0);
    TGButton *fClose = new TGTextButton(fButtonFrame, new TGHotString("C&lose"));
    fClose->Connect("Clicked()", "TGMsgBoxMod", this, "Stop()");
    fClose->Connect("Clicked()", "TGMsgBoxMod", this, "DoClose()");
    
    fButtonFrame->AddFrame(fClose, fL1);
    TGLayoutHints *fL2 = new TGLayoutHints(kLHintsBottom | kLHintsCenterX, 0, 0, 5, 5);
    fMain->AddFrame(fButtonFrame, fL2);
    TGVerticalFrame *fLabelFrame = new TGVerticalFrame(fMain, 60, 20);
    TGLayoutHints *fL3 = new TGLayoutHints(kLHintsCenterY | kLHintsLeft | kLHintsExpandX,
					   4, 2, 2, 2);
    TGLayoutHints *fL4 = new TGLayoutHints(kLHintsTop | kLHintsExpandX, 10, 10, 7, 2);
    // make one label per line of the message
    TGLabel *label;
    TGFont *fLabelFont = gClient->GetFont("-*-*-*-*-*-20-*-*-*-*-*-*-*", kFALSE);
    Pixel_t red;
    gClient->GetColorByName("red", red);

    label1 = new TGLabel(fLabelFrame, "FWHM:");
    label1->SetTextColor(red);
    label1->SetTextFont(fLabelFont);
    label1->SetTextJustify(text_align);
    fLabelFrame->AddFrame(label1, fL3);

    label2 = new TGLabel(fLabelFrame, "");
    label2->SetTextColor(red);
    label2->SetTextFont(fLabelFont);
    label2->SetTextJustify(text_align);
    fLabelFrame->AddFrame(label2, fL3);
    
    label3 = new TGLabel(fLabelFrame, "Integ. counts:");
    label3->SetTextJustify(text_align);
    fLabelFrame->AddFrame(label3, fL3);

    label4 = new TGLabel(fLabelFrame, msg);
    fLabelFrame->AddFrame(label4, fL3);
    fMain->AddFrame(fLabelFrame, fL4);
    fMain->MapSubwindows();
    width  = fMain->GetDefaultWidth();
    height = fMain->GetDefaultHeight();
    fMain->Resize(width, height);
    fMain->AddInput(kKeyPressMask);
    fMain->CenterOnParent();
    fMain->SetWMSize(width, height);
    fMain->SetWMSizeHints(width, height, width, height, 0, 0);
    fMain->SetWindowName(title);
    fMain->MapWindow();
  }
  virtual ~TGMsgBoxMod(){
    fMain->DeleteWindow();
  }
  void Stop(){
    delete mem_timer;
  }
  void Start(TTimer *timer){
    mem_timer = timer;
    gClient->WaitFor(fMain);
  }
  void fitting(){
    TH1 * hist = (TH1 *) gROOT->ProcessLine("GaussianFit();");
    TF1 * gaus = hist->GetFunction("gaus");
    if (label1){
      Double_t constant = gaus->GetParameter(0);
      Double_t mean     = gaus->GetParameter(1);
      Double_t sigma    = gaus->GetParameter(2);
      Double_t fwhm     = sigma * 2.355;
      Double_t integ_area   = constant*TMath::Sqrt(2*TMath::Pi());
      Double_t integ_counts = integ_area/(hist->GetXaxis()->GetBinWidth(1));
      Double_t kev_per_ch = 0.;
      kev_per_ch = 10.; // Modify here
      if(kev_per_ch > 0){
	label1->SetText("Energy Resolution:");
	label2->SetText(TString::Format("%.1f keV (FWHM)",fwhm*kev_per_ch));
      }else{
	label1->SetText(TString::Format("FWHM: %g",fwhm));
	label2->SetText("");
      }
      label3->SetText(TString::Format("Integ. counts:  %.5g",integ_counts));
    }
  }
  void DoClose() { CloseWindow(); }
  void CloseWindow() { delete this; }
  
private:
  TGTransientFrame    *fMain;
  TGLabel             *label1, *label2, *label3, *label4; // labels
  TTimer              *mem_timer;
  ClassDef(TGMsgBoxMod,0)  // A message dialog box
};
ClassImp(TGMsgBoxMod);

void GaussianFitLoop(){
  TCanvas* canvas = gPad->GetCanvas();
  TVirtualPad *sel_pad = canvas->GetPad(gPad->GetNumber());
  if (sel_pad == 0) {return;}
  TList * list = sel_pad->GetListOfPrimitives();
  if (list == 0) {return;}
  TH1 *hist = (TH1*) list->At(1);
  if (hist == 0) {return;}
  if (hist->InheritsFrom("TH2")) {
    printf("This script can not handle TH2 histograms.\n");
    return;
  }
  if (hist->InheritsFrom("TH1") == 0) {return;}
  
  gROOT->ProcessLine(".L GaussianFit.C");
  TGMsgBoxMod *msgb = new TGMsgBoxMod(gClient->GetRoot(),0, "Script is running!!", "GaussianFitLoop.C is now running!!\nDo you stop it?");
  TTimer *timer = new TTimer();
  timer->Connect("Timeout()", "TGMsgBoxMod", msgb, "fitting()");
  timer->Timeout();
  timer->Start(1000,kFALSE);
  msgb->Start(timer);
}
