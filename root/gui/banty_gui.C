#include <iostream>
#include <sstream>
#include "TROOT.h"
#include "TFrame.h"
#include "TBox.h"
#include "TGInputDialog.h"
#include "TCanvas.h"
#include "TVirtualPad.h"
#include "TList.h"
#include "TH2.h"

void banty_gui(){
  if (!gPad) {
    std::cout << "There is no gPad. This script is terminated." << std::endl;
    return;
  }
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx == 0) {return;}
  TString str = gBrowserEx->OpenTGInputDialog("y=ax+b: a(%f)",1);
  if (str.EqualTo("")){return;}
  
  std::istringstream iss(str.Data());
  Double_t par0;
  iss >> par0;

  Double_t x0, y0;
  WaitOneClickT *primi = new WaitOneClickT(&x0, &y0, par0); delete primi;
  
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
  Double_t yrange_min = hist->GetYaxis()->GetBinLowEdge(hist->GetYaxis()->GetFirst());
  Double_t yrange_max = hist->GetYaxis()->GetBinUpEdge(hist->GetYaxis()->GetLast());
  Double_t b0 = y0 - par0*x0;
  Int_t y0_1   = par0*yrange_min+b0;
  Int_t y0_2   = par0*yrange_max+b0;
  if (y0_1 < yrange_min) {y0_1 = yrange_min;}
  if (y0_1 > yrange_max) {y0_1 = yrange_max;}
  if (y0_2 < yrange_min) {y0_2 = yrange_min;}
  if (y0_2 > yrange_max) {y0_2 = yrange_max;}
  Double_t x0_1 = xrange_min;
  Double_t x0_2 = xrange_max;
  if (par0 != 0) {
    x0_1 = (y0_1-b0)/par0;
    x0_2 = (y0_2-b0)/par0;
  }
  line.DrawLine(x0_1,y0_1,x0_2,y0_2);
  Double_t x1, y1;
  primi = new WaitOneClickT(&x1, &y1, par0);
  delete primi;
  Double_t b1 = y1 - par0*x1;
  Int_t y1_1   = par0*yrange_min+b1;
  Int_t y1_2   = par0*yrange_max+b1;
  if (y1_1 < yrange_min) {y1_1 = yrange_min;}
  if (y1_1 > yrange_max) {y1_1 = yrange_max;}
  if (y1_2 < yrange_min) {y1_2 = yrange_min;}
  if (y1_2 > yrange_max) {y1_2 = yrange_max;}
  Double_t x1_1 = xrange_min;
  Double_t x1_2 = xrange_max;
  if (par0 != 0) {
    x1_1 = (y1_1-b1)/par0;
    x1_2 = (y1_2-b1)/par0;
  }
  std::cout << std::endl << "Clicked Positions" << std::endl;
  std::cout << "1st (x, y) = (" << x0 << ", " << y0 << ")"<< std::endl;
  std::cout << "2nd (x, y) = (" << x1 << ", " << y1 << ")"<< std::endl;

  if (b0 > b1) {
    Double_t tmpy = b0;
    b0 = b1;
    b1 = tmpy;
  }
  
  gROOT->ProcessLine(".L ../cui/banty.C");
  gROOT->ProcessLine(Form("banty((TH1*)%p,%f,%f,%f)",hist,par0,b0,b1));
  return;
}
