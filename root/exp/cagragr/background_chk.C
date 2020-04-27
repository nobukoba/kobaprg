#include "TFile.h"
#include "TH2.h"

void background_chk(){
  TFile *f = new TFile("output/date20190727/merged2_th.root");
  TH2 *hd = (TH2*)f->Get("Nobu/GXCEg_rgr_tg_thc");
  TH2 *hb = (TH2*)f->Get("Nobu/GXCEg_rgr_bg_thc");

  hd->GetYaxis()->SetLimits(hd->GetYaxis()->GetXmin()/1000.,
			      hd->GetYaxis()->GetXmax()/1000.);
  hd->GetXaxis()->SetLimits(0.006585*hd->GetXaxis()->GetXmin() + 5.051251,
			      0.006585*hd->GetXaxis()->GetXmax() + 5.051251);
  hb->GetYaxis()->SetLimits(hb->GetYaxis()->GetXmin()/1000.,
			      hb->GetYaxis()->GetXmax()/1000.);
  hb->GetXaxis()->SetLimits(0.006585*hb->GetXaxis()->GetXmin() + 5.051251,
			      0.006585*hb->GetXaxis()->GetXmax() + 5.051251);

  TH2 *hd2 = (TH2*)hd->Clone("hd2");
  TH2 *hb2 = (TH2*)hb->Clone("hb2");
  TGraph *gr = new TGraph();
  Double_t hwid = 0.1;
  gr->SetPoint(0, 0.,  hwid);
  gr->SetPoint(1, 0.,   11.);
  gr->SetPoint(2,10.,  11.);
  gr->SetPoint(3,10.,10.+hwid);
  gr->SetPoint(4, 0.,  hwid);
  TCutG *cutg = new TCutG("CUTG",gr->GetN(),
			  gr->GetX(),
			  gr->GetY());
  for (Int_t i = 0; i <= hd->GetNbinsX()+1; i++) {
    Double_t xx = hd->GetXaxis()->GetBinCenter(i);
    for (Int_t j = 0; j <= hd->GetNbinsY()+1; j++) {
      Double_t yy = hd->GetYaxis()->GetBinCenter(j);
      if (cutg->IsInside(xx,yy) != 1) {
	hd2->SetBinContent(i,j,0);
      }
    }
  }
  TH1D *hdprx = new TH1D("hdprox", hd2->GetTitle(), hd2->GetNbinsX(),
			hd2->GetXaxis()->GetXmin(), hd2->GetXaxis()->GetXmax());
  for (Int_t i = 0; i <= hd2->GetNbinsX()+1; i++) {
    for (Int_t j = 1; j <= hd2->GetNbinsY(); j++) {
      hdprx->Fill(hd2->GetXaxis()->GetBinCenter(i),
		 hd2->GetBinContent(i,j));
    }
  }
  hdprx->SetEntries(hd2->GetEntries());

  for (Int_t i = 0; i <= hb->GetNbinsX()+1; i++) {
    Double_t xx = hb->GetXaxis()->GetBinCenter(i);
    for (Int_t j = 0; j <= hb->GetNbinsY()+1; j++) {
      Double_t yy = hb->GetYaxis()->GetBinCenter(j);
      if (cutg->IsInside(xx,yy) != 1) {
	hb2->SetBinContent(i,j,0);
      }
    }
  }
  TH1D *hbprx = new TH1D("hbprox", hb2->GetTitle(), hb2->GetNbinsX(),
			hb2->GetXaxis()->GetXmin(), hb2->GetXaxis()->GetXmax());
  for (Int_t i = 0; i <= hb2->GetNbinsX()+1; i++) {
    for (Int_t j = 1; j <= hb2->GetNbinsY(); j++) {
      hbprx->Fill(hb2->GetXaxis()->GetBinCenter(i),
		 hb2->GetBinContent(i,j));
    }
  }
  hbprx->SetEntries(hb2->GetEntries());

  hbprx->SetLineColor(kRed);
  //hbprx->Draw();
  //hdprx->Draw("same");
  TH1D * hdsub = hdprx->Clone("hdsub");
  hdsub->Add(hbprx,-1);
  hdsub->Draw();
  //hb2->Draw("colz");
  //hb2->Draw("colz");
  //hd2->Draw("colz");
  //hb->Draw("colz");
  
  return;
}
