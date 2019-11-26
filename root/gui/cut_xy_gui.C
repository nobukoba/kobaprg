#include <iostream>
#include <sstream>
#include "TROOT.h"
#include "TGInputDialog.h"
#include "TCanvas.h"
#include "TVirtualPad.h"
#include "TList.h"
#include "TH2.h"

void cut_xy_gui(){
  gPad->SetCrosshair();
  TCanvas* canvas;
  if (!(canvas = gPad->GetCanvas())) {
    std::cout << "There is no canvas." << std::endl;
    return;
  }
  TMarker *mk = (TMarker*)canvas->WaitPrimitive("TMarker","Marker");
  Double_t x0 = mk->GetX();
  Double_t y0 = mk->GetY();
  delete mk;
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
    gPad->SetCrosshair(0);
    return;
  }
  TLine line;
  Double_t xrange_min = hist->GetXaxis()->GetBinLowEdge(hist->GetXaxis()->GetFirst());
  Double_t xrange_max = hist->GetXaxis()->GetBinUpEdge(hist->GetXaxis()->GetLast());
  Double_t yrange_min = hist->GetYaxis()->GetBinLowEdge(hist->GetYaxis()->GetFirst());
  Double_t yrange_max = hist->GetYaxis()->GetBinUpEdge(hist->GetYaxis()->GetLast());
  line.DrawLine(xrange_min,y0,xrange_max,y0);
  line.DrawLine(x0,yrange_min,x0,yrange_max);
  mk = (TMarker*)canvas->WaitPrimitive("TMarker","Marker");
  gPad->SetCrosshair(0);
  Double_t x1 = mk->GetX();
  Double_t y1 = mk->GetY();
  delete mk;
  line.DrawLine(xrange_min,y1,xrange_max,y1);
  line.DrawLine(x1,yrange_min,x1,yrange_max);

  std::cout << std::endl << "Clicked Position" << std::endl;
  std::cout << "1st (x, y) = (" << x0 << ", " << y0 << ")"<< std::endl;
  std::cout << "2nd (x, y) = (" << x1 << ", " << y1 << ")"<< std::endl;

  if (x0 > x1) {
    Double_t tmpx = x0;
    x0 = x1;
    x1 = tmpx;
  } 
  if (y0 > y1) {
    Double_t tmpy = y0;
    y0 = y1;
    y1 = tmpy;
  }

  gROOT->ProcessLine(".L ../cui/cut_xy.C");
  gROOT->ProcessLine(Form("cut_xy((TH1*)%p,%f,%f,%f,%f)",hist, x0, x1, y0, y1));
  return;
}
