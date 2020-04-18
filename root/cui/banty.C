void banty(TH1* hist, Double_t par0, Double_t par1, Double_t par2){
  char suffix[] = "_bty";
  Int_t num = 1;
  TString str;
  while (1) {
    str = Form("%s%s%d", hist->GetName(), suffix, num);
    if (gROOT->Get(str.Data())==0) {break;}
    num++;
  }
  TDirectory *save = gDirectory;
  gROOT->cd();
  TH1D *hout = new TH1D(str, hist->GetTitle(), hist->GetNbinsY(),
			hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax());
  save->cd();
  gROOT->ProcessLine(".L ../cui/clone_with_suffix.C");
  TH2 *hout2 = (TH2*)gROOT->ProcessLine(Form("clone_with_suffix((TH1*)%p,\"%s\");",hist,"_cut"));
  /*hout2->Reset();*/
  hout2->SetTitle(hist->GetTitle());

  Double_t xmin = hist->GetXaxis()->GetXmin();
  Double_t xmax = hist->GetXaxis()->GetXmax();
  Double_t ymin_xmin = par0 * xmin + par1;
  Double_t ymax_xmin = par0 * xmin + par2;
  Double_t ymin_xmax = par0 * xmax + par1;
  Double_t ymax_xmax = par0 * xmax + par2;
  
  TGraph *gr = new TGraph();
  gr->SetPoint(0, xmin, ymin_xmin);
  gr->SetPoint(1, xmin, ymax_xmin);
  gr->SetPoint(2, xmax, ymax_xmax);
  gr->SetPoint(3, xmax, ymin_xmax);
  TCutG *cutg = new TCutG("CUTG",gr->GetN(),
			  gr->GetX(),
			  gr->GetY());
  for (Int_t i = 0; i <= hist->GetNbinsX()+1; i++) {
    Double_t xx = hist->GetXaxis()->GetBinCenter(i);
    for (Int_t j = 0; j <= hist->GetNbinsY()+1; j++) {
      Double_t yy = hist->GetYaxis()->GetBinCenter(j);
      if (cutg->IsInside(xx,yy) != 1) {
	hout2->SetBinContent(i,j,0);
      }
    }
  }
  
  for (Int_t i = 1; i <= hout2->GetNbinsX(); i++) {
    for (Int_t j = 0; j <= hout2->GetNbinsY()+1; j++) {
      hout->Fill(hout2->GetYaxis()->GetBinCenter(j),
		 hout2->GetBinContent(i,j));
    }
  }
  hout->Draw();
  gPad->Update();
  gPad->GetFrame()->SetBit(TBox::kCannotMove);
  return;
}
