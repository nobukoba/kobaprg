void cut_xy(TH1* hist, Double_t x1, Double_t x2, Double_t y1, Double_t y2){
  gROOT->ProcessLine(".L ../cui/clone_with_suffix.C");
  TH2 *hout = (TH2*)gROOT->ProcessLine(Form("clone_with_suffix((TH1*)%p,\"%s\");",hist,"_cut"));
  hout->Reset();
  hout->SetTitle(hist->GetTitle());
  Int_t i1 = hist->GetXaxis()->FindBin(x1);
  Int_t i2 = hist->GetXaxis()->FindBin(x2);
  Int_t j1 = hist->GetYaxis()->FindBin(y1);
  Int_t j2 = hist->GetYaxis()->FindBin(y2);
  Double_t xbinw = hist->GetXaxis()->GetBinWidth(1);
  Double_t ybinw = hist->GetYaxis()->GetBinWidth(1);
  Double_t xfactor1 = (hist->GetXaxis()->GetBinUpEdge(i1) - x1)  / xbinw;
  Double_t xfactor2 = (x2 - hist->GetXaxis()->GetBinLowEdge(i2)) / xbinw;
  Double_t yfactor1 = (hist->GetYaxis()->GetBinUpEdge(j1) - y1)  / ybinw;
  Double_t yfactor2 = (y2 - hist->GetYaxis()->GetBinLowEdge(j2)) / ybinw;
  Double_t xfactor, yfactor;
  for (Int_t i = i1; i <= i2; i++) {
    if      (i == i1) {xfactor = xfactor1;}
    else if (i == i2) {xfactor = xfactor2;}
    else              {xfactor = 1.;}
    for (Int_t j = j1; j <= j2; j++) {
      if      (j == j1) {yfactor = yfactor1;}
      else if (j == j2) {yfactor = yfactor2;}
      else              {yfactor = 1.;}
      hout->Fill(hist->GetXaxis()->GetBinCenter(i),
		 hist->GetYaxis()->GetBinCenter(j),
		 hist->GetBinContent(i,j)*xfactor*yfactor);
    }
  }
  hout->Draw("colz");
  gPad->Update();
  gPad->GetFrame()->SetBit(TBox::kCannotMove);
  return;
}
