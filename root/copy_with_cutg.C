#include <iostream>
#include "TROOT.h"
#include "TSystem.h"
#include "TObject.h"
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
#include "TMarker.h"

void copy_with_cutg(){
  TCanvas* canvas = gPad->GetCanvas();
  if (canvas == 0) {
    std::cout << "There is no canvas." << std::endl;
    return;
  }

  TGraph *graphical_cut = new TGraph();
  TLine graphical_line;
  TLine *prevline = 0;
  graphical_cut->SetName("graphical_cut");
  gPad->SetCrosshair();
  Int_t event = 0;
  gSystem->ProcessEvents();
  Double_t cur_x, cur_y, past_x, past_y, pre_x, pre_y;
  
  while (!gSystem->ProcessEvents() && gROOT->GetSelectedPad()) {
    event = gPad->GetEvent();
    if (event == kButton1Double || event == kKeyPress) {
      //the following statement is required against other loop executions
      //before returning
      canvas->HandleInput((EEventType)-1,0,0);
      break;
    }
    if (event == kButton1Down) {
      cur_x = gPad->AbsPixeltoX(gROOT->GetSelectedPad()->GetEventX());
      cur_y = gPad->AbsPixeltoY(gROOT->GetSelectedPad()->GetEventY());
      pre_x = cur_x;
      pre_y = cur_y;
      graphical_cut->SetPoint(graphical_cut->GetN(), cur_x, cur_y);
      graphical_cut->Draw("L*");
      canvas->HandleInput((EEventType)-1,0,0);
    }
    if ((graphical_cut->GetN()>=1)&&(event == kMouseMotion)) {
      cur_x = gPad->AbsPixeltoX(gROOT->GetSelectedPad()->GetEventX());
      cur_y = gPad->AbsPixeltoY(gROOT->GetSelectedPad()->GetEventY());
      if ((cur_x != past_x) || (cur_y != past_y) ) {
    	if (prevline) {
    	  //gROOT->GetSelectedPad()->GetListOfPrimitives()->Remove(prevline);
	  prevline->Delete();
    	}
    	prevline = graphical_line.DrawLine(pre_x, pre_y, cur_x, cur_y);
	gROOT->GetSelectedPad()->Update();
    	past_x = cur_x;
    	past_y = cur_y;
      }
    }

    gSystem->Sleep(10);
  }
  gPad->SetCrosshair(0);
  if (prevline) {
    //gROOT->GetSelectedPad()->GetListOfPrimitives()->Remove(prevline);
    prevline->Delete();
  }

  TVirtualPad *sel_pad = gROOT->GetSelectedPad();
  if (!sel_pad) {
    std::cout << "There is no selected pad." << std::endl;
    return;
  }
  TList *listofpri = sel_pad->GetListOfPrimitives();
  if (listofpri == 0) {
    std::cout << "The sel_pad includes nothing." << std::endl;
    return;
  }
  TCutG *cutg = (TCutG*)listofpri->FindObject("CUTG");
  if (cutg != 0){
    cutg->Delete();
  }
  cutg = new TCutG("CUTG",graphical_cut->GetN(),
		   graphical_cut->GetX(),
		   graphical_cut->GetY());
  graphical_cut->Delete();

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
    return;
  }
  
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
  canvas->Update();
  return;
  
}
