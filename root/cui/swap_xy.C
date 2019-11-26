void swap_xy(TH1* hist){
  TString str = hist->GetName();
  str += "_swp";
  TString str_n = str;
  Int_t num = 1;
  while (gROOT->Get(str_n.Data())) {
    str_n = Form("%s%d",str.Data(),num);
    num++;
  }
  TDirectory *save = gDirectory;
  gROOT->cd();
  TH2 *hout = (TH2*)hist->Clone(str_n);
  save->cd();
  hout->SetTitle(hist->GetTitle());
  hout->Reset();
  hout->SetBins(hist->GetNbinsY(),hist->GetYaxis()->GetXmin(),hist->GetYaxis()->GetXmax(),
		hist->GetNbinsX(),hist->GetXaxis()->GetXmin(),hist->GetXaxis()->GetXmax());
  for (Int_t i=0; i <= hist->GetNbinsX()+1; i++){
    for (Int_t j=0; j <= hist->GetNbinsY()+1; j++){
      hout->SetBinContent(j,i,hist->GetBinContent(i,j));
    }
  }
  hout->SetEntries(hist->GetEntries());
  hout->Draw("colz");
  gPad->Modified();
  gPad->Update();
  gPad->GetFrame()->SetBit(TBox::kCannotMove);
  return;
}
