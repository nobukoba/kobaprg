#include <iostream>
#include "TROOT.h"
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
#include "TMarker.h"

void copy_with_cutg(){
  TCanvas* canvas = gPad->GetCanvas();
  if (canvas == 0) {
    std::cout << "There is no canvas." << std::endl;
    return;
  }
  //TCutG *cutg = (TCutG*)(gPad->GetListOfPrimitives()->FindObject("CUTG"));
  //if (cutg != 0){
  //  cutg->Delete();
  //}


  //TVirtualPad *sel_pad  = canvas->GetSelectedPad();
  TVirtualPad *sel_pad;
  if (!(sel_pad = canvas->GetPad(gPad->GetNumber()))) {
    //if (!sel_pad) {
    std::cout << "There is no selected pad." << std::endl;
    return;
  }
  sel_pad->cd();

  //TList *listofpri = sel_pad->GetListOfPrimitives();
  TList *listofpri = canvas->GetListOfPrimitives();
  if (listofpri == 0) {
    std::cout << "The canvas includes nothing." << std::endl;
    return;
  }
  TIter next(listofpri);
  TObject *obj;
  TH2 *hist = 0;
  while (obj = next()){
    if (obj->InheritsFrom("TH2")) {
      hist = (TH2*)obj;
      std::cout << "TH2 hist was found." << std::endl;
      break;
    }
  }
  if(hist == 0){
    std::cout << "TH2 histogram was not found in this pad." << std::endl;
    return;
  }

  gPad->SetCrosshair();

  //TMarker *mk = (TMarker*)canvas->WaitPrimitive("TMarker","Marker");
  //delete mk;
  TCutG *cutg;

  cutg  = (TCutG*)(sel_pad->WaitPrimitive("CUTG","CutG"));
  gPad->SetCrosshair(0);


  gROOT->cd();
  TString str = hist->GetName();
  str += "_cut";
  TString str_n = str;
  Int_t num = 1;
  while (gROOT->Get(str_n.Data())) {
    str_n = Form("%s%d",str.Data(),num);
    num++;
  }
  TH2 *hout = (TH2*)hist->Clone(str_n);
  hout->SetTitle(hist->GetTitle());
  //hout->Reset();
  Double_t xx, yy;
  for (Int_t i = 0; i <= hist->GetNbinsX()+1; i++) {
    for (Int_t j = 0; j <= hist->GetNbinsY()+1; j++) {
      xx = hist->GetXaxis()->GetBinCenter(i);
      yy = hist->GetYaxis()->GetBinCenter(j);
      if (cutg->IsInside(xx,yy) != 1) {
	hout->SetBinContent(i,j,0);
      }
    }
  }
  hout->Draw("colz");
  canvas->Update();
  return;
}
