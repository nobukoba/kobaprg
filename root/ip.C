#include <iostream>
//#include <iomanip>
#include "TROOT.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include "TObject.h"
#include "TH2D.h"
#include "TF1.h"
#include "TGraph.h"
#include "TLine.h"
#include "TPad.h"
#include "TString.h"

TGraph * MyWaitPrimitive(Int_t number_of_points) {
  TCanvas* canvas = gPad->GetCanvas();
  if (!canvas) {
    std::cout << "There is no canvas." << std::endl;
    return 0;
  }
  graphical_cut = new TGraph();
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

void ip() {
  TCanvas* canvas;
  if (!(canvas = gPad->GetCanvas())) {
    std::cout << "There is no canvas." << std::endl;
    return;
  }

  gPad->SetCrosshair();
  TGraph *gr = MyWaitPrimitive(4);
  gPad->SetCrosshair(0);

  TVirtualPad *sel_pad = canvas->GetPad(gPad->GetNumber());
  if (sel_pad == 0) {
    std::cout << "There is no sel_pad." << std::endl;
    return;
  }
  TList *listofpri = sel_pad->GetListOfPrimitives();
  if (listofpri == 0) {
    std::cout << "The pad includes nothing." << std::endl;
    return;
  }
  TIter next(listofpri);
  TObject *obj;
  TH1 *hist = 0;
  while (obj = next()){
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
  
  Double_t xarr[4];
  xarr[0] = (gr->GetX())[0];
  xarr[1] = (gr->GetX())[1];
  xarr[2] = (gr->GetX())[2];
  xarr[3] = (gr->GetX())[3];
  gr->Delete();
  
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
