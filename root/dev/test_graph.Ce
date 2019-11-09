//#include <iostream>
//#include "TROOT.h"
//#include "TSystem.h"
//#include "TF1.h"
//#include "TH1.h"
//#include "TH2.h"
//#include "TGraph.h"
//#include "TPad.h"
//#include "TCanvas.h"
//#include "TList.h"
//#include "TLine.h"
//#include "TCutG.h"
//#include "TString.h"

TGraph * MyWaitPrimitive(Int_t number_of_points) {
  //if (!gPad) {
  //  std::cout << "There is no gPad." << std::endl;
  //  return 0;
  //}
  TCanvas* canvas = gPad->GetCanvas();
  TVirtualPad *sel_pad = gROOT->GetSelectedPad();
  TList *listofpri = sel_pad->GetListOfPrimitives();

  TGraph gr;
  Int_t fCrosshairPos = 0;
  Int_t pxlast = 0, pylast = 0;
  Int_t event = 0;

  //graphical_cut->SetPoint(0,100.,500.);
  //graphical_cut->SetPoint(1,300.,220.);
  //std::cout << "graphical_cut->GetN()"
  //	    << graphical_cut->GetN()
  //	    << std::endl;
  //graphical_cut->Draw("L*");
  //return graphical_cut;
  
  //graphical_cut->Set(0);

  //Int_t k = 0;
  while (!gSystem->ProcessEvents() && gROOT->GetSelectedPad()) {
  //while (!gSystem->ProcessEvents()) {
  //while (1) {
    event = gPad->GetEvent();
    std::cout << "event: " << event <<std::endl;
    
    if (event == kButton1Double) {
      //gSystem->Sleep(1000);
      //graphical_cut->Draw("L*");
      //canvas->HandleInput((EEventType)-1,0,0);
      break;
    }
    if (event == kButton1Down) {
      pxlast = gPad->GetEventX();
      pylast = gPad->GetEventY();
      Double_t x = gPad->AbsPixeltoX(pxlast);
      Double_t y = gPad->AbsPixeltoY(pylast);
//std::cout << "graphical_cut->GetN(), k, x, y: "
//		<< graphical_cut->GetN() << ", "
//		<< k << ", "
//		<< x << ", "
//		<< y << ", "
//		<< std::endl;
      //graphical_cut->SetPoint(k, x, y);
      //k++;
      //graphical_cut->Draw("L*");
      gr.SetPoint(gr.GetN(), x, y);
      gr.Draw("L*");
      canvas->HandleInput((EEventType)-1,0,0);
      gSystem->Sleep(100);
    }
    gSystem->Sleep(10);
  }

  return (new TGraph(gr));
}

void test_graph(){
  gPad->SetCrosshair();
  TGraph * gr = MyWaitPrimitive(0);
  gPad->SetCrosshair(0);
  
  //TVirtualPad *sel_pad = gROOT->GetSelectedPad();
  //if (sel_pad == 0) {
  //  std::cout << "There is no sel_pad." << std::endl;
  //  gr->Delete();
  //  return;
  //}
  //TList *listofpri = sel_pad->GetListOfPrimitives();
  //TIter next(listofpri);
  //TObject *obj;
  //TH1 *hist = 0;
  //while (obj = next()){
  //  if (obj->InheritsFrom("TH1")) {
  //    hist = (TH1*)obj;
  //    std::cout << "hist was found." << std::endl;
  //    break;
  //  }
  //}
  //if(hist == 0){
  //  std::cout << "Histogram was not found in this pad." << std::endl;
  //  gr->Delete();
  //  return;
  //}
  //Int_t j = 0;
  //while(listofpri->FindObject(Form("Graph_%d",j))){
  //  j++;
  //}
  //gr->SetName(Form("Graph_%d",j));
  gr->Draw("L*");
  gPad->Update();
  return;
}
