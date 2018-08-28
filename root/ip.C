#include <iostream>
#include <iomanip>
#include "TROOT.h"
#include "TDirectory.h"
#include "TObject.h"
#include "TH2D.h"
#include "TF1.h"
#include "TGraph.h"
#include "TPad.h"
#include "TString.h"

extern void ip(const TH1*& h, const Double_t x0,  const Double_t x1, const Double_t x2, const Double_t x3);
extern void ip(const TH1*& h, const Double_t *xarr);
extern void ip();

void ip(const TH1*& h, const Double_t x0,  const Double_t x1, const Double_t x2, const Double_t x3) {
  Double_t xarr[] = {x0, x1, x2, x3};
  ip(h, xarr);
  return;
}

void ip() {
  TCanvas* canvas = gPad->GetCanvas();
  TVirtualPad *sel_pad = canvas->GetPad(gPad->GetNumber());
  if (sel_pad == 0) {
    std::cout << "There is no sel_pad." << std::endl;
    return 0;
  }
  TList *listofpri = sel_pad->GetListOfPrimitives();
  if (listofpri == 0) {
    std::cout << "The pad includes nothing." << std::endl;
    return 0;
  }
  TIter next(listofpri);
  TObject *obj;
  TH1 *hist = 0;
  while (obj = next()){
    if (obj->InheritsFrom("TH2")) {
      std::cout << "This script can not handle TH2 histograms." << std::endl;
      return 0;
    }
    if (obj->InheritsFrom("TH1")) {
      hist = (TH1*)obj;
      break;
    }
  }
  if(hist == 0){
    std::cout << "TH1 histogram was not found in this pad." << std::endl;
    return 0;
  }
  ip(hist);
}

void ip(const TH1*& h) {
  gPad->SetCrosshair();
  TGraph *grng;
  while (grng = (TGraph*)gPad->WaitPrimitive("Graph","PolyLine")) {
    Int_t np = grng->GetN();
    if (np == 4) {
      break;
    } else {
      std::cout << "Number of points: " << np << std::endl;
      std::cout << "4 points are necessary." << std::endl;
      grng->Delete();
    }
  }
  ip(h, grng->GetX());
  grng->Delete();
  gPad->SetCrosshair(0);
  return;
}

void ip(const TH1*& h, const Double_t *xarr) {
  Int_t npeaks = 0;
  while (gROOT->FindObject(Form("grng_%d",npeaks))) {
    npeaks++;
  }
  TGraph *gfit = new TGraph();
  TGraph *ghst = new TGraph();
  gfit->SetName(Form("gfit_%d",npeaks));
  ghst->SetName(Form("ghst_%d",npeaks));
  Int_t k = 0;
  Int_t l = 0;
  for (Int_t i = 1; i <= h->GetNbinsX(); i++) {
    Double_t xx = h->GetXaxis()->GetBinCenter(i);
    if (((xx >= xarr[0])&&(xx <= xarr[1]))
	||((xx >= xarr[2])&&(xx <= xarr[3]))) {
      gfit->SetPoint(k++,xx,h->GetBinContent(i));
    }
    if ((xx >= xarr[1])&&(xx <= xarr[2])) {
      ghst->SetPoint(l++,xx,h->GetBinContent(i));
    }
  }
  TF1 *tf1 = new TF1(Form("tf1_%d",npeaks),"pol1",h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
  gfit->Fit(tf1);
  tf1->SetRange(xarr[0],xarr[3]);  
  tf1->Draw("same");
  Double_t integ = 0;
  for (Int_t i = 0; i < ghst->GetN(); i++) {
    Double_t xx,yy;
    ghst->GetPoint(i,xx,yy);
    Double_t sub = yy - tf1->Eval(xx);
    integ += sub;
    ghst->SetPoint(i,xx,sub);
  }
  TGraph *grng = new TGraph();
  grng->SetName(Form("grng_%d",npeaks));
  for (Int_t i = 0; i < 4; i++) {
    grng->SetPoint(i,xarr[i],tf1->Eval(xarr[i]));
  }
  ghst->Draw("l");
  grng->Draw("*");
  
  Double_t area = integ * h->GetBinWidth(1);
  std::cout <<  "counts: " << integ << ", area:" << area << std::endl;
  
  TGraph *ginteg;
  TGraph *garea;
  if (!(ginteg = (TGraph *)gROOT->FindObject("ginteg"))) {
    ginteg = new TGraph();
    ginteg->SetName("ginteg");
  }
  if (!(garea = (TGraph *)gROOT->FindObject("garea"))) {
    garea = new TGraph();
    garea->SetName("garea");
  }

  ginteg->SetPoint(npeaks,(xarr[1]+xarr[2])/2.,integ);
  garea->SetPoint(npeaks,(xarr[1]+xarr[2])/2.,area);
  
  if (gDirectory->IsWritable()) {
    grng->Write();
    tf1->Write();
    ghst->Write();
    ginteg->Write("ginteg",TObject::kOverwrite);
    garea->Write("garea",TObject::kOverwrite);
  }
  return;
}
