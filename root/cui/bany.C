void bany(TH1* hist, Double_t par0, Double_t par1){
  char suffix[] = "_bny";
  Int_t num = 1;
  TString str;
  while (1) {
    str = Form("%s%s%d",hist->GetName(),suffix,num);
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
  hout2->Reset();
  hout2->SetTitle(hist->GetTitle());

  Int_t i1 = hist->GetXaxis()->FindBin(par0);
  Int_t i2 = hist->GetXaxis()->FindBin(par1);
  Double_t binw = hist->GetXaxis()->GetBinWidth(1);
  Double_t factor1 = (hist->GetXaxis()->GetBinUpEdge(i1) - par0)  / binw;
  Double_t factor2 = (par1 - hist->GetXaxis()->GetBinLowEdge(i2)) / binw;
  Double_t factor;
  for (Int_t i = i1; i <= i2; i++) {
    if      (i == i1) {factor = factor1;}
    else if (i == i2) {factor = factor2;}
    else              {factor = 1.;}
    for (Int_t j = 0; j <= hist->GetNbinsY()+1; j++) {
      hout->Fill(hist->GetYaxis()->GetBinCenter(j),
		 hist->GetBinContent(i,j)*factor);
      hout2->Fill(hist->GetXaxis()->GetBinCenter(i),
		  hist->GetYaxis()->GetBinCenter(j),
		  hist->GetBinContent(i,j)*factor);
    }
  }
  hout->Draw();
  gPad->Update();
  gPad->GetFrame()->SetBit(TBox::kCannotMove);
  return;
}
