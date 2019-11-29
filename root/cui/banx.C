void banx(TH1* hist, Double_t par0, Double_t par1){
  char suffix[] = "_bnx";
  Int_t num = 1;
  TString str;
  while (1) {
    str = Form("%s%s%d",hist->GetName(),suffix,num);
    if (gROOT->Get(str.Data())==0) {break;}
    num++;
  }
  TDirectory *save = gDirectory;
  gROOT->cd();
  TH1D *hout = new TH1D(str, hist->GetTitle(), hist->GetNbinsX(),
			hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
  save->cd();
  gROOT->ProcessLine(".L ../cui/clone_with_suffix.C");
  TH2 *hout2 = (TH2*)gROOT->ProcessLine(Form("clone_with_suffix((TH1*)%p,\"%s\");",hist,"_cut"));
  hout2->Reset();
  hout2->SetTitle(hist->GetTitle());
  
  Int_t j1 = hist->GetYaxis()->FindBin(par0);
  Int_t j2 = hist->GetYaxis()->FindBin(par1);
  Double_t binw = hist->GetYaxis()->GetBinWidth(1);
  Double_t factor1 = (hist->GetYaxis()->GetBinUpEdge(j1) - par0)  / binw;
  Double_t factor2 = (par1 - hist->GetYaxis()->GetBinLowEdge(j2)) / binw;
  Double_t factor;
  for (Int_t j = j1; j <= j2; j++) {
    if      (j == j1) {factor = factor1;}
    else if (j == j2) {factor = factor2;}
    else              {factor = 1.;}
    for (Int_t i = 0; i <= hist->GetNbinsX()+1; i++) {
      hout->Fill(hist->GetXaxis()->GetBinCenter(i),
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
