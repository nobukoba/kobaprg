#include <iostream>
#include "TROOT.h"
#include "TSystem.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TList.h"
#include "TLine.h"
#include "TCutG.h"
#include "TString.h"

TGraph * MyWaitPrimitive(Int_t number_of_points) {
  if (!gPad) {
    std::cout << "There is no gPad." << std::endl;
    return 0;
  }
  TCanvas* canvas = gPad->GetCanvas();
  TVirtualPad *sel_pad = gROOT->GetSelectedPad();
  TList *listofpri = sel_pad->GetListOfPrimitives();
  TGraph *graphical_cut = new TGraph();
  Int_t fCrosshairPos = 0;
  Int_t pxlast = 0, pylast = 0;
  Int_t event = 0;
  Int_t k = 0;
  gSystem->ProcessEvents();
  while (!gSystem->ProcessEvents() && gROOT->GetSelectedPad()) {
    event = gPad->GetEvent();
    if (number_of_points > 1) {
      if (graphical_cut->GetN() == number_of_points){
	canvas->HandleInput((EEventType)-1,0,0);
	break;
      }
    } else {
      if (event == kButton1Double || event == kKeyPress) {
	//the following statement is required against other loop executions
	//before returning
	canvas->HandleInput((EEventType)-1,0,0);
	break;
      }
    }
    if (event == kButton1Down) {
      pxlast = gPad->GetEventX();
      pylast = gPad->GetEventY();
      canvas->HandleInput((EEventType)-1,0,0);
      Double_t x = gPad->AbsPixeltoX(pxlast);
      Double_t y = gPad->AbsPixeltoY(pylast);
      graphical_cut->SetPoint(graphical_cut->GetN(), x, y);
      graphical_cut->Draw("L*");
    }
    if ((graphical_cut->GetN()>=1)) {
      if (gPad->GetEvent() == kMouseEnter) continue;
      canvas->FeedbackMode(kTRUE);
      //erase old position and draw a line at current position
      Int_t pxmin,pxmax,pymin,pymax,pxold,pyold,px,py;
      pxold = fCrosshairPos%10000;
      pyold = fCrosshairPos/10000;
      px    = gPad->GetEventX();
      py    = gPad->GetEventY()+1;
      pxmin = 0;
      pxmax = canvas->GetWw();
      pymin = 0;
      pymax = gPad->GetWh();
      if (pxold && pyold) gVirtualX->DrawLine(pxlast,pylast,pxold,pyold);
      if (gPad->GetEvent() == kButton1Down ||
	  gPad->GetEvent() == kButton1Up   ||
	  gPad->GetEvent() == kMouseLeave) {
	fCrosshairPos = 0;
	continue;
      }
      if (px && py) gVirtualX->DrawLine(pxlast,pylast,px,py);
      fCrosshairPos = px + 10000*py;
    }
    gSystem->Sleep(10);
  }
  return graphical_cut;
}

void copy_with_cutg(){
  TCanvas* canvas = gPad->GetCanvas();
  if (!canvas) {
    std::cout << "There is no canvas." << std::endl;
    return;
  }

  gPad->SetCrosshair();
  TGraph *graphical_cut = (TGraph *)MyWaitPrimitive(0);
  gPad->SetCrosshair(0);
  if (!graphical_cut){
    std::cout << "some problem occured. Exit." << std::end;
    return;
  }
  TVirtualPad *sel_pad = gROOT->GetSelectedPad();
  TList *listofpri = sel_pad->GetListOfPrimitives();
  TIter next(listofpri);
  TObject *obj;
  TH2 *hist = 0;
  
  while (obj = next()){
    if (obj->InheritsFrom("TH2")) {
      hist = (TH2*)obj;
      std::cout << "TH2 hist was found." << std::endl;
      break;
    }
  }
  if(hist == 0){
    std::cout << "TH2 histogram was not found in this pad." << std::endl;
    graphical_cut->Delete();
    sel_pad->Update();
    return;
  }
  TCutG *cutg = (TCutG*)listofpri->FindObject("CUTG");
  if (cutg != 0){
    cutg->Delete();
  }
  cutg = new TCutG("CUTG",graphical_cut->GetN(),
		   graphical_cut->GetX(),
		   graphical_cut->GetY());
  //graphical_cut->Delete();
  
  gROOT->cd();
  TString str = hist->GetName();
  str += "_cut";
  TString str_n = str;
  Int_t num = 1;
  while (gROOT->Get(str_n.Data())) {
    str_n = Form("%s%d",str.Data(),num);
    num++;
  }
  TH2 *hout = (TH2*)hist->Clone(str_n);
  hout->SetTitle(hist->GetTitle());
  //hout->Reset();
  Double_t xx, yy;
  for (Int_t i = 0; i <= hist->GetNbinsX()+1; i++) {
    for (Int_t j = 0; j <= hist->GetNbinsY()+1; j++) {
      xx = hist->GetXaxis()->GetBinCenter(i);
      yy = hist->GetYaxis()->GetBinCenter(j);
      if (cutg->IsInside(xx,yy) != 1) {
	hout->SetBinContent(i,j,0);
      }
    }
  }
  hout->Draw("colz");
  gPad->GetFrame()->SetBit(TBox::kCannotMove);
  sel_pad->Update();
  return;
  
}
