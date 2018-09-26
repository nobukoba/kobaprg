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
#include "TMarker.h"

TGraph * MyWaitPrimitive(Int_t number_of_points) {
  TCanvas* canvas = gPad->GetCanvas();
  if (!canvas) {
    std::cout << "There is no canvas." << std::endl;
    return 0;
  }
  TGraph *graphical_cut = new TGraph();
  TLine graphical_line;
  TLine *prevline = 0;
  graphical_cut->SetName("graphical_cut");
  gPad->SetCrosshair();
  Int_t event = 0;
  gSystem->ProcessEvents();
  Double_t cur_x  = 0, cur_y  = 0;
  Double_t past_x = 0, past_y = 0;
  Double_t pre_x  = 0, pre_y  = 0;
  while (!gSystem->ProcessEvents() && gROOT->GetSelectedPad()) {
    event = gPad->GetEvent();
    if (number_of_points > 1) {
      if (graphical_cut->GetN() == number_of_points){
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
  return graphical_cut;
}

void plot_graphs(){
  TCanvas* canvas = gPad->GetCanvas();
  if (canvas == 0) {
    std::cout << "There is no canvas." << std::endl;
    return;
  }
  TGraph *grtmp;
  while(grtmp = (TGraph*)gPad->GetListOfPrimitives()->FindObject("Graph")){
    grtmp->Delete();
  }
  grtmp = MyWaitPrimitive(0);
  TGraph * gr = new TGraph(grtmp->GetN(),grtmp->GetX(),grtmp->GetY());
  grtmp->Delete();
  
  TVirtualPad *sel_pad = gROOT->GetSelectedPad();
  if (sel_pad == 0) {
    std::cout << "There is no sel_pad." << std::endl;
    gr->Delete();
    return;
  }
  TList *listofpri = sel_pad->GetListOfPrimitives();
  TIter next(listofpri);
  TObject *obj;
  TH1 *hist = 0;
  while (obj = next()){
    if (obj->InheritsFrom("TH1")) {
      hist = (TH1*)obj;
      std::cout << "hist was found." << std::endl;
      break;
    }
  }
  if(hist == 0){
    std::cout << "Histogram was not found in this pad." << std::endl;
    gr->Delete();
    return;
  }
  Int_t j = 0;
  while(listofpri->FindObject(Form("Graph_%d",j))){
    j++;
  }
  gr->SetName(Form("Graph_%d",j));
  gr->Draw("L*");
  sel_pad->Modified();
  sel_pad->Update();
  return;
}
