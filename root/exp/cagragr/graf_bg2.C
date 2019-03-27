#include <iostream>
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TMath.h"

void graf_bg2(){
  std::cout << std::endl << "Macro: graf_bg2.C "<< std::endl;
  TNamed *named = (TNamed*)gROOT->FindObjectAny("initial_working_dir");
  if (named) {gSystem->cd(named->GetTitle());}
  std::cout << "gSystem->pwd(): " << gSystem->pwd() << std::endl;
  
  TList * lof = (TList *)gROOT->GetListOfFiles();
  TIter next(lof);
  TFile *file;
  
  Int_t runnum[1] = {1521};
  for (Int_t j = 0; j < 1; j++) {
    
    //file = TFile::Open(Form("output/date20190327/run%04d/hist_graf_rate_20190327.root",runnum[j]));
    file = TFile::Open(Form("output/date20190327/run%04d/hist_graf_rate_20190327_ch.root",runnum[j]));
    TH2 * hist1 = (TH2*)file->Get("Nobu/diffcagra"); 
    TH2 * hist2 = (TH2*)file->Get("Nobu/eachcagra"); 
    std::cout << "runnum " << runnum[j] << " counts";
    Int_t biny1 = hist2->GetYaxis()->FindBin(0.);
    Int_t biny2 = hist2->GetYaxis()->FindBin(10000.);
    double deno = hist2->Integral(204, 204,biny1,biny2);
    std::cout << "deno " << deno << std::endl;
    
    for (Int_t i=1; i <=250; i++) {
      Int_t biny3 = hist1->GetYaxis()->FindBin(100.);
      Int_t biny4 = hist1->GetYaxis()->FindBin(150.);
      std::cout << " " << hist1->Integral(i, i,biny3,biny4)/deno/500e-9;
      //std::cout << " " << hist1->Integral(i, i,biny3,biny4);
    }
    std::cout << std::endl;
    hist1->Draw();
    gPad->WaitPrimitive();
    file->Close();
  }
  
  return;
  
  TFile *newfile = TFile::Open("graf_bg.root","recreate");
  TH2D *hout[48];
  for (Int_t i = 0; i < 48; i++) {
    hout[i] = new TH2D(Form("h%02d",i),Form("h%02d",i),118,2065.5,2183.5,10000,0,10000);
  }
  
  while (file = (TFile*)next()){
    TString filename = file->GetName();
    if(filename.EndsWith(".root")){
      filename.Resize(filename.Length()-5);
    }
    TString runnumstr = filename(23,4);
    Int_t runnum = runnumstr.Atoi();
    
    
    if (runnum == 0) {continue;}
    std::cout << "filename: " << filename << std::endl;
    std::cout << "runnum:   " << runnum << std::endl;

    TH2 * hist = (TH2*)file->Get("Nobu/IDEneNS"); 
    if (!hist){
      std::cout << "hist is null. skip."<< std::endl;
      continue;
    }

    for (Int_t ibin = 1; ibin <= 48; ibin++) {
      for (Int_t j = 0; j <= 10001; j++) {
	(hout[ibin-1])->Fill(runnum,
			     hist->GetYaxis()->GetBinCenter(j),
			     hist->GetBinContent(ibin,j));
      }      
    }
    
    //newfile += "_fitted.root";
    //std::cout << "newfile name: " << newfile << std::endl;
    //TH2 * hist = (TH2*)file->Get("Nobu/IDEneNS"); 
    //std::cout << << std::endl;

//    if (!hist){
//      std::cout << "hist is null. skip."<< std::endl;
//      continue;
//    }
//    
//    TFile *h_newfile = TFile::Open(newfile.Data(),"recreate");
//    TList *file_list = h_newfile->GetList();
//    
//    TString str = hist->GetName();
//    str += "_sly_";
//    TString str_n = str;
//    Int_t num = 1;
//    while (1) {
//      TIter next2(file_list);
//      TObject *obj;
//      Int_t found = 0;
//      while (obj = next2()) {
//	if (!(obj->InheritsFrom("TH1"))) {
//	  continue;
//	} 
//	TString name = obj->GetName();
//	if (name.BeginsWith(str_n)){
//	  found = 1;
//	  break;
//	}
//      }
//      if (found == 1) {
//	str_n = Form("%s_sly%d_",hist->GetName(),num);
//	num++;
//      }else{
//	break;
//      }
//    }
//    Int_t nbins = hist->GetNbinsX();
//    Int_t ndig = 1;
//    while (nbins > (TMath::Power(10, ndig)-1)) {
//      ndig++;
//    }
//    for (Int_t i = 1; i <= hist->GetNbinsX(); i++) {
//      TString formatstr = Form("%s%%0%dd",str_n.Data(),ndig);
//      TString hname = Form(formatstr.Data(),i);
//      TH1D *hout = new TH1D(hname, hist->GetTitle(), hist->GetNbinsY(),
//			    hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax());
//      for (Int_t j = 0; j <= hist->GetNbinsY()+1; j++) {
//	hout->Fill(hist->GetYaxis()->GetBinCenter(j),
//		   hist->GetBinContent(i,j));
//	
//	
//      }
//      hout->GetXaxis()->SetRangeUser(480,550);
//      Double_t maxbin = hout->GetMaximumBin();
//      Double_t maxcen = hout->GetBinCenter(maxbin);
//      Double_t bwid   = hout->GetBinWidth(maxbin);
//      Double_t xlow   = maxcen - 5*bwid;
//      Double_t xup    = maxcen + 5*bwid;
//      Double_t xrang  = xup - xlow;
//      TF1* fgaus = new TF1("fit_narrow_gaus_init_0", "gaus", xlow, xup);
//      Double_t ymax = hout->GetBinContent(hout->GetMaximumBin());
//      if (ymax == 0.) {
//	std::cout << hout->GetName() << ": ymax == 0. skip."<< std::endl;
//	continue;
//      }
//      fgaus->SetParLimits(0, 0., ymax*10000.);
//      fgaus->SetParLimits(1, xlow, xup);
//      fgaus->SetParLimits(2, xrang/10., xrang);
//      hout->Fit(fgaus,"R+");
//      
//      Double_t par[3];
//      par[0] = fgaus->GetParameter(0);
//      par[1] = fgaus->GetParameter(1);
//      par[2] = fgaus->GetParameter(2);
//      TList *funclist = hout->GetListOfFunctions();
//      funclist->Last()->Delete();
//      xlow = par[1] - 0.75 * par[2];
//      xup  = par[1] + 0.75 * par[2];
//      TF1* fit_narrow_gaus = new TF1("fit_narrow_gaus_0","gaus",xlow,xup);
//      fit_narrow_gaus->SetLineWidth(1);
//      hout->Fit(fit_narrow_gaus,"R+");
//      hout->Draw();
//      gPad->Update();
//      gPad->Modified();
//    }
//    h_newfile->Write();
//    h_newfile->Close();
  }
  newfile->Write();
  newfile->Close();
  return;
}
