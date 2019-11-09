#if defined(__CINT__) && !defined(__MAKECINT__)
{
  TBrowserEx *gBrowserEx_tmp = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx_tmp) {
    gSystem->cd((gBrowserEx_tmp->GetInitialWorkingDir()).Data());
  }else{return;}
  gSystem->CompileMacro("./kobaprg/root/graph/text_to_graph.C","k");
  text_to_graph();
}
#else

#include <iostream>
#include <sstream>
#include <string>
#include <TROOT.h>
#include <TClass.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TGClient.h>
#include <TGLabel.h>
#include <TGListTree.h>
#include <TGButton.h>
#include <TGTextEdit.h>
#include <TGFrame.h>
#include <TString.h>
#include <TKey.h>
#include <TH2.h>
#include <TF2.h>
#include <TGraphErrors.h>
#include <TMath.h>

class Editor {
private:
  TGTransientFrame *fMain;   // main frame of this widget
  TGTextEdit *fEdit;
public:
  Editor(const TGWindow *main, UInt_t w, UInt_t h){
    fMain = new TGTransientFrame(gClient->GetRoot(), main, w, h);
    fMain->Connect("CloseWindow()", "Editor", this, "CloseWindow()");
    fMain->DontCallClose(); // to avoid double deletions.
    fMain->SetCleanup(kDeepCleanup); // use hierarchical cleaning
    fEdit = new TGTextEdit(fMain, w, h, kSunkenFrame | kDoubleBorder);
    TGLayoutHints *fL1 = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 3, 3, 3, 3);
    fMain->AddFrame(fEdit,fL1);
    TGHorizontalFrame *fHframe = new TGHorizontalFrame(fMain, 0, 0, 0);
    TGTextButton *fDraw       = new TGTextButton(fHframe, "  &Make   ");
    fDraw->Connect("Clicked()", "Editor", this, "MakeGraph()");
    TGTextButton *fClose      = new TGTextButton(fHframe, "  &Close  ");
    fClose->Connect("Clicked()", "Editor", this, "DoClose()");
    fHframe->AddFrame(fDraw, fL1);
    fHframe->AddFrame(fClose, fL1);
    TGLayoutHints *fL2 = new TGLayoutHints(kLHintsBottom | kLHintsCenterX, 0, 0, 5, 5);
    fMain->AddFrame(fHframe, fL2);
    fMain->SetWindowName("Graph x y data");
    fMain->SetIconName("Graph x y data");
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
    delete this;
  }
  void InitBuffer() {
    const char *initext =
      "1.0 1.0\n"
      "2.0 3.0\n";
      fEdit->LoadBuffer(initext);
  }
  void MakeGraph(){
    std::cout << "MakeGraph() starts." << std::endl;
    TGText* fText = fEdit->GetText();
    char   *buf1, *buf2;
    Long_t  len;
    ULong_t i = 0;
    TGLongPosition pos;
    pos.fX = pos.fY = 0;
    Long_t rowcount = fText->RowCount();
    Int_t ndata = 0;
    TList *grlist = (TList *)gROOT->GetList()->FindObject("grlist");
    if (!grlist) {
      // I need a directory for graphs. See http://root.cern.ch/root/roottalk/roottalk02/0672.html
      grlist = new TList();
      grlist->SetName("grlist");
      gROOT->Add(grlist);
    }
    TGraphErrors *gr = new TGraphErrors();
    TString str_n = "gr1";
    Int_t num = 2;
    while (grlist->FindObject(str_n.Data())) {
      str_n = Form("gr%d",num);
      num++;
    }
    gr->SetName(str_n.Data());
    gr->SetTitle(Form("data_graph_%d",num-1));
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(1.0);
    grlist->Add(gr);

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
      TString str = buf2;
      str.ReplaceAll("\t"," ");
      str.ReplaceAll(","," ");
      std::stringstream ss(str.Data());
      Double_t val[4];
      if (pos.fY == 0) {
	Int_t m;
	for (m = 0; m < 4; m++){
	  if (!(ss >> val[m])){
	    break;
	  }
	}
	ndata = m;
	if (ndata < 2) {
	  std::cout << "Num. of data is less than 2. ndata: " << ndata << std::endl;
	  std::cout << "This script is terminated." << std::endl;
	  DoClose();
	}else{
	  std::cout << "Num. of data in the 1st line: " << ndata << std::endl;
	}
      }else{
	Int_t m;
	for (m = 0; m < 4; m++){
	  if (!(ss >> val[m])){
	    break;
	  }
	}
	if ((pos.fY != (rowcount)) && (ndata != m)) {
	  std::cout << "ndata != num. of this line: " << ndata  << " != " << m << std::endl;
	  std::cout << "This script is terminated." << std::endl;
	  DoClose();
	}
      }
      if (ndata == 2) {
	gr->SetPoint(gr->GetN(), val[0],  val[1]);
      }else if (ndata == 3) {
	Int_t ngr = gr->GetN();
	gr->SetPoint(ngr, val[0],  val[1]);
	gr->SetPointError(ngr, 0., val[2]);
      }else{
	Int_t ngr = gr->GetN();
	gr->SetPoint(ngr, val[0],  val[1]);
	gr->SetPointError(ngr, val[2], val[3]);
      }

      delete [] buf1;
      delete [] buf2;
      pos.fY++;
    }
  }
  void DoClose(){ CloseWindow(); }
};

void text_to_graph(){
  //TGMainFrame * fMain = (TGMainFrame *) gROOT->ProcessLine("gBrowserEx->GetBrowserImp()->GetMainFrame();");
  //Editor *ed = new Editor(fMain, 600, 400);
  Editor *ed = new Editor(gClient->GetRoot(), 400, 600);
}

#endif
