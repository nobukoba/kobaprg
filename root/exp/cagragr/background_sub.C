#include "TFile.h"
#include "TH2.h"

void background_sub(){
  TFile *f = new TFile("output/date20190727/merged2_th.root");
  TH2 *hd = (TH2*)f->Get("Nobu/GXCEg_rgr_tg_thc");
  TH2 *hb = (TH2*)f->Get("Nobu/GXCEg_rgr_bg_thc");
  hd->Draw("colz");
  hb->Draw("colz");
  
  TH2 *histOut = (TH2*)hd->Clone("histOut");
  //histOut->Reset();
  histOut->Add(hb,-1);
  histOut->Draw("colz");
  double weight = 1.0;
  int r = 10;
  int limit_m = 1.25*3;
  int limit_n = 8.6*3;
  for (int i = 1; i <= hd->GetNbinsX(); i++){
    for (int j = 1; j <= hd->GetNbinsY(); j++){
      for (int k = 0; k < r; k++){
	int m, n;
	for (m = k, n = 0; n < k+1; m--, n++) {
	  if ((m > limit_m) || (n > limit_n)) continue;
	  if (hb->GetBinContent(i, j)==0) break;
	  double difference = hd->GetBinContent(i+n, j+m) - weight*hb->GetBinContent(i, j);
	  double error = histOut->GetBinError(i+n, j+m) * histOut->GetBinError(i+n, j+m);
	  error += weight * weight * hb->GetBinContent(i, j);
	  error = sqrt(error);
	  histOut->SetBinError(i+n, j+m, error);
	  if (difference >= 0.0) {
	    histOut->SetBinContent(i+n, j+m, difference);
	    hb->SetBinContent(i, j, 0);
	  }else{
	    histOut->SetBinContent(i+n, j+m, 0);
	    hb->SetBinContent(i, j, -difference/weight);
	  }
	}
      }
    }  
  }
  histOut->Draw("colz");
  return;  
}
