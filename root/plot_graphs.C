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

class MyWaitPrimitive{
public:
  ~MyWaitPrimitive(){
    canvas->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)",this);
  }
  MyWaitPrimitive(Int_t nop, TGraph *gr_ptr_in):
    number_of_points(nop), gr_ptr(gr_ptr_in),
    fCrosshairPos(0), pxlast(0), pylast(0){
    canvas = gPad->GetCanvas();
    canvas->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)",
		    "MyWaitPrimitive", this, "DrawFunction(Int_t,Int_t,Int_t,TObject*)");
    canvas->WaitPrimitive("MyPrimitive");
  }
  void DrawFunction(Int_t event, Int_t x_in, Int_t y_in, TObject*){
    if (number_of_points > 0) {
      if (gr.GetN() == number_of_points){
 	for (Int_t i = 0; i < gr.GetN(); i++){
	  gr_ptr->SetPoint(i, gr.GetX()[i], gr.GetY()[i]);
	}
	gr_ptr->SetName("MyPrimitive");
	gr_ptr->Draw("L*");
	return;
      }
    } else {
      if (event == kButton1Double || event == kKeyPress) {
 	for (Int_t i = 0; i < gr.GetN(); i++){
	  gr_ptr->SetPoint(i, gr.GetX()[i], gr.GetY()[i]);
	}
	gr_ptr->SetName("MyPrimitive");
	gr_ptr->Draw("L*");
	return;
      }
    }
    if (event == kButton1Down) {
      pxlast = gPad->GetEventX();
      pylast = gPad->GetEventY();
      Double_t x = gPad->AbsPixeltoX(pxlast);
      Double_t y = gPad->AbsPixeltoY(pylast);
      gr.SetPoint(gr.GetN(), x, y);
      gr.Draw("L*");
    }
    if ((gr.GetN()>=1)) {
      if (event == kMouseEnter) return;
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
	return;
      }
      if (px && py) gVirtualX->DrawLine(pxlast,pylast,px,py);
      fCrosshairPos = px + 10000*py;
    }
    return;
  }
private:
  Int_t number_of_points;
  Int_t fCrosshairPos;
  Int_t pxlast, pylast;
  TCanvas *canvas;
  TGraph gr;
  TGraph *gr_ptr;
};

void plot_graphs(){
  if (!gPad) {
    std::cout << "There is no gPad." << std::endl;
    return;
  }
  TGraph *gr_prev;
  while(gr_prev = (TGraph*)gPad->GetListOfPrimitives()->FindObject("Graph")){
    gr_prev->Delete();
  }
  gPad->SetCrosshair();
  TGraph * gr = new TGraph();
  MyWaitPrimitive primi(0, gr);
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
