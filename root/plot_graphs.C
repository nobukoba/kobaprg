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
  TList *listofpri = gPad->GetListOfPrimitives();
  TGraph gr;
  Int_t fCrosshairPos = 0;
  Int_t pxlast = 0, pylast = 0;
  Int_t event = 0;
  //gSystem->ProcessEvents();
  while (!gSystem->ProcessEvents() && gROOT->GetSelectedPad()) {
    event = gPad->GetEvent();
    if (number_of_points > 1) {
      if (gr.GetN() == number_of_points){
	break;
      }
    } else {
      if (event == kButton1Double || event == kKeyPress) {
	//the following statement is required against other loop executions
	//before returning
	//canvas->HandleInput((EEventType)-1,0,0);
	break;
      }
    }
    if (event == kButton1Down) {
      pxlast = gPad->GetEventX();
      pylast = gPad->GetEventY();
      canvas->HandleInput((EEventType)-1,0,0);
      Double_t x = gPad->AbsPixeltoX(pxlast);
      Double_t y = gPad->AbsPixeltoY(pylast);
      gr.SetPoint(gr.GetN(), x, y);
      gr.Draw("L*");
    }
    if ((gr.GetN()>=1)) {
      if (event == kMouseEnter) continue;
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
      if (event == kButton1Down ||
	  event == kButton1Up   ||
	  event == kMouseLeave) {
	fCrosshairPos = 0;
	continue;
      }
      if (px && py) gVirtualX->DrawLine(pxlast,pylast,px,py);
      fCrosshairPos = px + 10000*py;
    }
    gSystem->Sleep(10);
  }
  return (new TGraph(gr));
}

void plot_graphs(){
  if (!gPad) {
    std::cout << "There is no gPad." << std::endl;
    return;
  }
  TGraph *gr;
  while(gr = (TGraph*)gPad->GetListOfPrimitives()->FindObject("Graph")){
    gr->Delete();
  }
  gPad->SetCrosshair();
  TGraph * gr = MyWaitPrimitive(0);
  gPad->SetCrosshair(0);
  TList *listofpri = gPad->GetListOfPrimitives();
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
  gPad->Modified();
  gPad->Update();
  return;
}
