#include <iostream>
#include <sstream>
#include "TROOT.h"
#include "TSystem.h"
#include "TFrame.h"
#include "TBox.h"
#include "TCanvas.h"
#include "TVirtualPad.h"
#include "TLine.h"
#include "TList.h"
#include "TMarker.h"
#include "TH2.h"

class WaitOneClickY{
public:
  ~WaitOneClickY(){
    canvas->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)",this);
  }
  WaitOneClickY(Double_t *x_in, Double_t *y_in):
    x(x_in), y(y_in),
    fCrosshairPos(0), pxlast(0), pylast(0){
    canvas = gPad->GetCanvas();
    canvas->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)",
		    "WaitOneClickY", this, "DrawFunction(Int_t,Int_t,Int_t,TObject*)");
    canvas->WaitPrimitive("TMarker");
  }
  void DrawFunction(Int_t event, Int_t x_in, Int_t y_in, TObject*){
    if (event == kButton1Down) {
      pxlast = gPad->GetEventX();
      pylast = gPad->GetEventY();
      *x = gPad->AbsPixeltoX(pxlast);
      *y = gPad->AbsPixeltoY(pylast);
      mk.PaintMarker(*x, *y);
      return;
    }
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
    //if(pxold) gVirtualX->DrawLine(pxold,pymin,pxold,pymax);
    if(pyold) gVirtualX->DrawLine(pxmin,pyold,pxmax,pyold);
    if (event == kButton1Down ||
	event == kButton1Up   ||
	event == kMouseLeave) {
      fCrosshairPos = 0;
      return;
    }
    //gVirtualX->DrawLine(px,pymin,px,pymax);
    gVirtualX->DrawLine(pxmin,py,pxmax,py);
    fCrosshairPos = px + 10000*py;
    return;
  }
private:
  Double_t *x;
  Double_t *y;
  Int_t fCrosshairPos;
  Int_t pxlast, pylast;
  TMarker mk;
  TCanvas *canvas;
};

void banx_gui(){
  if (!gPad) {
    std::cout << "There is no gPad. This script is terminated." << std::endl;
    return;
  }
  Double_t x0, y0;
  WaitOneClickY *primi = new WaitOneClickY(&x0, &y0); delete primi;

  TList* listofpri = gPad->GetListOfPrimitives();
  TH2* hist = 0;
  TIter next(listofpri); TObject *obj;
  while ((obj = next())){
    if (obj->InheritsFrom("TH2")) {
      hist = (TH2*)obj;
      break;
    }
  }
  if (!hist) {
    std::cout << "TH2 histogram was not found in this pad." << std::endl;
    return;
  }
  TLine line;
  Double_t xrange_min = hist->GetXaxis()->GetBinLowEdge(hist->GetXaxis()->GetFirst());
  Double_t xrange_max = hist->GetXaxis()->GetBinUpEdge(hist->GetXaxis()->GetLast());
  line.DrawLine(xrange_min,y0,xrange_max,y0);
  Double_t x1, y1;
  primi = new WaitOneClickY(&x1, &y1); delete primi;
  line.DrawLine(xrange_min,y1,xrange_max,y1);
  std::cout << std::endl << "Clicked Positions" << std::endl;
  std::cout << "1st (x, y) = (" << x0 << ", " << y0 << ")"<< std::endl;
  std::cout << "2nd (x, y) = (" << x1 << ", " << y1 << ")"<< std::endl;

  if (y0 > y1) {
    Double_t tmpy = y0;
    y0 = y1;
    y1 = tmpy;
  }

  gROOT->ProcessLine(".L ../cui/banx.C");
  gROOT->ProcessLine(Form("banx((TH1*)%p,%f,%f)",hist,y0,y1));
  return;
}
