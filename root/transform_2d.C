#include <iostream>
#include <sstream>
#include <string>
#include <TROOT.h>
#include <TClass.h>
#include <TApplication.h>
#include <TGClient.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGTextEdit.h>
#include <TGFrame.h>
#include <TF2.h>
#include <TMath.h>

Double_t func_xy(Double_t *xy, Double_t *par){
  Int_t npar = par[0];
  Double_t x = xy[0];
  Double_t y = xy[1];
  Double_t ret = 0.;
  for (Int_t i = 0; i < npar/3; i++) {
    ret += par[i*3+3]
      * TMath::Power(x,par[i*3+1])
      * TMath::Power(y,par[i*3+2]);
  }
  return ret;
}

class Editor {
private:
  TGTransientFrame *fMain;   // main frame of this widget
  TGTextEdit *fEdit;
  TF2 *f_xy, *g_xy;
public:
  Editor(const TGWindow *main, UInt_t w, UInt_t h) :
    f_xy(0), g_xy(0){
    fMain = new TGTransientFrame(gClient->GetRoot(), main, w, h);
    fMain->Connect("CloseWindow()", "Editor", this, "CloseWindow()");
    fMain->DontCallClose(); // to avoid double deletions.
    fMain->SetCleanup(kDeepCleanup); // use hierarchical cleaning
    fEdit = new TGTextEdit(fMain, w, h, kSunkenFrame | kDoubleBorder);
    TGLayoutHints *fL1 = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 3, 3, 3, 3);
    fMain->AddFrame(fEdit,fL1);
    TGHorizontalFrame *fHframe = new TGHorizontalFrame(fMain, 0, 0, 0);
    TGTextButton *fTransform  = new TGTextButton(fHframe, "&Transform");
    fTransform->Connect("Clicked()", "Editor", this, "DoTransform()");
    TGTextButton *fClose      = new TGTextButton(fHframe, "  &Close  ");
    fClose->Connect("Clicked()", "Editor", this, "DoClose()");
    fHframe->AddFrame(fTransform, fL1);
    fHframe->AddFrame(fClose, fL1);
    TGLayoutHints *fL2 = new TGLayoutHints(kLHintsBottom | kLHintsCenterX, 0, 0, 5, 5);
    fMain->AddFrame(fHframe, fL2);
    fMain->SetWindowName("f(x,y) g(x,y)");
    fMain->SetIconName("f(x,y) g(x,y)");
    fMain->MapSubwindows();
    fMain->Resize();
    fMain->CenterOnParent(kTRUE, TGTransientFrame::kCenter);
    InitBuffer();
    fMain->MapWindow();
    gClient->WaitFor(fMain);
  }
  virtual ~Editor(){
    fMain->DeleteWindow();  // deletes fMain
  }
  void CloseWindow(){
    //if(f_xy){delete f_xy;}
    //if(g_xy){delete g_xy;}
    delete this;
  }
  void InitBuffer() {
    const char *initext =
      "0 0 2 1.0\n"
      "1 0 0 1.0\n";
      fEdit->LoadBuffer(initext);
  }
  void DoTransform(){
    std::cout << "DoTransform" << std::endl;
    TGText* fText = fEdit->GetText();
    char   *buf1, *buf2;
    Long_t  len;
    ULong_t i = 0;
    TGLongPosition pos;
    pos.fX = pos.fY = 0;
    Long_t rowcount = fText->RowCount();
    Double_t *f_xy_par = new Double_t[rowcount*3+1];
    Double_t *g_xy_par = new Double_t[rowcount*3+1];
    Int_t i_f = 1;
    Int_t i_g = 1;
    while (pos.fY < rowcount) {
      len = fText->GetLineLength(pos.fY);
      if (len < 0) len = 0;
      buf1 = fText->GetLine(pos, len);
      buf2 = new char[len + 2];
      if (buf1 != 0) {
	strncpy(buf2, buf1, (UInt_t)len);
      }
      if (len == 0) {
	if (pos.fY == (rowcount-1)){
	  rowcount--;
	}
      }
      //buf2[len] = '\0';
      buf2[len]   = '\n';
      buf2[len+1] = '\0';
      while (buf2[i] != '\0') {
      	if (buf2[i] == '\t') {
      	  ULong_t j = i+1;
      	  while (buf2[j] == 16)
      	    j++;
      	  // coverity[secure_coding]
      	  strcpy(buf2+i+1, buf2+j);
      	}
      	i++;
      }
      //fwrite(buf2, sizeof(char), strlen(buf2)+1, p);
      //printf("%s",buf2);
      TString str = buf2;
      str.ReplaceAll("\t"," ");
      str.ReplaceAll(","," ");
      std::stringstream ss(str.Data());
      Int_t fg;
      Double_t xo, yo, val;
      if(ss >> fg >> xo >> yo >> val) {
	std::cout
	  << fg << ", "
	  << xo << ", "
	  << yo << ", "
	  << val
	  << std::endl;
	if (fg == 0) {
	  f_xy_par[i_f++] = xo;
	  f_xy_par[i_f++] = yo;
	  f_xy_par[i_f++] = val;
	}
	if (fg == 1) {
	  g_xy_par[i_g++] = xo;
	  g_xy_par[i_g++] = yo;
	  g_xy_par[i_g++] = val;
	}
      }
      delete [] buf1;
      delete [] buf2;
      pos.fY++;
    }
    if (f_xy != 0) {
      delete f_xy;
    } 
    if (g_xy != 0) {
      delete g_xy;
    } 
    f_xy = new TF2("f_xy",func_xy,-1,1,-1,1,i_f);
    g_xy = new TF2("g_xy",func_xy,-1,1,-1,1,i_g);
    f_xy_par[0] = i_f;
    g_xy_par[0] = i_g;
    f_xy->SetParameters(f_xy_par);
    g_xy->SetParameters(g_xy_par);
    f_xy->Draw("surf");
    TCanvas* canvas = gPad->GetCanvas();
    canvas->Modified();
    canvas->Update();
    delete [] f_xy_par;
    delete [] g_xy_par;
  }
  void DoClose(){ CloseWindow(); }
};

void transform_2d(){
  TCanvas* canvas = gPad->GetCanvas();
  TVirtualPad *sel_pad = canvas->GetPad(gPad->GetNumber());
  if (sel_pad == 0) {return;}
  TList * list = sel_pad->GetListOfPrimitives();
  if (list == 0) {return;}
  TH1 *hist = (TH1*) list->At(1);
  if (hist == 0) {return;}
  if (hist->InheritsFrom("TH2") == 0) {
    printf("This script can handle only TH2 histograms.\n");
    return;
  }
  
  Editor *ed = new Editor(gClient->GetRoot(), 600, 400);
  
  
}
