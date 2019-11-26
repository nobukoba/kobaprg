void bany(TH1* hist, Double_t par0, Double_t par1){
  TString str = hist->GetName();
  str += "_bny";
  TString str_n = str;
  Int_t num = 1;
  while (gROOT->Get(str_n.Data())) {
    str_n = Form("%s%d",str.Data(),num);
    num++;
  }

  TString str2 = hist->GetName();
  str2 += "_cut";
  TString str2_n = str2;
  Int_t num2 = 1;
  while (gROOT->Get(str2_n.Data())) {
    str2_n = Form("%s%d",str2.Data(),num);
    num2++;
  }
  
  TDirectory *save = gDirectory;
  gROOT->cd();
  TH1D *hout = new TH1D(str_n, hist->GetTitle(), hist->GetNbinsY(),
			hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax());
  TH2 *hout2 = (TH2*)hist->Clone(str2_n);
  save->cd();
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
