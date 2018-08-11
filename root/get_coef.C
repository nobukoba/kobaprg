#include <iostream>
#include "TROOT.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TFile.h"
#include "TCutG.h"
#include "TMatrixD.h"
#include "TPad.h"
#include "TF1.h"
#include "TF2.h"

void get_coef(){
  TFile *filein  = new TFile("run5023_thir_thcxyg_cut.root","READ");
  TFile *fileout = new TFile("run5023_thir_thcxyg_coef.root","RECREATE");
  TH2D  *hin     = (TH2D*)filein->Get("hin");

  const Int_t number_of_peaks = 3;
  const Int_t degree_of_polN  = 4;
  TF1 *fit_func  = new TF1("fit_func",Form("pol%d",degree_of_polN),-10.,10.);
  for (Int_t i = 0; i < number_of_peaks; i++) {
    hin->Draw("colz");
    TCutG    *cutg = (TCutG*)gPad->WaitPrimitive("CUTG","CutG");
    TProfile *pfy  = hin->ProfileY(Form("pfy_%d",i),1,-1,"[CUTG]");
    pfy->Fit(fit_func);
    cutg->SetName(Form("CUTG_%d",i));
    cutg->Write();
    pfy->Write();
  }
  hin->Write();
  
  TMatrixD fit_pars(number_of_peaks, degree_of_polN+1);
  for (Int_t i = 0; i < number_of_peaks; i++) {
    TProfile *pfy      = (TProfile *)gROOT->FindObject(Form("pfy_%d",i));
    TF1      *tf1_ptr  = pfy->GetFunction("fit_func");
    for (Int_t j = 0; j < degree_of_polN+1; j++) {
      fit_pars[i][j] = tf1_ptr->GetParameter(j);
      std::cout << "i, j, fit_pars: " << i << ", " << j << ", " << fit_pars[i][j] << std::endl;
    }
  }
  
  TMatrixD m(number_of_peaks, number_of_peaks);
  for (Int_t i = 0; i < number_of_peaks; i++) {
    for (Int_t j = 0; j < number_of_peaks; j++) {
      m[i][j] = pow(fit_pars[i][0],j);
      std::cout << "i, j, m: " << i << ", " << j << ", " << m[i][j] << std::endl;
    }      
  }
  
  TMatrixD m_inv(number_of_peaks, number_of_peaks);
  m_inv = m.Invert();
  
  TMatrixD coef(number_of_peaks, degree_of_polN+1);
  for (Int_t j = 0; j < degree_of_polN+1; j++) {
    for (Int_t i = 0; i < number_of_peaks; i++) {
      coef[i][j] = 0.;
      for (Int_t k = 0; k < number_of_peaks; k++) {
	coef[i][j] += m_inv[i][k] * -fit_pars[k][j];
      }
      std::cout << "i, j, coef: " << i << ", " << j << ", " << coef[i][j] << std::endl;
    }
  }
  for (Int_t i = 0; i < number_of_peaks; i++) {
    if (i == 1) {
      coef[i][0] = 1.;
    }else{
      coef[i][0] = 0.;
    }
  }
  coef.Write("coef");
}
