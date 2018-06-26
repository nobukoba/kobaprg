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
#include "TTimer.h"
#include "TMath.h"
#include "TH1.h"
#include "TF1.h"

class TGMsgBoxMod : public TGTransientFrame {
public:
  TGMsgBoxMod(const TGWindow *p_in, const TGWindow *main_in,
	      const char *title_in, const char *msg_in,
	      UInt_t options_in = kVerticalFrame,
	      Int_t text_align_in = kTextCenterX | kTextCenterY) :
    TGTransientFrame(p_in, main_in, 10, 10, options_in),
    p(p_in), main(main_in), title(title_in),msg(msg_in),
    options(options_in),text_align(text_align_in) {
    label1 = 0;
    label2 = 0;
    label3 = 0;
    label4 = 0;
  }
  virtual ~TGMsgBoxMod(){
    delete fClose;
    delete fButtonFrame;
    delete fLabelFrame;
    delete label1; delete label2; delete label3; delete label4;
    delete fL1; delete fL2; delete fL3; delete fL4;
  }
  void Start(){
    UInt_t width, height;
    fClose = 0;
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
    
    label3 = new TGLabel(fLabelFrame, "Integ. cnt:");
    label3->SetTextJustify(text_align);
    fLabelFrame->AddFrame(label3, fL3);

    label4 = new TGLabel(fLabelFrame, msg);
    fLabelFrame->AddFrame(label4, fL3);
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
      label3->SetText(TString::Format("Integ. cnt:  %.2g",integ_counts));
    }
  }
  
private:
  TGButton            *fClose;   // buttons in dialog box
  TGHorizontalFrame   *fButtonFrame;                // frame containing buttons
  TGVerticalFrame     *fLabelFrame;                 // frame containing text
  TGLayoutHints       *fL1, *fL2, *fL3, *fL4; // layout hints
  TGLabel             *label1, *label2, *label3, *label4; // labels
  const TGWindow *p; 
  const TGWindow *main;
  const char *title;
  const char *msg;
  UInt_t options;
  Int_t text_align;
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
  msgb->Start();
  delete timer;
}
