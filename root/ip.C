#include <iostream>
#include "TROOT.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include "TObject.h"
#include "TH2.h"
#include "TF1.h"
#include "TGraph.h"
#include "TLine.h"
#include "TPad.h"
#include "TString.h"


TGraph * MyWaitPrimitive(Int_t number_of_points) {
  if (!gPad) {
    std::cout << "There is no gPad." << std::endl;
    return 0;
  }
  TCanvas* canvas = gPad->GetCanvas();
  TVirtualPad *sel_pad = gROOT->GetSelectedPad();
  TList *listofpri = sel_pad->GetListOfPrimitives();
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

void ip() {
  TCanvas* canvas;
  if (!(canvas = gPad->GetCanvas())) {
    std::cout << "There is no canvas." << std::endl;
    return;
  }

  gPad->SetCrosshair();
  TGraph *gr = MyWaitPrimitive(4);
  gPad->SetCrosshair(0);
  Double_t xarr[4];
  xarr[0] = (gr->GetX())[0];
  xarr[1] = (gr->GetX())[1];
  xarr[2] = (gr->GetX())[2];
  xarr[3] = (gr->GetX())[3];
  gr->Delete();

  TVirtualPad *sel_pad = gROOT->GetSelectedPad();
  TList *listofpri = sel_pad->GetListOfPrimitives();
  TIter next(listofpri);
  TObject *obj;
  TH1 *hist = 0;
  while ((obj = next())){
    if (obj->InheritsFrom("TH2")) {
      std::cout << "This script can not handle TH2 histograms." << std::endl;
      return;
    }
    if (obj->InheritsFrom("TH1")) {
      hist = (TH1*)obj;
      break;
    }
  }
  if(hist == 0){
    std::cout << "TH1 histogram was not found in this pad." << std::endl;
    return;
  }
    
  Int_t npeaks = 0;
  while (listofpri->FindObject(Form("grng_%d",npeaks))) {
    npeaks++;
  }
  TGraph *gfit = new TGraph();
  TGraph *ghst = new TGraph();
  gfit->SetName(Form("gfit_%d",npeaks));
  ghst->SetName(Form("ghst_%d",npeaks));
  Int_t k = 0;
  Int_t l = 0;
  for (Int_t i = 1; i <= hist->GetNbinsX(); i++) {
    Double_t xx = hist->GetXaxis()->GetBinCenter(i);
    if (((xx >= xarr[0])&&(xx <= xarr[1]))
	||((xx >= xarr[2])&&(xx <= xarr[3]))) {
      gfit->SetPoint(k++,xx,hist->GetBinContent(i));
    }
    if ((xx >= xarr[1])&&(xx <= xarr[2])) {
      ghst->SetPoint(l++,xx,hist->GetBinContent(i));
    }
  }
  
  TF1 *tf1 = new TF1(Form("tf1_%d",npeaks),"pol1",hist->GetXaxis()->GetXmin(),hist->GetXaxis()->GetXmax());
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
  
  Double_t area = integ * hist->GetBinWidth(1);
  std::cout <<  "counts: " << integ << ", area:" << area << std::endl;
  
  TGraph *ginteg;
  TGraph *garea;
  if (!(ginteg = (TGraph *)listofpri->FindObject("ginteg"))) {
    ginteg = new TGraph();
    ginteg->SetName("ginteg");
  }
  if (!(garea = (TGraph *)listofpri->FindObject("garea"))) {
    garea = new TGraph();
    garea->SetName("garea");
  }

  ginteg->SetPoint(npeaks,(xarr[1]+xarr[2])/2.,integ);
  garea->SetPoint(npeaks,(xarr[1]+xarr[2])/2.,area);
  gPad->Update();
  return;
}
