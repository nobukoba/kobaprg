#include <iostream>
#include "TROOT.h"
#include "TSystem.h"
#include "TH2.h"
#include "TF1.h"
#include "TGraph.h"
#include "TList.h"
#include "TLine.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TString.h"

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

void fit_emg() { // I could not add const modifier because of h->Fit(f[i],"R")!
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
  gPad->SetCrosshair(0);
  TVirtualPad *sel_pad = gROOT->GetSelectedPad();
  if (sel_pad == 0) {
    std::cout << "There is no sel_pad." << std::endl;
    gr->Delete();
    return;
  }

  TList *listofpri = sel_pad->GetListOfPrimitives();
  TH1 *hist = 0;
  TIter next(listofpri); TObject *obj;
  hist = 0;
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
  if (!hist) {
    std::cout << "TH1 histogram was not found in this pad." << std::endl;
    return;
  }
  
  Int_t    np    = gr->GetN();
  Int_t    ng    = np-2;
  if (ng < 1) {
    std::cout << "The number of points >= 3." << std::endl;
    return;
  }

  Double_t *xarr = gr->GetX();
  Double_t *yarr = gr->GetY();
  Double_t x1    = xarr[0];
  Double_t x2    = xarr[np-1];
  Double_t y1    = yarr[0];
  Double_t y2    = yarr[np-1];
  Double_t bwid  = hist->GetBinWidth(1);
  std::cout << ng << std::endl;
  
  //std::vector<TF1*> f(ng);
  TF1** f = new TF1*[ng];
  //std::vector<Double_t> par(2+ng*3);
  Double_t *par = new Double_t [2+ng*3];
  for (Int_t i = 0; i < ng; i++) {
    Double_t xlow = xarr[i+1] - bwid*10;
    Double_t xup  = xarr[i+1] + bwid*10;
    if (xlow < x1) {xlow = x1;}
    if (xup  > x2) {xup  = x2;}
    if ((i !=0) && (xlow < (xarr[i]+xarr[i+1])/2.)) {
      xlow = (xarr[i]+xarr[i+1])/2.;
    }
    if ((i !=ng-1) && (xup > (xarr[i+1]+xarr[i+2])/2.)) {
      xup = (xarr[i+1]+xarr[i+2])/2.;
    }
    f[i] = new TF1(Form("f%d",i),"gaus",xlow,xup);
    f[i]->SetLineWidth(1);
    f[i]->FixParameter(1,xarr[i+1]);
    if (i == 0) {
      hist->Fit(f[i],"R");
    }else{
      hist->Fit(f[i],"R+");
    }
    f[i]->GetParameters(&(par[2+i*3]));
    delete f[i];
  }
  delete [] f;

  Int_t option = 1;
  TString s;
  if (option == 0) {
    s = "pol1(0)";
    par[0] = (y1*x2-y2*x1)/(x2-x1);
    par[1] = (y2-y1)/(x2-x1);
    for (Int_t i = 0; i < ng; i++) {
      par[2+i*3] = par[2+i*3] - (par[0]+par[1]*xarr[i+1]);
    }
  }else{
    s = "expo(0)";
    par[0] = (log(y1)*x2-log(y2)*x1)/(x2-x1);
    par[1] = (log(y2)-log(y1))/(x2-x1);
    for (Int_t i = 0; i < ng; i++) {
      par[2+i*3] = par[2+i*3] - exp(par[0]+par[1]*xarr[i+1]);
    }
  }
  for (Int_t i = 0; i < ng; i++) {
    s += Form("+gaus(%d)",2+i*3);
  }

  TF1* fsum = new TF1("fsum",s,x1,x2);
  fsum->SetLineWidth(1);
  fsum->SetParameters(&(par[0]));
  fsum->SetParName(0,"Constant");
  fsum->SetParName(1,"Slope");
  for (Int_t i = 0; i < ng; i++) {
    fsum->SetParName(2+i*3,  Form("Constant_%d",i+1));
    fsum->SetParName(2+i*3+1,Form("Mean_%d",i+1));
    fsum->SetParName(2+i*3+2,Form("Sigma_%d",i+1));
  }

  hist->Fit(fsum,"R+");
  //hist->Draw();

  TF1* fit =  hist->GetFunction("fsum");
  for (Int_t i = 0; i < ng; i++) {
    Double_t cons   = fit->GetParameter(i*3+2);
    Double_t sigma  = fit->GetParameter(i*3+4);
    Double_t econs  = fit->GetParError(i*3+2);
    Double_t esigma = fit->GetParError(i*3+4);
    Double_t area   = fabs(cons*sigma*sqrt(2*3.141592));
    Double_t earea  = area*sqrt(pow((econs/cons),2)+pow((esigma/sigma),2));
    Double_t cnts   = area / bwid;
    Double_t ecnts  = earea / bwid;
    
    std::cout << "Peak #" << i+1
	      << ": area =" << area << " +/- " << earea
	      << ", counts =" << cnts << " +/- " << ecnts
	      << std::endl;
  }
  
  sel_pad->Update();
  sel_pad->Modified();
  delete [] par;
  return;
}
