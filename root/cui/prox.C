void prox(TH1* hist){
  TString str = hist->GetName();
  str += "_prx";
  TString str_n = str;
  Int_t num = 1;
  while (gROOT->Get(str_n.Data())) {
    str_n = Form("%s%d",str.Data(),num);
    num++;
  }
  
  TDirectory *save = gDirectory;
  gROOT->cd();
  TH1D *hout = new TH1D(str_n, hist->GetTitle(), hist->GetNbinsX(),
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
