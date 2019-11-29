void prox(TH1* hist){
  char suffix[] = "_prx";
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
  for (Int_t i = 0; i <= hist->GetNbinsX()+1; i++) {
    for (Int_t j = 1; j <= hist->GetNbinsY(); j++) {
      hout->Fill(hist->GetXaxis()->GetBinCenter(i),
		 hist->GetBinContent(i,j));
    }
  }
  hout->SetEntries(hist->GetEntries());
  hout->Draw();
  gPad->Update();
  gPad->GetFrame()->SetBit(TBox::kCannotMove);
  return;
}
